#include <exception>
#include "Dispatcher.h"
#include "../handlers/404.h"
#include "../handlers/403.h"
#include "../security/Security.h"
#include "../response/RedirectResponse.h"
#include "../Application.h"

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
                if (!route.m_roles.size()) {
                    std::string response = route.m_function(obj);
                    if (cookies.has("sessionid") && onyx::Application::m_csrf_token_enabled) {
                        onyx::session::User user = onyx::Security::getUser(cookies["sessionid"]);
                        unsigned char* digest;
                        std::string user_id = user.getId();
                        if (user_id == "") 
                            return response;
                        digest = HMAC(EVP_sha1(), onyx::Application::m_csrf_token_secret.data(), onyx::Application::m_csrf_token_secret.size(), (unsigned char*) user_id.data(), user_id.size(), NULL, NULL);
                        char mdString[20];
                        for (size_t i = 0; i < 20; i++)
                            sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);
                        boost::replace_all(response, "%%csrf_token_value%%", mdString);
                        return response;
                    }
                    return response;
                } else {
                    if (!cookies.has("sessionid")) {
                        return onyx::RedirectResponse("Login", onyx::Security::m_login_url);
                    } else {
                        onyx::session::User user = onyx::Security::getUser(cookies["sessionid"]);
                        if (onyx::Application::m_csrf_token_enabled && request.getMethod() == "POST") {
                            std::map<std::string, std::string> form_params = onyx::Request::parse_form_params(obj.getBody());
                            if (form_params.find("csrf_token") == form_params.end())
                                return onyx::handler::_403();
                            std::string csrf_token = form_params["csrf_token"];
                            unsigned char* digest;
                            std::string user_id = user.getId();
                            digest = HMAC(EVP_sha1(), onyx::Application::m_csrf_token_secret.data(), onyx::Application::m_csrf_token_secret.size(), (unsigned char*) user_id.data(), user_id.size(), NULL, NULL);
                            char mdString[20];
                            for (size_t i = 0; i < 20; i++)
                                sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);
                            if (strcmp(csrf_token.data(), mdString) != 0)
                                return onyx::handler::_403();
                        }
                        if (std::find(route.m_roles.begin(), route.m_roles.end(), user.getRole()) != route.m_roles.end()) {
                            std::string response = route.m_function(obj);
                            if (onyx::Application::m_csrf_token_enabled) {
                                unsigned char* digest;
                                std::string user_id = user.getId();
                                digest = HMAC(EVP_sha1(), onyx::Application::m_csrf_token_secret.data(), onyx::Application::m_csrf_token_secret.size(), (unsigned char*) user_id.data(), user_id.size(), NULL, NULL);
                                char mdString[20];
                                for (size_t i = 0; i < 20; i++)
                                    sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);
                                boost::replace_all(response, "%%csrf_token_value%%", mdString);
                                return response;
                            }
                            return response;
                        }
                        return onyx::handler::_403();
                    }
                }
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