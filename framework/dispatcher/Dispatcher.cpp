#include <exception>
#include "Dispatcher.h"
#include "../handlers/404.h"

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
                
                return route.m_function(obj);
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