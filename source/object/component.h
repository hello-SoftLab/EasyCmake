#pragma once 
#include <concepts>
#include "../../vendor/entt/single_include/entt/entt.hpp"
#include "registry.h"





class Component {

public:
    bool Valid() {
        return Registry::Get().valid(m_MasterHandle);
    }

    
protected:
    
    Component& operator=(const Component& comp){
        return *this;

    }
    /**
     * Use instead of constructor.
     */
    virtual void Init() {};
    /**
     * Use instead of destructor.
     */
    virtual void Destroy() {};

    virtual void Update(float delta) {};

    /**
     * Serializer function.
     * Called before Destroy().
     * *WARNING* -> node is already inside of component
     */
    virtual YAML::Node Serialize() { return {}; };

    /**
     * Deserializer function.
     * Called after Init()
     * *WARNING* -> node is already inside of component
     */
    virtual bool Deserialize(YAML::Node& node) { return true; };

    


    

    ~Component(){}

    

    


private:

    void SetMaster(entt::entity entity) {
        m_MasterHandle = entity;
    };
    
    
    entt::entity m_MasterHandle = entt::null;
    
    
    
    friend class Window;
    
    template<typename,typename>
    friend class ComponentSpecifier;
    
    
    friend class ObjectPropertyRegister;

};




