#ifndef APPLICATION_H
#define APPLICATION_H

#include <fcgi_config.h>
#include <fcgiapp.h>
#include <string>
#include <string.h>
#include <thread>
#include <vector>
#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mutex>

#include "common/plog/Log.h"
#include "common/plog/Appenders/ColorConsoleAppender.h"
#include "common/json/json.hpp"
#include "dispatcher/Dispatcher.h"

using json = nlohmann::json;

namespace onyx {

    class Application {
    private:
        
        static int m_socket_id;
        static std::vector<std::thread> m_threads;
        
        static Dispatcher m_dispatcher;
        
        static std::mutex m_mutex_class;
        static std::unique_ptr<plog::RollingFileAppender<plog::TxtFormatter>> m_file_log_appender;
        static plog::ColorConsoleAppender<plog::TxtFormatter> m_console_log_appender;
        
        static void handler();
        
        static void setConfig(const std::string & path_config_file);
        
    public:
        
        static std::string m_socket_path;
        static std::string m_log_file_path;
        static size_t m_thread_count;
        
        static void run();
        static Dispatcher & getDispatcher() {
            return m_dispatcher;
        }

 
    };
}

#endif

