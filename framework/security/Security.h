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

namespace onyx {
    
    namespace security {
        onyx::session::User CallbackUserEmpty(const std::string & login, const std::string & password);
    }
    
    class Security {
       
        
    public:
        static std::string m_login_url;
        static std::string m_auth_url;
        static std::string m_redirect_url;
        
        static std::unique_ptr<SessionStorage> m_session_storage;
        static std::function<onyx::session::User(const std::string & login, const std::string & password) > m_callbackUser;
        
        static onyx::session::User getUser(const std::string & id){
            std::unique_ptr<Session> session = m_session_storage->fetchSession(id);
            return session->getUser();
        }
        
        static std::string auth(onyx::ONObject & obj);
        
    };
    
    
}

#endif

