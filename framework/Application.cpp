#include <mutex>

#include "Application.h"
#include "request/Request.h"
#include "response/JsonResponse.h"

int onyx::Application::m_socket_id;
std::vector<std::thread> onyx::Application::m_threads;
std::mutex onyx::Application::m_mutex_class;
plog::RollingFileAppender<plog::TxtFormatter> onyx::Application::m_file_log_appender("/var/log/onyx/onyx-app-example/log", 10000000, 10);
plog::ColorConsoleAppender<plog::TxtFormatter> onyx::Application::m_console_log_appender;

std::string onyx::Application::m_socket_path = "/tmp/onyx-app-example.sock";
size_t onyx::Application::m_thread_count = 8;

void onyx::Application::run() {
    plog::init(plog::debug, &m_file_log_appender).addAppender(&m_console_log_appender);
    FCGX_Init();
    m_socket_id = FCGX_OpenSocket(m_socket_path.c_str(), 32);
    char buf[80];
    snprintf(buf, sizeof (buf), "chmod a+w %s", m_socket_path.c_str());
    int res = system(buf);
    if(res != 0 ){
        LOGE << "Error change mode access to socket file";
        exit(EXIT_FAILURE);
    }
    if (m_socket_id < 0) {
        LOGE << "Error create socket";
        exit(EXIT_FAILURE);
    }
    LOGI << "ONYX started success"; 
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
        char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
        size_t content_length = strtol(content_length_str, &content_length_str, 10);
        
        onyx::Request onyx_request;
                
        if (content_length > 0) {
            std::unique_ptr<char[]> buffer(new char[content_length + 1]);
            FCGX_GetStr(buffer.get(), content_length, request.in);
            onyx_request.setBody(buffer.get());
        }
        if(request_cookie)
            onyx_request.parse_cookie(request_cookie);
        onyx_request.setUrl(request_url);
        onyx_request.setIp(request_ip_address);
        onyx_request.setMethod(request_method);
        onyx_request.parse_tokens_queries(FCGX_GetParam("QUERY_STRING", request.envp));
         
        onyx::JsonResponse response("{\"ups\" : 123}");        
        FCGX_PutS(response.getResponse().c_str(), request.out);
        FCGX_Finish_r(&request);
    }
    return;
}

void onyx::Application::setConfig(const std::string & config) {
    
}