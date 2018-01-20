#ifndef FILTERCHAINAUTH_H
#define FILTERCHAINAUTH_H

#include "IBaseFilterChains.h"

class FilterChainCheckRole : public IBaseFilterChainAuth {
public:
    virtual std::string handler(const onyx::Request & request, onyx::ONObject & obj, const onyx::Dispatcher::Route & route, std::shared_ptr<onyx::Session> session, const std::string & sessionid) override;
};

#endif

