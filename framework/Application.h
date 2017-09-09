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

namespace onyx {

    class Application {
    private:
        
        static int m_socket_id;
        static std::vector<std::thread> m_threads;
        static std::mutex m_mutex_class;
        static plog::RollingFileAppender<plog::TxtFormatter> m_file_log_appender;
        static plog::ColorConsoleAppender<plog::TxtFormatter> m_console_log_appender;
        
        static void handler();
        
    public:
        
        static std::string m_socket_path;
        static size_t m_thread_count;
        
        static void setConfig(const std::string & config);
        static void run();
    };
}

#endif

