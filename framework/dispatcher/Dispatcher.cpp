#include <exception>
#include "Dispatcher.h"
#include "../handlers/404.h"
#include "../handlers/403.h"
#include "../security/Security.h"

std::string onyx::Dispatcher::getResponseStr(const onyx::Request & request) const {
    for (auto & route : m_routes) {
        int regerr;
        regmatch_t pm;
        if (request.getMethod() == route.m_method) {
            regerr = regexec(&route.m_preg, request.getUrl().c_str(), 0, &pm, 0);
            if (regerr == 0) {
                onyx::TokenCollection token(request.getUrl());
                onyx::ParamCollection params(request.getParams());
                onyx::CookieCollection cookies(request.getCookies());
                onyx::ONObject obj(token, params, cookies, request.getBody());
                if(std::find(route.m_roles.begin(), route.m_roles.end(), "GUEST") != route.m_roles.end()){
                    std::string response = route.m_function(obj);
                    return response;
                }else{
                    return onyx::handler::_403();
                }
                
//                try {
//                    cookies["sessionid"];
//                    return response;
//                } catch (onyx::Exception &ex) {
//                    std::stringstream stream;
//                    boost::uuids::uuid uuid = boost::uuids::random_generator()();
//                    time_t expires = time(NULL) + 60 * 60 * 24 * 30;
//                    char buff[40];
//                    strftime(buff, sizeof(buff), "%d-%b-%Y %H:%M:%S", localtime(&expires));
//                    stream << "Set-Cookie: sessionid=" << boost::lexical_cast<std::string>(uuid) << "; expires=" << buff << ";\r\n " << response;
//                    return stream.str();
//                }
            } else if (regerr == 1) {
                // todo:
            } else {
                char errbuf[1024];
                regerror(regerr, &route.m_preg, errbuf, sizeof (errbuf));
                LOGE << errbuf;
            }
        }
    }
    if (request.getMethod() == "GET")
        return onyx::handler::_404();
    throw onyx::Exception("Request " + request.getUrl() + " can't process");
}