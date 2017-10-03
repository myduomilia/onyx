#include "Security.h"



std::string onyx::Security::m_login_url = "/login";
std::string onyx::Security::m_auth_url = "/auth";
std::string onyx::Security::m_redirect_url = "/";

std::unique_ptr<onyx::Session> onyx::Security::m_session(nullptr);
std::function<std::string(const std::string & login, const std::string & password)> onyx::Security::m_callbackRole = nullptr;

std::string onyx::Security::auth(onyx::ONObject& obj) {
    std::stringstream stream;
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    time_t expires = time(NULL) + 60 * 60 * 24 * 30;
    char buff[40];
    strftime(buff, sizeof (buff), "%d-%b-%Y %H:%M:%S", localtime(&expires));
    std::string response = onyx::RedirectResponse("Вход в приложение", m_redirect_url);
    stream << "Set-Cookie: sessionid=" << boost::lexical_cast<std::string>(uuid) << "; expires=" << buff << ";\r\n " << response;
    m_session->create(boost::lexical_cast<std::string>(uuid));
    return stream.str();
}