#ifndef XMLRESPONSE_H
#define XMLRESPONSE_H

#include "BaseResponse.h"

namespace onyx {

    class XmlResponse : public BaseResponse {
    public:

        explicit XmlResponse(const std::string & body) :
        BaseResponse("Content-type: application/xml; charset=utf-8\r\n\r\n") {
            m_body = body;
        }
    };
}

#endif

