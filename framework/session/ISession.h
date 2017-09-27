#ifndef ISESSION_H
#define ISESSION_H

#include <ctime>
#include <string>
#include <exception>
#include <memory>
#include "../exception/Exception.h"

template <typename T, typename V>
class ISession {
private:
    T m_session_id;
    V m_user_id;
    std::string m_data;
    time_t timestamp_expire;
public:
    
    virtual ~ISession() {
    }
    
    void setData(const std::string & data) {
        m_data = data;
    }

    void setTimestampExpire(time_t timestamp_expire) {
        this->timestamp_expire = timestamp_expire;
    }
    
    std::string getData() const {
        return m_data;
    }

    const T getSessionId() const {
        return m_session_id;
    }

    const V getUserId() const {
        return m_user_id;
    }

    time_t getTimestampExpire() const {
        return timestamp_expire;
    }

    virtual ISession fetch() throw(onyx::Exception) = 0;
    virtual void create() = 0;
    virtual void save() = 0;
    virtual void remove() = 0;

};

#endif

