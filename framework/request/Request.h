#ifndef REQUEST_H
#define REQUEST_H

#include <curl/curl.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#include "../common/utils.h"

namespace onyx {

    

    class Request {
    private:
        std::string m_method;
        std::string m_url;
        std::string m_body;
        std::string m_ip;
        std::string m_params;
        std::string m_cookies;
        std::string m_content_type;

        static inline void ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
        }

        static inline void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), s.end());
        }

        static inline void trim(std::string &s) {
            ltrim(s);
            rtrim(s);
        }

    public:
        std::vector<std::string> fetch_tokens_url() noexcept;

        void setCookies(const char* cookies) {
            m_cookies = cookies;
        }

        void setMethod(const std::string & method) {
            m_method = method;
        }

        void setIp(const std::string & ip) {
            m_ip = ip;
        }

        void setBody(const char* body) {
            char * data = (char *) malloc(strlen(body) + 1);
            memset(data, 0, strlen(body) + 1);
            memcpy(data, body, strlen(body));
            onyx::utils::urldecode(data);
            m_body = data;
            free(data);
        }

        void setUrl(const char* url) {
            char * url_decode = curl_unescape(url, strlen(url));
            m_url = url_decode;
            curl_free(url_decode);
        }

        void setParams(const char* params) {
            char * params_decode = curl_unescape(params, strlen(params));
            m_params = params_decode;
            curl_free(params_decode);
        }

        void setContentType(const std::string content_type) {
            m_content_type = content_type;
        }

        std::string getUrl() const {
            return m_url;
        }

        std::string getMethod() const {
            return m_method;
        }

        std::string getParams() const {
            return m_params;
        }

        std::string getBody() const {
            return m_body;
        }

        std::string getCookies() const {
            return m_cookies;
        }

        std::string getContentType() const {
            return m_content_type;
        }

        static std::map<std::string, std::string> parse_form_params(const std::string& body) {
            std::map<std::string, std::string> map_form_params;
            std::unique_ptr<char[] > buffer(new char[body.size() + 1]);
            memset(buffer.get(), '\0', body.size() + 1);
            strncpy(buffer.get(), body.c_str(), body.size() + 1);
            char *savep_tr;
            char *token = strtok_r(buffer.get(), "&", &savep_tr);
            std::vector<std::string> tmp;
            while (token != NULL) {
                tmp.push_back(token);
                token = strtok_r(NULL, "&", &savep_tr);
            }
            for (size_t i = 0; i < tmp.size(); i++) {
                std::string value = tmp[i];
                size_t sep = value.find("=");
                std::string key, val;
                key = value.substr(0, sep);
                val = value.substr(sep + 1, value.size() - sep - 1);
                map_form_params.insert(std::pair<std::string, std::string>(key, val));
            }
            return map_form_params;
        }

    };
}

#endif

