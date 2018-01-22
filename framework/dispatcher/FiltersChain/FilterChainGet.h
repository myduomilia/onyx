#ifndef FILTERCHAINGET_H
#define FILTERCHAINGET_H

#include "IBaseFilterChains.h"


class FilterChainGet : public IBaseFilterChainAuth {
    virtual std::string handler(const onyx::Request & request, onyx::ONObject & obj, const onyx::Dispatcher::Route & route, std::shared_ptr<onyx::Session> session, const std::string & sessionid) override;
};

#endif

