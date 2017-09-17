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
#include <exception>
#include <memory>



namespace onyx {

    class Dispatcher {
    private:

        class Route {
        public:
            std::string m_method;
            std::string m_regex;
            regex_t m_preg;
            std::function<std::string(onyx::ONObject) > m_function;
        };

        std::vector<Route> m_routes;
    public:
        void addRoute(const std::string &method, const std::string &regex, std::function<std::string(onyx::ONObject)>) noexcept;
        std::string getResponseStr(const onyx::Request & request) const;
    };
}



#endif

