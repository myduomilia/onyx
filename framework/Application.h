#ifndef APPLICATION_H
#define APPLICATION_H

#include <fcgi_config.h>
#include <fcgiapp.h>
#include <string>
#include <string.h>
#include <thread>
#include <vector>
#include <iostream>
#include <regex.h>
#include <functional>
#include <algorithm>

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mutex>
#include <memory>
#include <tuple>

#include "common/plog/Log.h"
#include "common/plog/Appenders/ColorConsoleAppender.h"
#include "common/json/json.hpp"
#include "dispatcher/Dispatcher.h"

#include "security/Security.h"

#include "handlers/404.h"

using json = nlohmann::json;

namespace onyx {
    
    class Application {
    private:
        
        std::string m_socket_path;
        std::string m_domain_socket;
        std::string m_log_file_path;
        size_t m_thread_count;
        bool m_mode_debug;
        
        int m_socket_id; 
        
        Dispatcher * m_dispatcher;
        std::vector<std::thread> m_threads;
        std::mutex m_mutex_instance; 
        plog::RollingFileAppender<plog::TxtFormatter> * m_file_log_appender; 
        plog::ColorConsoleAppender<plog::TxtFormatter> * m_console_log_appender;
        
        /*
            interaction function with interface FastCGI
         */
        void handler();
        void setAppSettings(const std::string & path_config_file);
        void init();
        std::string fetchEmptyURL(const char * url) noexcept;

    public:
        
        Application(); 
        
        /**
            event loop
        */
        void run();
        
        /**
            add route
        */
        void addRoute(const std::string & method, const std::string & regex, std::function<std::string(onyx::ONObject &)> function, std::vector<std::string> roles = {}) noexcept;
        
        /*
            activate check csrf token
        */
        void setCSRFTokenEnabled(bool csrf_token_enabled) {
            m_dispatcher->setCSRFTokenEnabled(csrf_token_enabled);
        }
    };
}

#endif

