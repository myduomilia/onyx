#ifndef PLAINTEXTRESPONSE_H
#define PLAINTEXTRESPONSE_H

#include "BaseResponse.h"

namespace onyx {

    class PlainTextResponse : public BaseResponse {
    public:

        explicit PlainTextResponse(const std::string & body) :
        BaseResponse("Content-type: text/plain; charset=utf-8\r\n\r\n") {
            m_body = body;
        }
    };
}

#endif

