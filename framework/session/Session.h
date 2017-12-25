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

    namespace session {

        class User {
        private:
            std::string m_id;
            std::string m_role;
        public:

            User(const std::string & id, const std::string & role) :
            m_id(id), m_role(role) {
            }

            std::string getId() const {
                return m_id;
            }

            std::string getRole() const {
                return m_role;
            }
            
        };
    }

    class Session {
    private:
        std::string m_id;
        std::string m_token;
        onyx::session::User m_user;
    public:

        Session(const std::string & id, const std::string & token, onyx::session::User user) : m_id(id), m_token(token), m_user(user) {
        }

        onyx::session::User getUser() const {
            return m_user;
        }
        
        std::string getToken() const {
            return m_token;
        }

    };

    class SessionStorage {
    public:
        virtual void createSession(const std::string & id, const std::string & token, const std::string & user_id) noexcept = 0;
        virtual std::unique_ptr<Session> fetchSession(const std::string & id) noexcept = 0;
        virtual void removeSession(const std::string & id) noexcept = 0;
        virtual void clearAllSession() noexcept = 0;
    };

    class SessionStorageEmpty : public SessionStorage {

        virtual void createSession(const std::string & id, const std::string & token, const std::string & user_id) noexcept override {

        };

        virtual std::unique_ptr<onyx::Session> fetchSession(const std::string & id) noexcept override {
            onyx::session::User user("", "");
            return std::unique_ptr<onyx::Session>(new onyx::Session("", "", user));
        };

        virtual void removeSession(const std::string & id) noexcept override {

        };

        virtual void clearAllSession() noexcept override {

        };
    };
}

#endif

