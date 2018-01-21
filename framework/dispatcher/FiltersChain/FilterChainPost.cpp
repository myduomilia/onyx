#include "FilterChainPost.h"

std::string FilterChainPost::handler(const onyx::Request & request, onyx::ONObject & obj, const onyx::Dispatcher::Route & route, std::shared_ptr<onyx::Session> session, const std::string & sessionid) {
    if (request.getMethod() == "POST") {
        onyx::Dispatcher * dispatcher = onyx::Dispatcher::getInstance();
        std::string response;
        if (dispatcher->isCSRFTokenEnabled() && session) {
            std::map<std::string, std::string> form_params = onyx::Request::parse_form_params(obj.getBody());
            if (form_params.find("csrf_token") == form_params.end()){
                LOGD << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Processed forbidden";
                return onyx::handler::_403();
            }
            std::string csrf_token = form_params["csrf_token"];
            if (csrf_token != session->getToken()){
                LOGD << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Processed forbidden";
                return onyx::handler::_403();
            }
            response = route.m_function(obj);
            boost::replace_all(response, "%%csrf_token_value%%", session->getToken());
        } else
            response = route.m_function(obj);
        LOGD << "Request url " << request.getUrl() << ". Method " << request.getMethod() << ". Processed success";
        return response;
    }
    if (m_nextFilterChain != nullptr)
        return m_nextFilterChain->handler(request, obj, route, session, sessionid);
}