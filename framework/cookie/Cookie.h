#ifndef COOKIE_H
#define COOKIE_H

#include <cstring>
#include <memory>
#include <map>
#include "../exception/Exception.h"
#include "../common/utils.h"

namespace onyx {

    class CookieCollection {
    private:
        std::map<std::string, std::string> m_cookies;
    public:
        CookieCollection(const std::string & cookies);

        std::string & operator[](const std::string & key) {
            auto it = m_cookies.find(key);
            if (it == m_cookies.end())
                throw onyx::Exception("key not exists");
            return it->second;
        }

        int size() {
            return m_cookies.size();
        }


    };
}


#endif

