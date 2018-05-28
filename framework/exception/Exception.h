#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace onyx {

    class Exception : public std::exception {
    private:
        std::string m_message;
        int m_code;
    public:

        Exception(const std::string & message, int code = 0) :
        exception(), m_message(message), m_code(code) {
        }

        virtual const char* what() const throw() override {
            return m_message.c_str();
        }
        
        virtual int code() const {
            return m_code;
        }
    };
}

#endif

