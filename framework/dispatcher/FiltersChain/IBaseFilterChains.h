#ifndef BASEFILTERCHAINS_H
#define BASEFILTERCHAINS_H

#include "../Dispatcher.h"
#include "../../request/Request.h"
#include <string>
#include "../../handlers/403.h"

class IBaseFilterChainAuth {
protected:
    IBaseFilterChainAuth * m_nextFilterChain;
public:
    IBaseFilterChainAuth(){
        m_nextFilterChain = nullptr;
    }
    void setNextHandler(IBaseFilterChainAuth * nextFilterChain){
        m_nextFilterChain = nextFilterChain;
    }
    virtual std::string handler(const onyx::Request & request, onyx::ONObject & obj, const onyx::Dispatcher::Route & route, std::shared_ptr<onyx::Session> session, const std::string & sessionid) = 0;
};

#endif

