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

            User(std::string m_id, std::string m_role) :
            m_id(m_id), m_role(m_role) {
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
        onyx::session::User m_user;
    public:

        Session(const std::string & id, onyx::session::User user) : m_id(id), m_user(user) {
        }

        onyx::session::User getUser() const {
            return m_user;
        }

    };

    class SessionStorage {
    public:
        virtual std::unique_ptr<Session> createSession(const std::string & id, const std::string & user_id) noexcept = 0;
        virtual std::unique_ptr<Session> fetchSession(const std::string & id) noexcept = 0;
        virtual void removeSession(const std::string & id) noexcept = 0;
        virtual void clearAllSession() noexcept = 0;
    };
}

#endif

