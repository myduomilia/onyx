#ifndef SECURITY_H
#define SECURITY_H

#include <typeinfo>
#include "../cookie/Cookie.h"
#include <string>
#include "../exception/Exception.h"
#include <boost/uuid/uuid.hpp>            
#include "../common/json/json.hpp"

namespace onyx {
    class Security {
        
    public:
        static std::string m_login_url;
        static std::string m_auth_url;
        
    };
}

#endif

