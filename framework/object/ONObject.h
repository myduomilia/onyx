#ifndef ONOBJECT_H
#define ONOBJECT_H

#include "../token/Token.h"
#include "../param/Param.h"
#include "../cookie/Cookie.h"
#include <memory>
#include "../common/plog/Log.h"

namespace onyx {

    class ONObject {
    private:
        TokenCollection m_token_collection;
        ParamCollection m_param_collection;
        CookieCollection m_cookies_collection;
        std::string m_body;
    public:
        
        ONObject(const TokenCollection & token, const ParamCollection & params, const CookieCollection & cookies, const std::string & body) : m_token_collection(token), m_param_collection(params), m_cookies_collection(cookies), m_body(body) {}

        TokenCollection getTokenCollection() const {
            return m_token_collection;
        }
        
        ParamCollection getParamCollection() const {
            return m_param_collection;
        }
        
        CookieCollection getCookiesCollection() const {
            return m_cookies_collection;
        }

        
        std::string getBody() const {
            return m_body;
        }

    };
}


#endif

