#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace onyx {

    class Exception : public std::exception {
    private:
        std::string m_message;
    public:

        Exception(const std::string & message) :
        exception(), m_message(message) {
        }

        virtual const char* what() const throw() override {
            return m_message.c_str();
        }
    };
}

#endif

