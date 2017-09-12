#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <regex.h>
#include <functional>
#include <string>
#include "../request/Request.h"
#include "../common/plog/Log.h"
#include "../response/BaseResponse.h"

namespace onyx {
    class Dispatcher {
    private:
        class Route {
        public:
            std::string m_method;
            std::string m_regex;
            regex_t m_preg;
            std::function<std::string()> m_function;
        };
        std::vector<Route> m_routes;
    public:
        void addRoute(const std::string &method, const std::string &regex, std::function<std::string()>) noexcept;
        std::string getResponseStr(const onyx::Request & request) const ;
    };
}



#endif

