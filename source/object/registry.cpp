#include "registry.h"



entt::registry Registry::m_Registry;

entt::registry& Registry::Get() {
    return Registry::m_Registry;
}






















