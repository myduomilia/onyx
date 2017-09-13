#ifndef BASERESPONSE_H
#define BASERESPONSE_H

namespace onyx {

    class BaseResponse {
    protected:
        std::string m_header;
        std::string m_body;
    public:

        explicit BaseResponse(const std::string & header) :
        m_header(header), m_body() {
        }

        std::string getResponseStr() const noexcept {
            return m_header + m_body;
        }
         
        operator std::string(){
            return m_header + m_body;
        }

    };
}

#endif

