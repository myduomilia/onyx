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
    
    class Dispatcher {
    public:
        
        struct Route {
            std::string m_method;
            std::string m_regex;
            regex_t m_preg;
            std::function<std::string(onyx::ONObject &) > m_function;
            std::vector<std::string> m_roles;
        };

        static Dispatcher * getInstance() noexcept {
            if (m_instance == nullptr)
                m_instance = new Dispatcher;
            return m_instance;
        }
        
        std::string getResponseStr(const onyx::Request & request) const;
        
        void addRoute(Route route);
        
        onyx::Security* getSecurity() const {
            return m_security;
        }
        
        void setSecurity(onyx::Security* security) {
            m_security = security;
        }
        
        bool isCSRFTokenEnabled() const {
            return m_csrf_token_enabled;
        }
        
        std::string getCSRFTokenSecret() const {
            return m_csrf_token_secret;
        }
        
        void setCSRFTokenEnabled(bool csrf_token_enabled) {
            m_csrf_token_enabled = csrf_token_enabled;
        }

    private:
        
        bool m_csrf_token_enabled;
        std::string m_csrf_token_secret;

        std::vector<Route> m_routes;

        static Dispatcher * m_instance;
        onyx::Security * m_security;

        Dispatcher();
        
    };
}



#endif

