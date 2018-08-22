#ifndef COOKIE_H
#define COOKIE_H

#include <cstring>
#include <memory>
#include <map>
#include "../exception/Exception.h"
#include "../common/utils.h"

namespace onyx {
    
    /*
     * Parse the string raw cookies and create map cookies
    */

    class CookieCollection {
    private:
        std::map<std::string, std::string> m_cookies;
    public:
        CookieCollection(const std::string & cookies);

        std::string & operator[](const std::string & key) {
            auto it = m_cookies.find(key);
            if (it == m_cookies.end())
                throw onyx::Exception("Key doesn't exists in the CookieCollection");
            return it->second;
        }
        
        bool has(const std::string & key){
            auto it = m_cookies.find(key);
            if(it == m_cookies.end())
                return false;
            return true;
        }

        int size() {
            return m_cookies.size();
        }


    };
}


#endif

