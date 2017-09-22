#ifndef ONOBJECT_H
#define ONOBJECT_H

#include "../token/Token.h"
#include "../param/Param.h"
#include <memory>
#include "../common/plog/Log.h"

namespace onyx {

    class ONObject {
    private:
        TokenCollection m_token;
        ParamCollection m_params;
    public:

        ONObject(const TokenCollection & token, const ParamCollection & params) : m_token(token), m_params(params) {}

        TokenCollection getTokens() const {
            return m_token;
        }
        
        ParamCollection getParams() const {
            return m_params;
        }
    };
}


#endif

