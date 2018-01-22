#ifndef SECURITY_H
#define SECURITY_H

#include <typeinfo>
#include "../cookie/Cookie.h"
#include <string>
#include "../exception/Exception.h"
#include <boost/uuid/uuid.hpp>            
#include "../common/json/json.hpp"
#include "../session/Session.h"
#include <memory>
#include <sstream>
#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "../response/RedirectResponse.h"
#include "../object/ONObject.h"
#include "../session/Session.h"
#include "../validate/ValidateXSS.h"

namespace onyx {
    
    /**
        \brief Класс отвечающий за авторизацию, безопасность и разграничение прав доступа в приложении
        \author myduomilia
        \date Январь 2018
        \details Представлен в системе уникальным объектом, реализованный паттерном Singleton. Объект обязательно будет создан onyx::Dispatcher приложения
     */
    
    class Security {
    private:
        std::string m_login_url; ///< URL на который происходит редирект в случае отсутсвия прав доступа GET запрос
        std::string m_auth_url; ///< URL по которому происходит авторизация в приложении
        std::string m_redirect_url; ///< URL на который происходит редирект в случае удачной авторизации в приложении
        
        ISessionStorage * m_session_storage; ///< Указатель на объект отвечающий за действия с сессиями в приложении
        
        static Security * m_instance; ///< Указатель на объект Security
        
        Security();
        
        std::function<onyx::session::User * (const std::string & login, const std::string & password) > m_callbackUser;
        
    public:
        
        /**
            Создает объект Security
            \return Возвращает указатель на объект Security
            \warning Данный метод потоко не безопасный
         */
        static Security * getInstance() noexcept {
            if (m_instance == nullptr)
                m_instance = new Security;
            return m_instance;
        }
        
        /**
            Возвращает фцнкцию, которая производит авторизацию пользователя и добавляет данные в куки браузера
            \return Возвращает указатель на функтор
         */
        const std::function<std::string(onyx::ONObject &)> fetchAuthHandler();
        
        /**
            Функция возвращает URL авторизации
        */
        std::string getAuthURL() const {
            return m_auth_url;
        }
        
        /**
            Функция возвращает URL страницы авторизации
        */
        std::string getLoginURL() const {
            return m_login_url;
        }

        /**
            Функция возвращает URL редиректа, после удачной авторизации
        */
        std::string getRedirectURL() const {
            return m_redirect_url;
        }

        /**
            Функция устанавливает URL авторизации
        */
        void setAuthURL(std::string auth_url) {
            m_auth_url = auth_url;
        }

        /**
            Функция устанавливает URL страницы авторизации
        */
        void setLoginURL(std::string login_url) {
            m_login_url = login_url;
        }
        
        /**
            Функция устанавливает URL редиректа, после удачной авторизации в приложении
        */
        void setRedirectURL(std::string redirect_url) {
            m_redirect_url = redirect_url;
        }

        /**
            Функция устанавливает объект SessionStorage
        */
        void setSessionStorage(ISessionStorage* session_storage) {
            m_session_storage = session_storage;
        }
        
        /**
            Функция возвращает объект SessionStorage
            \return указатель на объект ISessionStorage*
        */
        ISessionStorage* getSessionStorage() const {
            return m_session_storage;
        }
        
        /**
            Функция устанавливает функтор необходимый для авторизации пользователя. 
            \param[in] callback - функтор.
            \warning Данный функтор обязан вернуть указатель на пользователя, в случае, если пользователь не прошел авторизацию, вернуть nullptr. Входными параметрами функтора является логин и пароль пользователя. Данный функтор обязательно должен быть установлен в приложении, если даже не ведется авторизация пользователя в системе
        */
        void setCallbackUser(std::function<onyx::session::User*(const std::string&, const std::string&)> callback) {
            m_callbackUser = callback;
        }
        
        /**
            Функция возвращает функтор необходимый для авторизации пользователя. 
            \return функтор std::function<onyx::session::User * (const std::string&, const std::string&)>
            \warning Данный функтор обязан вернуть указатель на пользователя, в случае, если пользователь не прошел авторизацию, вернуть nullptr. Входными параметрами функтора является логин и пароль пользователя. Данный функтор обязательно должен быть установлен в приложении, если даже не ведется авторизация пользователя в системе
        */
        std::function<onyx::session::User * (const std::string&, const std::string&)> getCallbackUser() {
            return m_callbackUser;
        }

        
    };
    
}

#endif

