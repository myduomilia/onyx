#include "Token.h"

onyx::TokenCollection::TokenCollection(const std::string & url){
    std::unique_ptr<char[]> buffer(new char[url.size() + 1]);
    memset(buffer.get(), '\0', url.size() + 1);
    strncpy(buffer.get(), url.c_str(), url.size());
    char *token = strtok(buffer.get(), "/");
    while (token != NULL) {
        m_tokens.push_back(token);
        token = strtok(NULL, "/");
    }
}