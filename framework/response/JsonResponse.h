#ifndef JSONRESPONSE_H
#define JSONRESPONSE_H

#include "BaseResponse.h"


namespace onyx {
    class JsonResponse : public BaseResponse {
    public:
        JsonResponse(std::string m_body) : BaseResponse(m_body) {
            setHeader("Content-type: application/json; charset=utf-8\r\n\r\n");
        }

    };
}

#endif

