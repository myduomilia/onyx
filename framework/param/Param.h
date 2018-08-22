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
        
        std::string & operator[](const std::string & key) {
            auto it = m_params.find(key);
            if(it == m_params.end())
                throw onyx::Exception("Key doesn't exists in the ParamCollection");
            return it->second;
        }
        
        bool has(const std::string & key){
            auto it = m_params.find(key);
            if(it == m_params.end())
                return false;
            return true;
        }
        
        int size(){
            return m_params.size();
        }
        

    };
}



#endif

