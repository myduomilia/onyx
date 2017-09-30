#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <map>
#include <functional>
#include <string>
#include <string.h>
#include <memory>
#include "../exception/Exception.h"
#include "../common/plog/Log.h"

namespace onyx {

    class TokenCollection {
    private:
        std::vector<std::string> m_tokens;
    public:
        TokenCollection(const std::string & url);

        std::string & operator[](size_t index) {
            if(index > m_tokens.size() || index < 0)
                throw onyx::Exception("index not exists");
            return m_tokens[index];
        }
        
        int size(){
            return m_tokens.size();
        }
    };
}

#endif

