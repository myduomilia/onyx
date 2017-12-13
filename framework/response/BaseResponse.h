#ifndef BASERESPONSE_H
#define BASERESPONSE_H

namespace onyx {

    class BaseResponse {
    protected:
        std::string m_header;
        std::string m_body;
    public:

        explicit BaseResponse(const std::string & header) : m_header(header){
        }
         
        operator std::string(){
            return m_header + m_body;
        }
        
        void addHeader(const std::string & header){
            std::string str = "\r\n" + header;
            m_header.insert(m_header.size() - 4, str);
        }

    };
}

#endif

