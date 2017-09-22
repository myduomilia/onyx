#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <string.h>
#include <memory>
#include "../exception/Exception.h"
#include "../common/plog/Log.h"

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

namespace onyx {

    class TokenCollection {
    private:
        std::vector<std::string> m_tokens;
    public:
        TokenCollection(const std::string & url);

        std::string & operator[](size_t index) {
            if(index > m_tokens.size() || index < 0)
                throw onyx::Exception("index of token not exists");
            return m_tokens[index];
        }
        
        int size(){
            return m_tokens.size();
        }
    };
}

#endif

