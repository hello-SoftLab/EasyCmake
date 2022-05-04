#include "helpers.h"
#include "../object/registry.h"
#include "../object/object.h"

size_t HelperFunctions::HashPtr(void* ptr)
{
    return std::hash<void*>()(ptr);
}



std::string HelperFunctions::GenerateStringHash(void* ptr)
{
    return std::to_string(HashPtr(ptr));
}

bool HelperFunctions::EraseWordFromString(std::string& mainWord, std::string wordToLookFor) {
    auto iter = mainWord.find(wordToLookFor);
    
    bool foundAny = false;
    if(iter != std::string::npos){
        foundAny = true;
    }
    while (iter != std::string::npos) {
        
        mainWord.erase(iter, wordToLookFor.length());
        
        iter = mainWord.find(wordToLookFor, iter);
    }
    return foundAny;
}

bool HelperFunctions::IsMetaClass(std::string className) {
    return entt::resolve(entt::hashed_string(className.c_str())).operator bool();
}

bool HelperFunctions::IsMetaFunction(std::string className, std::string funcName)
{
    if (!IsMetaClass(className)) {
        return false;
    }
    return entt::resolve(entt::hashed_string(className.c_str())).func(entt::hashed_string(funcName.c_str())).operator bool();
}

ObjectHandle::ObjectHandle(entt::entity ent)
{
    m_Handle = ent;
}

ObjectHandle::ObjectHandle(Object obj) : m_Handle(obj.ID())
{
}

ObjectHandle::ObjectHandle()
{
    isNull = true;
}

Object ObjectHandle::GetAsObject()
{
    return Object(m_Handle);
}



entt::entity ObjectHandle::ID()
{
    return m_Handle;
}

std::string ObjectHandle::GetType()
{
    return GetAsObject().GetType();
}

std::string ObjectHandle::ToString()
{
    return std::to_string((uint32_t)m_Handle);
}

bool ObjectHandle::IsType(entt::id_type type)
{
    return type == GetAsObject().GetTypeOfObject();
}


bool ObjectHandle::operator==(const ObjectHandle& other)
{
    return this->m_Handle == other.m_Handle;
}

ObjectHandle::operator bool() const{
    if (this->isNull) {
        return false;
    }
    return Registry::Get().valid(this->m_Handle);
}