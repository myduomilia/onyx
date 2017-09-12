#ifndef HTMLRESPONSE_H
#define HTMLRESPONSE_H

#include "BaseResponse.h"

namespace onyx {

    class HtmlResponse : public BaseResponse {
    public:

        explicit HtmlResponse(std::string body) :
        BaseResponse("Content-type: text/html; charset=utf-8\r\n\r\n") {
            m_body = body;
        }

    };
}

#endif