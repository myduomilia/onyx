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
        time_t m_expires;
    public:
        Session(){
            
        }
        
        void setExpires(time_t expires) {
            m_expires = expires;
        }
        
        time_t getExpires() const {
            return m_expires;
        }
        
        virtual UserSession fetchUserBySessionId(const std::string & id) noexcept = 0;
        virtual void create(const std::string & id) noexcept = 0;
        virtual void remove() noexcept = 0;

        
    };
}

#endif

