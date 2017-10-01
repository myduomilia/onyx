#include "403.h"

#include "../response/Response403.h"

std::string onyx::handler::_403(){
    return onyx::Response403();
}