#include "Security.h"
#include "../request/Request.h"

onyx::Security * onyx::Security::m_instance = nullptr;

//std::function<onyx::session::User * (const std::string & login, const std::string & password) > onyx::Security::callbackUser = nullptr;

onyx::Security::Security() {
    m_session_storage = nullptr;
    m_callbackUser = nullptr;
    m_login_url = "/login";
    m_auth_url = "/auth";
    m_redirect_url = "/";
}

const std::function<std::string(onyx::ONObject &)> onyx::Security::fetchAuthHandler() {
    return [](onyx::ONObject & obj) -> std::string {
        onyx::Security * security = onyx::Security::getInstance();
        std::stringstream stream;
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        boost::uuids::uuid token = boost::uuids::random_generator()();
        time_t expires = time(NULL) + 60 * 60 * 24 * 30;
        char buff[40];
        strftime(buff, sizeof (buff), "%a, %d-%b-%Y %H:%M:%S", localtime(&expires));
        std::map<std::string, std::string> m = onyx::Request::parse_form_params(obj.getBody());
        std::string login = "";
        std::string password = "";
        if (m.find("login") != m.end())
            m["login"];
        if (m.find("password") != m.end())
            password = m["password"];
        std::unique_ptr<onyx::session::User> user(security->getCallbackUser()(login, password));
        if (user == nullptr)
            return onyx::RedirectResponse("Login", security->getLoginURL());
        std::string response = onyx::RedirectResponse("Login", security->getRedirectURL());
        stream << "Set-Cookie: sessionid=" << boost::lexical_cast<std::string>(uuid) << "; expires=" << buff << "; HttpOnly;\r\n " << response;
        security->getSessionStorage()->createSession(boost::lexical_cast<std::string>(uuid), boost::lexical_cast<std::string>(token), user->getId());
        return stream.str();
    };

}