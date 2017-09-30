#include "Cookie.h"

onyx::CookieCollection::CookieCollection(const std::string& cookies){
    std::unique_ptr<char[]> buffer(new char[cookies.size() + 1]);
    memset(buffer.get(), '\0', cookies.size() + 1);
    strncpy(buffer.get(), cookies.data(), cookies.size() + 1);
    char *savep_tr;
    char *token = strtok_r(buffer.get(), ";", &savep_tr);
    while (token != NULL) {
        std::string cook(token);
        onyx::utils::trim(cook);
        cook.erase(cook.find_last_not_of(" ") + 1);
        size_t pos = cook.find("=");
        if(pos != std::string::npos)
            m_cookies.insert(std::pair<std::string, std::string>(cook.substr(0, pos), cook.substr(pos + 1, cook.size())));
        token = strtok_r(NULL, ";", &savep_tr);
    }
}