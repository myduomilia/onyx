#include "Security.h"
#include "../request/Request.h"

std::string onyx::Security::m_login_url = "/login";
std::string onyx::Security::m_auth_url = "/auth";
std::string onyx::Security::m_redirect_url = "/";

std::unique_ptr<onyx::SessionStorage> onyx::Security::m_session_storage(nullptr);
std::function<onyx::session::User(const std::string & login, const std::string & password) > onyx::Security::m_callbackUser = nullptr;

std::string onyx::Security::auth(onyx::ONObject& obj) {
    std::stringstream stream;
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    time_t expires = time(NULL) + 60 * 60 * 24 * 30;
    char buff[40];
    strftime(buff, sizeof (buff), "%a, %d-%b-%Y %H:%M:%S", localtime(&expires));
    std::map<std::string, std::string> m = onyx::Request::parse_form_params(obj.getBody());
    std::string login = "";
    std::string password = "";
    if (m.find("login") != m.end())
        login = m["login"];
    if (m.find("password") != m.end())
        password = m["password"];
    onyx::session::User user = m_callbackUser(login, password);
    if (user.getId() == "")
        return onyx::RedirectResponse("Login", onyx::Security::m_login_url);
    std::string response = onyx::RedirectResponse("Login", m_redirect_url);
    stream << "Set-Cookie: sessionid=" << boost::lexical_cast<std::string>(uuid) << "; expires=" << buff << "; HttpOnly;\r\n " << response;
    m_session_storage->createSession(boost::lexical_cast<std::string>(uuid), user.getId());
    return stream.str();
}

onyx::session::User onyx::security::CallbackUserEmpty(const std::string & login, const std::string & password){
    return onyx::session::User("", "");
}