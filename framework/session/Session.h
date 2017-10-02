#ifndef SESSION_H
#define SESSION_H

#include <ctime>
#include <string>
#include <exception>
#include <memory>

namespace onyx {
    
    class UserSession {
    private:
        std::string m_id;
        std::string m_role;
    public:
        UserSession(std::string m_id, std::string m_role) :
        m_id(m_id), m_role(m_role) {
        }
        
        std::string getId() const {
            return m_id;
        }

        std::string getRole() const {
            return m_role;
        }
        
    };

    class Session {
        
    private:
        std::string m_id;
        std::string m_data;
        time_t m_expires;
    public:
        
        Session(const std::string & id, std::string & data, time_t expires) : m_id(id), m_data(data), m_expires(expires) {
            
        }
        
        Session(){
            
        }
        
        void setData(const std::string & data) {
            m_data = data;
        }

        void setExpires(time_t expires) {
            m_expires = expires;
        }
        
        std::string getData() const {
            return m_data;
        }

        time_t getExpires() const {
            return m_expires;
        }
        
        virtual void save() = 0;
        virtual void remove() = 0;
        virtual UserSession fetchUserBySessionId(const std::string & id) = 0;

        
    };
}

#endif

