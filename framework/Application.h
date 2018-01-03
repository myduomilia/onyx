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
    
    /**
        \brief Главный класс фреймворка
        \author myduomilia
        \date Декабрь 2017
     */
    
    class Application {
    private:
        
        std::string m_socket_path; ///< Путь к файлу сокета
        std::string m_log_file_path; ///< Путь к файлу логов
        size_t m_thread_count; ///< Количество потоков приложения
        bool m_mode_debug; ///< Флаг режима дебаг
        
        int m_socket_id; ///< Идентификатор серверного сокета
        
        Dispatcher * m_dispatcher; ///< Главный диспатчер
        std::vector<std::thread> m_threads; ///< Список объектов потоков приложения
        std::mutex m_mutex_instance; ///< Мьютех
        plog::RollingFileAppender<plog::TxtFormatter> * m_file_log_appender; ///< Объект логирования в файл
        plog::ColorConsoleAppender<plog::TxtFormatter> * m_console_log_appender; ///< Объект логирования на консоль
        
        /**
            Функция для работы с интерфейсом FastCGI
         */
        void handler();
        /**
            Функция чтения конфигурации сервера
            \param[in] path_config_file путь к конфигурационному файлу, данный путь на данный момент жестко регламитирован settings.json ы корне приложения
        */
        void setAppSettings(const std::string & path_config_file);
        /**
            Функция инициализация фреймворка и интерфейса FastCGI
        */
        void init();
        /**
            Функция получает чисты URL без параметров запроса
            \param[in] url полный URL запроса
        */
        std::string fetchEmptyURL(const char * url) noexcept;

    public:
        
        Application(); 
        
        /**
            Функция запускает бесконечный цикл работы фреймворка
        */
        void run();
        /**
            Функция добавления нового маршрута
            \param[in] method метод запроса (GET, POST)
            \param[in] regex регулярное выражение сравнения
            \param[in] function статическая функция callback для данного запроса
            \param[in] roles список ролей, которым доступен данный запрос
        */
        void addRoute(const std::string & method, const std::string & regex, std::function<std::string(onyx::ONObject &)> function, std::vector<std::string> roles = {}) noexcept;
        /**
            Функция установки csrf токена
            \param[in] csrf_token_secret
        */
        void setCSRFTokenEnabled(bool csrf_token_enabled) {
            m_dispatcher->setCSRFTokenEnabled(csrf_token_enabled);
        }
    };
}

#endif

