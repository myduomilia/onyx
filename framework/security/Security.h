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

namespace onyx {
    class Security {
    private:
        std::unique_ptr<Session> m_session;
        
    public:
        static std::string m_login_url;
        static std::string m_auth_url;
        
        Security(Session * session) : m_session(session) {
        }
        
        onyx::UserSession getUser(const std::string & id){
            return m_session->fetchUserBySessionId(id);
        }
        
    };
}

#endif

