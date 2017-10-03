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
    class Security {
       
        
    public:
        static std::string m_login_url;
        static std::string m_auth_url;
        static std::string m_redirect_url;
        
        static std::unique_ptr<Session> m_session;
        static std::function<std::string(const std::string & login, const std::string & password) > m_callbackRole;
        
        static onyx::UserSession getUser(const std::string & id){
            return m_session->fetchUserBySessionId(id);
        }
        
        static std::string auth(onyx::ONObject & obj);
        
    };
}

#endif

