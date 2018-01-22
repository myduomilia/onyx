#ifndef VALIDATEXSS_H
#define VALIDATEXSS_H

#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <string>
#include <sstream>

namespace onyx {
    namespace validate {

        class ValidateXSS {
        private:
            ValidateXSS(){}
        public:
            static std::string stripXSS(const std::string & source);
            static std::string escapeXSS(const std::string & source);
        };

    }
}

#endif

