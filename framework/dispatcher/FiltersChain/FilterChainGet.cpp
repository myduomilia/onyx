#include "FilterChainGet.h"

std::string FilterChainGet::handler(const onyx::Request & request, onyx::ONObject & obj, const onyx::Dispatcher::Route & route, std::shared_ptr<onyx::Session> session, const std::string & sessionid) {
    if (request.getMethod() == "GET") {
        std::string response = route.m_function(obj);
        onyx::Dispatcher * dispatcher = onyx::Dispatcher::getInstance();
        if (dispatcher->isCSRFTokenEnabled() && session)
            boost::replace_all(response, "%%csrf_token_value%%", session->getToken());
        LOGD << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Processed success";
        return response;
    }
    if (m_nextFilterChain != nullptr)
        return m_nextFilterChain->handler(request, obj, route, session, sessionid);
}