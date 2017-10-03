#ifndef SESSION_H
#define SESSION_H

#include <ctime>
#include <string>
#include <exception>
#include <memory>
#include <map>
#include <thread>
#include <mutex>

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
        UserSession m_user;
        std::map<std::string, std::string> m_data;
    public:
        
        Session(const std::string & id, UserSession user) : m_id(id), m_user(user) {
        }

        
        virtual std::unique_ptr<Session> create(const std::string & id) noexcept = 0;
        virtual std::unique_ptr<Session> fetch(const std::string & id) noexcept = 0;
        virtual void remove() noexcept = 0;
        virtual void clearAll() noexcept = 0;
        
        UserSession getUser() const {
            return m_user;
        }
        
        void addStringData(const std::string & key, const std::string & val) noexcept {
            m_data[key] = val;
        };
        std::string getStringData(const std::string & key) {
            if(m_data.find(key) == m_data.end())
                return m_data[key];
        }
        void removeData(const std::string & key) {
            if(m_data.find(key) != m_data.end()){
                auto it = m_data.find(key);
                m_data.erase(it);
            }
        };
        
    };
}

#endif

