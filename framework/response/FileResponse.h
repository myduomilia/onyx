#ifndef FILERESPONSE_H
#define FILERESPONSE_H

#include "BaseResponse.h"
#include <iostream>
#include <fstream>
#include <string>

namespace onyx {

    class FileResponse : public BaseResponse {
    private:

        std::string file_reader(const std::string & path_to_file) {
            std::string buffer;
            std::ifstream file(path_to_file, std::ios::in | std::ifstream::binary);
            if (file.good()) {
                buffer = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();
            }
            return buffer;
        }
    public:

        explicit FileResponse(const std::string & path_to_file) :
        BaseResponse("Content-Description: File Transfer;\r\nContent-Type: application/octet-stream;\r\nContent-Transfer-Encoding: binary;\r\n\r\n") {
            m_body = file_reader(path_to_file);
            std::string header = "Content-Length: " + std::to_string(m_body.size());
            addHeader(header);
        }

    };
}


#endif

