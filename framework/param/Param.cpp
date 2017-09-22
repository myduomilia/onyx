#include "Param.h"

onyx::ParamCollection::ParamCollection(const std::string& params) {
    std::unique_ptr<char[] > buffer(new char[params.size() + 1]);
    memset(buffer.get(), '\0', params.size() + 1);
    strncpy(buffer.get(), params.c_str(), params.size() + 1);
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
        m_params.insert(std::pair<std::string, std::string>(key, val));
    }
}