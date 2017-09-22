#include <mutex>

#include "Application.h"
#include "request/Request.h"
#include "response/JsonResponse.h"
#include "dispatcher/Dispatcher.h"

int onyx::Application::m_socket_id;
std::vector<std::thread> onyx::Application::m_threads;

std::unique_ptr<onyx::Dispatcher> onyx::Application::m_dispatcher(new Dispatcher);

std::mutex onyx::Application::m_mutex_class;
std::unique_ptr<plog::RollingFileAppender<plog::TxtFormatter>> onyx::Application::m_file_log_appender(nullptr);
std::unique_ptr<plog::ColorConsoleAppender<plog::TxtFormatter>> onyx::Application::m_console_log_appender(new plog::ColorConsoleAppender<plog::TxtFormatter>);

std::string onyx::Application::m_socket_path;
std::string onyx::Application::m_log_file_path;
size_t onyx::Application::m_thread_count = 8;

void onyx::Application::run() {

    for (size_t i = 0; i < m_thread_count; i++)
        m_threads.push_back(std::thread(handler));

    for (auto& thread : m_threads)
        thread.join();

}

void onyx::Application::handler() {
    int rc;
    FCGX_Request request;
    if (FCGX_InitRequest(&request, m_socket_id, 0) != 0)
        return;
    for (;;) {
        m_mutex_class.lock();
        rc = FCGX_Accept_r(&request);
        m_mutex_class.unlock();

        if (rc < 0)
            continue;

        const char * server_name = FCGX_GetParam("SERVER_NAME", request.envp);
        const char * request_ip_address = FCGX_GetParam("REMOTE_ADDR", request.envp);
        const char * request_url = FCGX_GetParam("REQUEST_URI", request.envp);
        const char * request_method = FCGX_GetParam("REQUEST_METHOD", request.envp);
        const char * request_cookie = FCGX_GetParam("HTTP_COOKIE", request.envp);
        const char * request_params = FCGX_GetParam("QUERY_STRING", request.envp);
        char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
        size_t content_length = strtol(content_length_str, &content_length_str, 10);

        onyx::Request onyx_request;

        if (content_length > 0) {
            std::unique_ptr<char[] > buffer(new char[content_length + 1]);
            FCGX_GetStr(buffer.get(), content_length, request.in);
            onyx_request.setBody(buffer.get());
        }
        if (request_cookie)
            onyx_request.parse_cookie(request_cookie);
        std::string url = prepare_url(request_url);
        onyx_request.setUrl(url.c_str());
        onyx_request.setIp(request_ip_address);
        onyx_request.setMethod(request_method);
        onyx_request.setParams(request_params);
        try {
            std::string response_str = m_dispatcher->getResponseStr(onyx_request);
            FCGX_PutS(response_str.c_str(), request.out);
            LOGI << "Request " << onyx_request.getUrl() << " processed";
        } catch (onyx::Exception & ex) {
            LOGE << ex.what();
        }
        FCGX_Finish_r(&request);
    }
    return;
}

void onyx::Application::addRoute(const std::string& method, const std::string& regex, std::function<std::string(onyx::ONObject & object) > function) noexcept {
    onyx::Dispatcher::Route route;
    route.m_method = method;
    route.m_regex = regex;
    route.m_function = function;

    int err;
    err = regcomp(&route.m_preg, route.m_regex.c_str(), REG_EXTENDED);
    if (err != 0) {
        char buf[512];
        regerror(err, &route.m_preg, buf, sizeof (buf));
        LOGE << buf;
    } else {
        m_dispatcher->m_routes.push_back(route);
    }
}

void onyx::Application::setConfig(const std::string & path_config_file) {
    FILE* f = fopen(path_config_file.c_str(), "r");
    if (f == NULL) {
        printf("%s\n", "Error open configuration file. Application stopped");
        exit(EXIT_FAILURE);
    }
    std::string data;
    char buffer[1000];
    memset(buffer, '\0', sizeof (buffer));
    while (fgets(buffer, sizeof (buffer), f) != NULL) {
        data += std::string(buffer);
        memset(buffer, '\0', sizeof (buffer));
    }
    fclose(f);
    json settings;
    try {
        settings = json::parse(data);
        if (settings.find("unix_socket") != settings.end())
            m_socket_path = settings["unix_socket"].get<std::string>();
        if (settings.find("log") != settings.end())
            m_log_file_path = settings["log"].get<std::string>();
        if (settings.find("threads") != settings.end()) {
            m_thread_count = settings["threads"].get<int>();
            if (m_thread_count <= 0)
                m_thread_count = 1;
        }
    } catch (...) {
        printf("%s\n", "Invalid format configuration file. Application stopped");
        exit(EXIT_FAILURE);
    }
}

void onyx::Application::init() {
    setConfig("settings.json");
    if (m_log_file_path != "")
        m_file_log_appender = std::unique_ptr<plog::RollingFileAppender < plog::TxtFormatter >> (new plog::RollingFileAppender<plog::TxtFormatter>(m_log_file_path.c_str(), 10000000, 10));
    plog::init(plog::debug, m_file_log_appender.get()).addAppender(m_console_log_appender.get());
    if (m_socket_path == "") {
        LOGE << "undefined unix socket file. Application stoped";
        exit(EXIT_FAILURE);
    }
    FCGX_Init();
    m_socket_id = FCGX_OpenSocket(m_socket_path.c_str(), 32);
    char buf[1024];
    snprintf(buf, sizeof (buf), "chmod a+w %s", m_socket_path.c_str());
    int res = system(buf);
    if (res != 0) {
        LOGE << "Error change mode access to socket file";
        exit(EXIT_FAILURE);
    }
    if (m_socket_id < 0) {
        LOGE << "Error create socket";
        exit(EXIT_FAILURE);
    }
    LOGI << "ONYX started success";

}
std::string onyx::Application::prepare_url(const char * url) noexcept {
    std::string prepare(url);
    int ind = strlen(url) - 1;
    while (ind > 0) {
        if (url[ind] == '?')
            break;
        ind--;
    }
    if (ind > 0)
        return prepare.substr(0, ind);
    return prepare;
}