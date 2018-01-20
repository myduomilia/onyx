#ifndef FILTERCHAINPOST_H
#define FILTERCHAINPOST_H

#include "IBaseFilterChains.h"


class FilterChainPost : public IBaseFilterChainAuth {
    virtual std::string handler(const onyx::Request & request, onyx::ONObject & obj, const onyx::Dispatcher::Route & route, std::shared_ptr<onyx::Session> session, const std::string & sessionid) override;
};

#endif

