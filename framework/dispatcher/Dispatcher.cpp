#include <exception>
#include "Dispatcher.h"
#include "../handlers/404.h"
#include "../handlers/403.h"
#include "../security/Security.h"
#include "../response/RedirectResponse.h"
#include "../Application.h"
#include "FiltersChain/FilterChainCheckRole.h"
#include "FiltersChain/FilterChainPost.h"
#include "FiltersChain/FilterChainGet.h"

onyx::Dispatcher * onyx::Dispatcher::m_instance = nullptr;

void onyx::Dispatcher::addRoute(Route route) {
    m_routes.push_back(route);
}

onyx::Dispatcher::Dispatcher() {
    m_security = onyx::Security::getInstance();
}

std::string onyx::Dispatcher::getResponseStr(const onyx::Request & request) const {
    onyx::TokenCollection token(request.getUrl());
    onyx::ParamCollection params(request.getParams());
    onyx::CookieCollection cookies(request.getCookies());
    onyx::ONObject obj(token, params, cookies, request.getBody());

    // Получаем сессию
    std::string sessionid;
    if (cookies.has("sessionid"))
        sessionid = cookies["sessionid"];
    onyx::Security * security = onyx::Security::getInstance();
    std::shared_ptr<onyx::Session> session(security->getSessionStorage()->fetchSession(sessionid));

    // Инициализируем цепочку обработчиков запроса
    FilterChainCheckRole filterChainCheckRole;
    FilterChainPost * filterChainPost = new FilterChainPost();
    FilterChainGet * filterChainGet = new FilterChainGet();
    
    filterChainCheckRole.setNextHandler(filterChainPost);
    filterChainPost->setNextHandler(filterChainGet);

    for (auto & route : m_routes) {
        regmatch_t pm;
        if (request.getMethod() == route.m_method) {
            if (regexec(&route.m_preg, request.getUrl().c_str(), 0, &pm, 0) == 0) {
                return filterChainCheckRole.handler(request, obj, route, session, sessionid);
            }
        }
    }
    LOGE << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Can't proccess";
    return onyx::handler::_404();
}