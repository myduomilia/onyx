#include "404.h"
#include "../response/Response404.h"

std::string onyx::handler::_404(){
    return onyx::Response404();
}