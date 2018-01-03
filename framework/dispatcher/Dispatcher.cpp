#include <exception>
#include "Dispatcher.h"
#include "../handlers/404.h"
#include "../handlers/403.h"
#include "../security/Security.h"
#include "../response/RedirectResponse.h"
#include "../Application.h"

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
    std::unique_ptr<onyx::Session> session(security->getSessionStorage()->fetchSession(sessionid));
    onyx::session::User user;
    if (session)
        user = session->getUser();
    
    for (auto & route : m_routes) {
        regmatch_t pm;
        if (request.getMethod() == route.m_method) {
            if (regexec(&route.m_preg, request.getUrl().c_str(), 0, &pm, 0) == 0) {
                
                // Если есть ограничение по роли, но нет sessionid в куках или сессия отсутствует (не выполнен вход) редирект на login страницу
                if (!route.m_roles.empty() && (sessionid == "" || !session))
                    return onyx::RedirectResponse("Login", security->getLoginURL());
                // Если есть ограничение по роли и роль пользователя не подходит для данного route, то редирект 403
                if (!route.m_roles.empty() && std::find(route.m_roles.begin(), route.m_roles.end(), user.getRole()) == route.m_roles.end()) {
                    return onyx::handler::_403();
                }
                // Заполняем все токены (csrf_token_value), должна быть найдена сессия
                std::string response = route.m_function(obj);
                if (m_csrf_token_enabled && session) 
                    boost::replace_all(response, "%%csrf_token_value%%", session->getToken());
                if (request.getMethod() == "POST") {
                    if (m_csrf_token_enabled && session) {
                        std::map<std::string, std::string> form_params = onyx::Request::parse_form_params(obj.getBody());
                        if (form_params.find("csrf_token") == form_params.end())
                            return onyx::handler::_403();
                        std::string csrf_token = form_params["csrf_token"];
                        if (csrf_token != session->getToken())
                            return onyx::handler::_403();
                    }
                    LOGD << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Processed success";
                    return response;
                } else if (request.getMethod() == "GET") {
                    LOGD << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Processed success";
                    return response;
                } else {
                    //  todo: вернуть ошибку сервера UnSupportedMethod
                }
            }
        }
    }
    LOGE << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Can't proccess";
    return onyx::handler::_404();
}