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
#include <exception>
#include <memory>
#include <mutex>
#include <thread>
#include <iostream>
#include <sstream>
#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <tuple>

namespace onyx {

    class Dispatcher {
        
    public:
        
        class Route {
        public:
            std::string m_method;
            std::string m_regex;
            regex_t m_preg;
            std::function<std::string(onyx::ONObject &) > m_function;
            std::vector<std::string> m_roles;
        };
        std::vector<Route> m_routes;
        std::string getResponseStr(const onyx::Request & request) const;
        
    };
}



#endif

