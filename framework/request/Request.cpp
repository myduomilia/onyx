#include "Request.h"

void onyx::Request::parse_cookie(const char * cookie) noexcept {
    std::unique_ptr<char[]> buffer(new char[strlen(cookie) + 1]);
    strncpy(buffer.get(), cookie, strlen(cookie) + 1);
    char *savep_tr;
    char *token = strtok_r(buffer.get(), ";", &savep_tr);
    while (token != NULL) {
        std::string cook(token);
        trim(cook);
        cook.erase(cook.find_last_not_of(" ") + 1);
        size_t pos = cook.find("=");
        if(pos != std::string::npos)
            m_cookie.insert(std::pair<std::string, std::string>(cook.substr(0, pos), cook.substr(pos + 1, cook.size())));
        token = strtok_r(NULL, ";", &savep_tr);
    }
}