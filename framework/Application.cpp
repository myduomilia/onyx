#include <mutex>

#include "Application.h"
#include "request/Request.h"
#include "response/JsonResponse.h"
#include "dispatcher/Dispatcher.h"

onyx::Application::Application() {
    m_file_log_appender = nullptr;
    m_console_log_appender = new plog::ColorConsoleAppender<plog::TxtFormatter>;
    m_dispatcher = onyx::Dispatcher::getInstance();
    init();
}

void onyx::Application::run() {
    onyx::Security * security = m_dispatcher->getSecurity();

    if (security->getCallbackUser() == nullptr || security->getSessionStorage() == nullptr) {
        std::cerr << "CallbackRole function or session storage are undefined. Application stoped" << std::endl;
        exit(EXIT_FAILURE);
    }

    LOGI << "ONYX started success";

    for (size_t i = 0; i < m_thread_count; i++)
        m_threads.push_back(std::thread(&Application::handler, this));

    for (auto& thread : m_threads)
        thread.join();

}

void onyx::Application::handler() {
    int rc;
    FCGX_Request request;
    if (FCGX_InitRequest(&request, m_socket_id, 0) != 0)
        return;
    for (;;) {
        m_mutex_instance.lock();
        rc = FCGX_Accept_r(&request);
        m_mutex_instance.unlock();

        if (rc < 0)
            continue;

        const char * server_name = FCGX_GetParam("SERVER_NAME", request.envp);
        const char * request_ip_address = FCGX_GetParam("REMOTE_ADDR", request.envp);
        const char * request_url = FCGX_GetParam("REQUEST_URI", request.envp);
        const char * request_method = FCGX_GetParam("REQUEST_METHOD", request.envp);
        const char * request_content_type = FCGX_GetParam("CONTENT_TYPE", request.envp);
        const char * request_cookie = FCGX_GetParam("HTTP_COOKIE", request.envp);
        const char * request_params = FCGX_GetParam("QUERY_STRING", request.envp);
        char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
        size_t content_length = strtol(content_length_str, &content_length_str, 10);

        onyx::Request onyx_request;

        if (content_length > 0) {
            std::unique_ptr<char[] > buffer(new char[content_length + 1]);
            memset(buffer.get(), '\0', content_length + 1);
            FCGX_GetStr(buffer.get(), content_length, request.in);
            onyx_request.setBody(buffer.get());
        }
        if (request_cookie)
            onyx_request.setCookies(request_cookie);
        std::string url = fetchEmptyURL(request_url);
        onyx_request.setUrl(url.c_str());
        onyx_request.setIp(request_ip_address);
        onyx_request.setMethod(request_method);
        onyx_request.setParams(request_params);
        onyx_request.setContentType(request_content_type);
        std::string response_str = m_dispatcher->getResponseStr(onyx_request);
        FCGX_PutStr(response_str.c_str(), response_str.size(), request.out);
        FCGX_Finish_r(&request);
    }
    return;
}

void onyx::Application::addRoute(const std::string& method, const std::string& regex, std::function<std::string(onyx::ONObject & object) > function, std::vector<std::string> roles) noexcept {
    onyx::Dispatcher::Route route;
    route.m_method = method;
    route.m_regex = regex;
    route.m_function = function;
    route.m_roles = roles;

    int err;
    err = regcomp(&route.m_preg, route.m_regex.c_str(), REG_EXTENDED);
    if (err != 0) {
        char buf[1024];
        regerror(err, &route.m_preg, buf, sizeof (buf));
        LOGE << buf;
    } else {
        m_dispatcher->addRoute(route);
    }
}

void onyx::Application::setAppSettings(const std::string & path_config_file) {
    FILE* f = fopen(path_config_file.c_str(), "r");
    if (f == NULL) {
        std::cerr << "Can't open configuration file. Application stopped" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string data;
    char buffer[1024 * 16];
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
        if (settings.find("domain_unix_socket") != settings.end())
            m_domain_socket = settings["domain_unix_socket"].get<std::string>();
        if (settings.find("log") != settings.end())
            m_log_file_path = settings["log"].get<std::string>();
        if (settings.find("threads") != settings.end())
            m_thread_count = settings["threads"].get<int>();
        m_mode_debug = false;
        if (settings.find("debug") != settings.end())
            m_mode_debug = settings["debug"].get<bool>();
    } catch (...) {
        std::cerr << "Invalid format of configuration file. Application stopped" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void onyx::Application::init() {

    onyx::Security * security = m_dispatcher->getSecurity();

    setAppSettings("settings.json");
    if (m_log_file_path != "")
        m_file_log_appender = new plog::RollingFileAppender<plog::TxtFormatter>(m_log_file_path.c_str(), 10000000, 10);
    if (m_mode_debug)
        plog::init(plog::debug, m_file_log_appender).addAppender(m_console_log_appender);
    else
        plog::init(plog::info, m_file_log_appender).addAppender(m_console_log_appender);
    if (m_socket_path == "" && m_domain_socket == "") {
        std::cerr << "Unix socket file and Domain Socket are undefined. Application stoped" << std::endl;
        exit(EXIT_FAILURE);
    }
    FCGX_Init();
    if (m_socket_path != "") {
        m_socket_id = FCGX_OpenSocket(m_socket_path.c_str(), 512);
        char buf[1024];
        snprintf(buf, sizeof (buf), "chmod a+w %s", m_socket_path.c_str());
        int res = system(buf);
        if (res != 0) {
            std::cerr << "Can't change mode access of socket file. Application stoped" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (m_socket_id < 0) {
            std::cerr << "Can't create socket. Application stoped" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    if (m_domain_socket != "") {
        m_socket_id = FCGX_OpenSocket(m_domain_socket.c_str(), 512);
        if (m_socket_id < 0) {
            std::cerr << "Can't create socket. Application stoped" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::string regex = "^" + security->getAuthURL() + "$";
    addRoute("POST", regex, security->fetchAuthHandler());
}

std::string onyx::Application::fetchEmptyURL(const char * url) noexcept {
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