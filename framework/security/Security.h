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
    
   
    class Security {
    private:
        std::string m_login_url;
        std::string m_auth_url; 
        std::string m_redirect_url;
        
        ISessionStorage * m_session_storage;
        
        static Security * m_instance;
        
        Security();
        
        std::function<onyx::session::User * (const std::string & login, const std::string & password) > m_callbackUser;
        
    public:
        
        static Security * getInstance() noexcept {
            if (m_instance == nullptr)
                m_instance = new Security;
            return m_instance;
        }
        
        const std::function<std::string(onyx::ONObject &)> fetchAuthHandler();
        
        std::string getAuthURL() const {
            return m_auth_url;
        }
        
        std::string getLoginURL() const {
            return m_login_url;
        }

        std::string getRedirectURL() const {
            return m_redirect_url;
        }

        void setAuthURL(std::string auth_url) {
            m_auth_url = auth_url;
        }
        
        void setLoginURL(std::string login_url) {
            m_login_url = login_url;
        }
        
        void setRedirectURL(std::string redirect_url) {
            m_redirect_url = redirect_url;
        }

        void setSessionStorage(ISessionStorage* session_storage) {
            m_session_storage = session_storage;
        }
        
        ISessionStorage* getSessionStorage() const {
            return m_session_storage;
        }
        
        void setCallbackUser(std::function<onyx::session::User*(const std::string&, const std::string&)> callback) {
            m_callbackUser = callback;
        }
        
        std::function<onyx::session::User * (const std::string&, const std::string&)> getCallbackUser() {
            return m_callbackUser;
        }

        
    };
    
}

#endif

