#pragma once
#include "../general/includes.h"
#include <random>

namespace ComponentHelpers {
    class Null {

    private:
        int dummy = 0;
    
    };
}

class Object;
class ObjectHandle;
class Registry{
public:


    
    
    
    

    
    
    

    static entt::registry& Get();
    
    

    


private:
   
    

    
    
    static entt::registry m_Registry;

};


