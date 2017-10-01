#ifndef HTMLRESPONSE_H
#define HTMLRESPONSE_H

#include "BaseResponse.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../common/mustache/mustache.h"

namespace onyx {
    
    class HtmlResponse : public BaseResponse {
    private:

        std::string file_reader(const std::string & path_to_html_file) {
            std::stringstream buffer;
            std::string line;
            std::ifstream file(path_to_html_file.c_str());
            if (file.is_open()) {
                while (file.good()) {
                    getline(file, line);
                    buffer << line;
                }
                file.close();
            }
            return buffer.str();
        }
    public:

        explicit HtmlResponse(const std::string & path_to_html_file) :
        BaseResponse("Content-type: text/html; charset=utf-8\r\n\r\n") {
            m_body = file_reader(path_to_html_file);
        }

    };
}

#endif