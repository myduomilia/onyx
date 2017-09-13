#ifndef JSONRESPONSE_H
#define JSONRESPONSE_H

#include "BaseResponse.h"


namespace onyx {

    class JsonResponse : public BaseResponse {
    public:

        explicit JsonResponse(const std::string & body) :
        BaseResponse("Content-type: application/json; charset=utf-8\r\n\r\n") {
            m_body = body;
        }
         
    };
}

#endif

