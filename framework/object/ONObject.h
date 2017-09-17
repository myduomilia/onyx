#ifndef ONOBJECT_H
#define ONOBJECT_H

#include "../token/Token.h"
#include <memory>
#include "../common/plog/Log.h"

namespace onyx {

    class ONObject {
    private:
        TokenCollection m_token;
    public:

        ONObject(TokenCollection token) : m_token(token) {

        }

        TokenCollection getTokenCollection() const {
            return m_token;
        }
    };
}


#endif

