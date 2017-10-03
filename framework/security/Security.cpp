#include "Security.h"

std::string onyx::Security::m_login_url = "/login";
std::string onyx::Security::m_auth_url = "/auth";
std::string onyx::Security::m_redirect_url = "/";

std::unique_ptr<onyx::Session> onyx::Security::m_session(nullptr);
std::function<std::string(const std::string & login, const std::string & password)> onyx::Security::m_callbackRole = nullptr;

static std::map<std::string, std::string> parse(const std::string& body) {
    std::map<std::string, std::string> m;
    std::unique_ptr<char[] > buffer(new char[body.size() + 1]);
    memset(buffer.get(), '\0', body.size() + 1);
    strncpy(buffer.get(), body.c_str(), body.size() + 1);
    char *savep_tr;
    char *token = strtok_r(buffer.get(), "&", &savep_tr);
    std::vector<std::string> tmp;
    while (token != NULL) {
        tmp.push_back(token);
        token = strtok_r(NULL, "&", &savep_tr);
    }
    for (size_t i = 0; i < tmp.size(); i++) {
        std::string value = tmp[i];
        size_t sep = value.find("=");
        std::string key, val;
        key = value.substr(0, sep);
        val = value.substr(sep + 1, value.size() - sep - 1);
        m.insert(std::pair<std::string, std::string>(key, val));
    }
    return m;
}



std::string onyx::Security::auth(onyx::ONObject& obj) {
    std::stringstream stream;
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    time_t expires = time(NULL) + 60 * 60 * 24 * 30;
    char buff[40];
    strftime(buff, sizeof (buff), "%d-%b-%Y %H:%M:%S", localtime(&expires));
    std::map<std::string, std::string> m = parse(obj.getBody());
    std::string login = "";
    std::string password = "";
    if(m.find("login") != m.end())
        login = m["login"];
    if(m.find("password") != m.end())
        password = m["password"];
    std::string role = m_callbackRole(login, password);
    if(role == "")
        return onyx::RedirectResponse("Авторизация", onyx::Security::m_login_url);
    std::string response = onyx::RedirectResponse("Вход в приложение", m_redirect_url);
    stream << "Set-Cookie: sessionid=" << boost::lexical_cast<std::string>(uuid) << "; expires=" << buff << ";\r\n " << response;
    m_session->create(boost::lexical_cast<std::string>(uuid));
    return stream.str();
}