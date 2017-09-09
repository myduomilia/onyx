#ifndef BASERESPONSE_H
#define BASERESPONSE_H

namespace onyx {

    class BaseResponse {
    private:
        std::string m_header;
        std::string m_body;
    public:

        BaseResponse(std::string m_body) :
        m_body(m_body) {
        }

        std::string getResponse() const noexcept {
            return m_header + m_body;
        }
        
        void setHeader(const std::string & header) {
            m_header = header;
        }


    };
}

#endif

