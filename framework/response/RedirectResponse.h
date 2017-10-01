#ifndef REDIRECTRESPONCE_H
#define REDIRECTRESPONCE_H

#include "BaseResponse.h"
#include <string>

namespace onyx {

    class RedirectResponse : public BaseResponse {
    private:

    public:

        explicit RedirectResponse(const std::string & title, const std::string & url) :
        BaseResponse("Content-type: text/html; charset=utf-8\r\n\r\n") {
            m_body = "<html><head><title>" + title + "</title><meta http-equiv=\"refresh\" content=0;URL=\"" + url + "\"></head></html>";
        }

    };
}

#endif

