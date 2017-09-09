#include "Request.h"

void onyx::Request::parse_tokens_queries(const char * url) noexcept {
    char * params_decode = curl_unescape(url, strlen(url));       
    std::vector<std::string> tokens;
    std::unique_ptr<char[]> buffer(new char[strlen(params_decode) + 1]);
    strncpy(buffer.get(), params_decode, strlen(params_decode) + 1);
    char *token = strtok(buffer.get(), "&");
    while (token != NULL) {
        tokens.push_back(token);
        token = strtok(NULL, "&");
    }
    for (size_t i = 0; i < tokens.size(); i++) {
        std::string value = tokens[i];
        size_t sep = value.find("=");
        std::string key, val;
        key = value.substr(0, sep);
        val = value.substr(sep + 1, value.size() - sep - 1);
        m_params.insert(std::pair<std::string, std::string>(key, val));
    }
    curl_free(params_decode);
} 

void onyx::Request::parse_cookie(const char * cookie) noexcept {
    std::unique_ptr<char[]> buffer(new char[strlen(cookie) + 1]);
    strncpy(buffer.get(), cookie, strlen(cookie) + 1);
    char *token = strtok(buffer.get(), ";");
    while (token != NULL) {
        std::string cook(token);
        trim(cook);
        cook.erase(cook.find_last_not_of(" ") + 1);
        size_t pos = cook.find("=");
        if(pos != std::string::npos)
            m_cookie.insert(std::pair<std::string, std::string>(cook.substr(0, pos), cook.substr(pos + 1, cook.size())));
        token = strtok(NULL, ";");
    }
}

std::vector<std::string> onyx::Request::fetch_tokens_url() noexcept {
    std::vector<std::string> tokens;
    std::unique_ptr<char[]> buffer(new char[m_url.size() + 1]);
    memset(buffer.get(), '\0', m_url.size() + 1);
    strncpy(buffer.get(), m_url.c_str(), m_url.size());
    char *token = strtok(buffer.get(), "/");
    while (token != NULL) {
        tokens.push_back(token);
        token = strtok(NULL, "/");
    }
    return tokens;
}