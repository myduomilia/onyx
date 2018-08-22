#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <algorithm>

namespace onyx {
    namespace utils {
        
        /*
         * delete all characters space from left
         */
        static inline void ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
        }
        
        /*
         * delete all characters space from right
         */
        static inline void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), s.end());
        }
        
        /*
         * remove all characters from both ends
         */
        static inline void trim(std::string &s) {
            ltrim(s);
            rtrim(s);
        }
        
        /*
         * Decode url string
         */

        static void urldecode(char *st) {
            char *p = st;
            char hex[3];
            int code;
            do {
                if (*st == '%') {
                    hex[0] = *(++st);
                    hex[1] = *(++st);
                    hex[2] = 0;
                    sscanf(hex, "%X", &code);
                    *p++ = (char) code;
                }
                else if (*st == '+') *p++ = ' ';
                else *p++ = *st;
            } while (*st++ != 0);
        }
    }

}

#endif

