#ifndef PARAM_H
#define PARAM_H

#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <string.h>
#include <memory>
#include "../exception/Exception.h"
#include "../common/plog/Log.h"

namespace onyx {

    class ParamCollection {
    private:
        std::map<std::string, std::string> m_params;
    public:
        ParamCollection(const std::string & params);
        
        std::map<std::string, std::string> getMap(){
            return m_params;
        }

    };
}



#endif

