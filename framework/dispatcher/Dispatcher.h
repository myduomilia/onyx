#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <regex.h>
#include <functional>
#include <string>
#include "../request/Request.h"
#include "../common/plog/Log.h"
#include "../response/BaseResponse.h"
#include "../exception/Exception.h"
#include "../object/ONObject.h"
#include "../cookie/Cookie.h"
#include "../security/Security.h"
#include <exception>
#include <memory>
#include <mutex>
#include <thread>
#include <iostream>
#include "../common/mustache/mustache.h"
#include <boost/algorithm/string/replace.hpp>

#include <tuple>

namespace onyx {
    
    class Application;
    
    /**
        \brief Главный обработчик запросов в приложении
        \author myduomilia
        \date Декабрь 2017
        \details Класс реализован в виде паттерна Singleton, инициализация происходит до первого вызова объекта класса
     */
    
    class Dispatcher {
        
    public:
        
        /**
        \brief Внутренняя структура представляющая route 
        \author myduomilia
        \date Декабрь 2017
         */
        struct Route {
            std::string m_method; ///< Метод запроса на котором работает данный route (POST, GET и т.д.)
            std::string m_regex; ///< Регулярное выражение, которому должен соответствовать запрос
            regex_t m_preg; ///< Скомпилированное регулярное выражение
            std::function<std::string(onyx::ONObject &) > m_function; ///< Функтор, который вызывается при обработке данного route
            std::vector<std::string> m_roles; ///< Список ролей, которым разрешен вызов данного route
        };

        /**
            Создает объект Dispatcher
            \return Возвращает указатель на объект Dispatcher
            \warning Данный метод потоко не безопасный
         */
        static Dispatcher * getInstance() noexcept {
            if (m_instance == nullptr)
                m_instance = new Dispatcher;
            return m_instance;
        }
        
        /**
            \param[in] request внутренний объект onyx::Request - инкапсулирующий в себе параметры запроса FastCGI
            \return возвращает строкое представление ответа сервера, все объекты возвращаемые сервером, могут сериализоваться в строковое представление
            \details Dispatcher последовательно перебирает маршруты и сравнивает по регулярному выражению, учитывая также метод запроса.
             Проверяется сравнение разрешенной роли, если роль есть у route и роль пользователя.
             При включенном csrf токен, проверяется его наличие в каждом POST запросе, а также происходит заполнение токенов при отдаче сервером
        */
        std::string getResponseStr(const onyx::Request & request) const;
        
        /**
            Добавляет route в список
            \param[in] route маршрут запроса
        */
        void addRoute(Route route);
        
        /**
            Получить указатель на объект onyx::Security
        */
        onyx::Security* getSecurity() const {
            return m_security;
        }
        
        /**
            Установить onyx::Security, для обработки в onyx::Dispatcher
            \param[in] security указатель на onyx::Security
        */
        void setSecurity(onyx::Security* security) {
            m_security = security;
        }
        
        /**
            Функция проверки установлен режим csrf токена
        */
        bool isCSRFTokenEnabled() const {
            return m_csrf_token_enabled;
        }
        
        /**
            Функция возвращает секретный ключ
        */
        std::string getCSRFTokenSecret() const {
            return m_csrf_token_secret;
        }
        
        void setCSRFTokenEnabled(bool csrf_token_enabled) {
            m_csrf_token_enabled = csrf_token_enabled;
        }

    private:
        
        bool m_csrf_token_enabled; ///< Флаг включения проверки csrf токена
        std::string m_csrf_token_secret; ///< Секретный ключ приложения

        std::vector<Route> m_routes; ///< Список маршрутов

        static Dispatcher * m_instance; ///< Указатель на объект Dispatcher
        onyx::Security * m_security;

        Dispatcher();
        
    };
}



#endif

