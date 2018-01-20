#include "FilterChainCheckRole.h"

std::string FilterChainCheckRole::handler(const onyx::Request & request, onyx::ONObject & obj, const onyx::Dispatcher::Route & route, std::shared_ptr<onyx::Session> session,const std::string & sessionid) {
    onyx::Security * security = onyx::Security::getInstance();
    onyx::session::User user;
    if (session)
        user = session->getUser();
    // Если есть ограничение по роли, но нет sessionid в куках или сессия отсутствует (не выполнен вход) редирект на login страницу
    if (!route.m_roles.empty() && (sessionid == "" || !session))
        return onyx::RedirectResponse("Login", security->getLoginURL());
    // Если есть ограничение по роли и роль пользователя не подходит для данного route, то редирект 403
    if (!route.m_roles.empty() && std::find(route.m_roles.begin(), route.m_roles.end(), user.getRole()) == route.m_roles.end()) {
        return onyx::handler::_403();
    }
    if(m_nextFilterChain != nullptr)
        return m_nextFilterChain->handler(request, obj, route, session, sessionid);
}