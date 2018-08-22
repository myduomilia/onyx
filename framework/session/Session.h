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

            User(const std::string & id = "", const std::string & role = "") : m_id(id), m_role(role) {
            }

            std::string getId() const {
                return m_id;
            }

            std::string getRole() const {
                return m_role;
            }

        };

    }
    
    /**
    \brief Класс представляющий объект сессии
    \author myduomilia
    \date Январь 2018
    */
    class Session {
    private:
        std::string m_id;
        std::string m_token;
        onyx::session::User m_user;

    public:
        
        /**
            Конструктор сессии
            \param[in] id - индентификатор сессии
            \param[in] token - токен сессии, используется для защиты от csrf атак
            \param[in] user - объект связанного пользователя
            \warning Пользователь с пустый ID считается обыкновенным пользователем
        */
        Session(const std::string & id, const std::string & token, onyx::session::User user) : m_id(id), m_token(token), m_user(user) {
        }

        /**
            Получение связанного пользователя
            \return объект связанного пользователя
         */
        onyx::session::User getUser() const {
            return m_user;
        }

        /**
            Получение токена сессии
            \return токен сессии
         */
        std::string getToken() const {
            return m_token;
        }

        /**
            Получение идентификатора сессии
            \return идентификатор сессии
         */
        std::string getId() const {
            return m_id;
        }


    };

    /**
    \brief Интерфейс хранилища сессий
    \author myduomilia
    \date Январь 2018
    \warning Вы обязательно должны предоставить имплементацию данного класса
     */

    class ISessionStorage {
    public:
        /**
         Создание сессии и помещение ее в хранилище (например БД)
         \param[in] id - индентификатор сессии
         \param[in] token - токен сессии предназначенный для защиты сессии от csrf атаки
         \param[in] user_id - идентификатор связанного с сессией пользователя
         */
        virtual void createSession(const std::string & id, const std::string & token, const std::string & user_id) noexcept = 0;
        /**
            Получение сесси из хранилища
            \param[in] id - индентификатор сессии
            \return Возвращает указатель на объект Session
            \warning Возвращение nullptr говорит о том что данная сессия не найдена
         */
        virtual Session * fetchSession(const std::string & id) noexcept = 0;
        /**
            Удаление сессии из хранилища
            \param[in] id - индентификатор сессии
         */
        virtual void removeSession(const std::string & id) noexcept = 0;
        /**
            Очистка всего хранилища сессии
            \param[in] id - индентификатор сессии
         */
        virtual void clearAllSession() noexcept = 0;
    };

}

#endif

