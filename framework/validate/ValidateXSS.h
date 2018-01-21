#ifndef VALIDATEXSS_H
#define VALIDATEXSS_H

#include <boost/regex.hpp>
#include <string>

namespace onyx {
    namespace validate {

        class ValidateXSS {
        private:
            ValidateXSS(){}
        public:
            static std::string stripXSS(std::string & source);
        };

    }
}

#endif

