#ifndef CSVRESPONSE_H
#define CSVRESPONSE_H

namespace onyx {

    class CsvResponse : public BaseResponse {
    public:

        explicit CsvResponse(const std::string & body) :
        BaseResponse("Content-type: application/csv; charset=utf-8\r\n\r\n") {
            m_body = body;
        }
    };
}

#endif

