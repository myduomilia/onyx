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

        static int m_socket_id;
        static std::vector<std::thread> m_threads;

        static std::unique_ptr<Dispatcher> m_dispatcher;

        static std::mutex m_mutex_class;
        static std::unique_ptr<plog::RollingFileAppender<plog::TxtFormatter>> m_file_log_appender;
        static std::unique_ptr<plog::ColorConsoleAppender<plog::TxtFormatter>> m_console_log_appender;
        
        static void handler();

        static void setConfig(const std::string & path_config_file);
        
        static std::string prepare_url(const char * url) noexcept;

    public:

        static std::string m_socket_path;
        static std::string m_log_file_path;
        static size_t m_thread_count;
        static bool m_csrf_token_enabled;
        static std::string m_csrf_token_secret;

        static void init();
        static void run();

        static void addRoute(const std::string &method, const std::string &regex, std::function<std::string(onyx::ONObject &)>, std::vector<std::string> roles = {}) noexcept;
        
        static void setCsrfTokenEnabled(bool csrf_token_enabled) {
            Application::m_csrf_token_enabled = csrf_token_enabled;
        }

        static void setCsrfTokenSecret(const std::string csrf_token_secret) {
            m_csrf_token_secret = csrf_token_secret;
        }

    };
}

#endif

