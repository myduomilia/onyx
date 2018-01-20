#include "FilterChainPost.h"

std::string FilterChainPost::handler(const onyx::Request & request, onyx::ONObject & obj, const onyx::Dispatcher::Route & route, std::shared_ptr<onyx::Session> session, const std::string & sessionid) {
    if (request.getMethod() == "POST") {
        std::string response = route.m_function(obj);
        onyx::Dispatcher * dispatcher = onyx::Dispatcher::getInstance();
        if (dispatcher->isCSRFTokenEnabled() && session) {
            boost::replace_all(response, "%%csrf_token_value%%", session->getToken());
            std::map<std::string, std::string> form_params = onyx::Request::parse_form_params(obj.getBody());
            if (form_params.find("csrf_token") == form_params.end())
                return onyx::handler::_403();
            std::string csrf_token = form_params["csrf_token"];
            if (csrf_token != session->getToken())
                return onyx::handler::_403();
        }
        LOGD << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Processed success";
        return response;
    }
    if (m_nextFilterChain != nullptr)
        return m_nextFilterChain->handler(request, obj, route, session, sessionid);
}