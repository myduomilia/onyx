#ifndef REQUEST_H
#define REQUEST_H

#include <curl/curl.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

namespace onyx {

    class Request {
    private:
        std::string m_method;
        std::string m_url;
        std::string m_body;
        std::string m_ip;
        std::string m_params;
        std::string m_cookies;

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
        
        void setCookies(const char* cookies){
            m_cookies = cookies;
        }

        void setMethod(const std::string & method) {
            m_method = method;
        }

        void setIp(const std::string & ip) {
            m_ip = ip;
        }

        void setBody(const std::string & body) {
            m_body = body;
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

    };
}

#endif

