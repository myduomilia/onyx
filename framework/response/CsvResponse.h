#ifndef CSVRESPONSE_H
#define CSVRESPONSE_H

#include <sstream>

namespace onyx {

    class CsvResponse : public BaseResponse {
    public:

        CsvResponse(const std::string & body) : 
        BaseResponse("Content-Disposition: attachment; filename=download.csv\r\nContent-type: application/csv; charset=utf-8\r\n\r\n") {
            m_body = body;
        }
    };
}

#endif

