#include "Dispatcher.h"

void onyx::Dispatcher::addRoute(const std::string& method, const std::string& regex, std::function<std::string() > function) noexcept {
    Route route;
    route.m_method = method;
    route.m_regex = regex;
    route.m_function = function;

    int err;
    err = regcomp(&route.m_preg, route.m_regex.c_str(), REG_EXTENDED);
    if (err != 0) {
        char buf[512];
        regerror(err, &route.m_preg, buf, sizeof (buf));
        LOGE << buf;
    } else
        m_routes.push_back(route);
}

std::string onyx::Dispatcher::getResponseStr(const onyx::Request & request) const {
    for (auto &route : m_routes) {
        int regerr;
        regmatch_t pm;
        if (request.getMethod() == route.m_method) {
            regerr = regexec(&route.m_preg, request.getUrl().c_str(), 0, &pm, 0);
            if (regerr == 0) {
                return route.m_function();
            } else if(regerr == 1){
                
            }
            else {
                char errbuf[1024];
                regerror(regerr, &route.m_preg, errbuf, sizeof (errbuf));
                LOGE << errbuf;
            }
        }
    }
    throw std::invalid_argument("Request " + request.getUrl() + " cannot process");
}