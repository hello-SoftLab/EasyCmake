#pragma once
#include "includes.h"
#include "color.h"
#include "../object/registry.h"


class Object;

class ObjectHandle {
public:
    ObjectHandle(entt::entity ent);
    ObjectHandle(Object obj);
    ObjectHandle();

    Object GetAsObject();

    template<typename T>
    T GetAs() {
        return T(m_Handle);
    }

    operator bool() const;

    entt::entity ID();

    std::string GetType();

    std::string ToString();

    bool IsType(entt::id_type type);

    bool operator==(const ObjectHandle& other);



private:
    entt::entity m_Handle = entt::null;
    bool isNull = false;

};


class HelperFunctions {
public:
    static inline std::vector<std::string> SplitString(const std::string &str, const std::string &delimiter = " ", const int max_elements = 0) {
        std::vector<std::string> tokens;
        std::string::size_type start_index = 0;
        while (true) {
            std::string::size_type next_index = str.find(delimiter, start_index);
            if (next_index == std::string::npos) {
                if(str.substr(start_index) != ""){
                tokens.push_back(str.substr(start_index));
                }
                break;
            } else {
                if(str.substr(start_index, next_index - start_index) != ""){
                    tokens.push_back(str.substr(start_index, next_index - start_index));
                }
                start_index = next_index + delimiter.length();
            }
            if (max_elements > 0 && tokens.size() == max_elements - 1) {
                tokens.push_back(str.substr(start_index));
                break;
            }
        }

        return tokens;
    }

    


    static bool EraseWordFromString(std::string& mainWord, std::string wordToLookFor);

    template<typename T>
    static std::string GetClassName() {
        std::string name = std::string(entt::type_id<T>().name());
        HelperFunctions::EraseWordFromString(name, "class ");
        HelperFunctions::EraseWordFromString(name, "struct ");
        if (auto loc = name.find_last_of(':'); loc != std::string::npos) {
            name = std::string(name.begin() + loc + 1, name.end());
        }
        return name;
    }

    template<typename T>
    static std::string GetClassDisplayName() {
        std::string name = std::string(entt::type_id<T>().name());
        HelperFunctions::EraseWordFromString(name, "Component");
        HelperFunctions::EraseWordFromString(name, "class ");
        HelperFunctions::EraseWordFromString(name, "struct ");
        if (auto loc = name.find_last_of(':'); loc != std::string::npos) {
            name = std::string(name.begin() + loc + 1, name.end());
        }
        return name;
    }

    

    template<typename T>
    static entt::id_type HashClassName() {
        return entt::hashed_string(GetClassName<T>().c_str());
    }

    template<typename... Args>
    static entt::meta_any CallMetaFunction(std::string handle, std::string funcName,Args&&... args) {
        auto resolved = entt::resolve(entt::hashed_string(handle.c_str()));

        if (resolved) {
            if (auto func = resolved.func(entt::hashed_string(funcName.c_str())); func) {
                return func.invoke({}, std::forward<Args>(args)...);
            }
            return {};
        }
        return {};
    }

    template<typename Class,typename FuncWithArgs>
    static bool RegisterMemberMetaFunction(std::string funcMetaName) {
        entt::id_type hash = HelperFunctions::HashClassName<Class>();
        entt::meta<Class>().type(hash).template func<FuncWithArgs>(entt::hashed_string(funcMetaName.c_str()));
        return true;
    }

    template<typename Class,typename Func>
    static bool RegisterObjectMemberMetaFunction(std::string funcMetaName,Func function) {
        entt::id_type hash = HelperFunctions::HashClassName<Class>();
        entt::meta<Class>().type(hash).template func<&HelperFunctions::CallFunctionForObject<Class,Func>>(entt::hashed_string(funcMetaName.c_str()));
        return true;
    }

    template<typename Base,typename Class, typename Func>
    static bool RegisterVirtualObjectMemberMetaFunction(std::string funcMetaName) {
        entt::id_type hash = HelperFunctions::HashClassName<Class>();
        entt::meta<Class>().type(hash).template func<&HelperFunctions::CallFunctionForObjectWithVirtualBase<Base,Class, Func>>(entt::hashed_string(funcMetaName.c_str()));
        return true;
    }


    

    template<typename T>
    static void SerializeVariable(std::string name, T& var, YAML::Node& node) {
        node[name] = var;
    }

    template<typename T>
    static void DeserializeVariable(std::string name, T& var, YAML::Node& node) {
        if (node[name]) {
            var = node[name].as<T>();
        }

    }
    




private:
    template<typename T,typename Func,typename... Args>
    static bool CallFunctionForObject(entt::entity e,Args&&... args) {
        if (!ObjectHandle(e)) {
            return false;
        }
        
        T obj(e);

        [&obj](auto&&... ar) {
            (std::forward<T>(obj).*Func)(std::forward<decltype(ar)>(ar)...);
        };

        return true;

    }

    template<typename Base,typename T, typename Func, typename... Args>
    static bool CallFunctionForObjectWithVirtualBase(entt::entity e, Args&&... args) {
        if (!ObjectHandle(e)) {
            return false;
        }

        T obj(e);

        ((Base*)(&obj))->Func(args...);

        return true;

    }



    
};

namespace HelperClass {
    class Null {
    private:
        int dummy = 0;
    };

}

namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& vec) {
            Node node;
            node["x"] = vec.x;
            node["y"] = vec.y;
            node["z"] = vec.z;
            return node;
        };

        static bool decode(const Node& node, glm::vec3& vec) {
            if (!node.IsMap()) {
                return false;
            }

            vec.x = node["x"].as<float>();
            vec.y = node["y"].as<float>();
            vec.z = node["z"].as<float>();
            return true;

        };

    };
    
    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& vec) {
            Node node;
            node["x"] = vec.x;
            node["y"] = vec.y;
            node["z"] = vec.z;
            node["w"] = vec.w;
            return node;
        };

        static bool decode(const Node& node, glm::vec4& vec) {
            if (!node.IsMap()) {
                return false;
            }

            vec.x = node["x"].as<float>();
            vec.y = node["y"].as<float>();
            vec.z = node["z"].as<float>();
            vec.w = node["w"].as<float>();
            return true;

        };

    };

    template<>
    struct convert<Color> {
        static Node encode(const Color& color) {
            Node node;
            node["r"] = color.AsIntegers().x;
            node["g"] = color.AsIntegers().y;
            node["b"] = color.AsIntegers().z;
            return node;
        };

        static bool decode(const Node& node, Color& color) {
            if (!node.IsMap()) {
                return false;
            }

            color = Color(node["r"].as<int>(),node["g"].as<int>(),node["b"].as<int>());
            return true;

        };

    };


    template<>
    struct convert<entt::entity> {
        static Node encode(const entt::entity& e) {
            Node node;
            node = (uint32_t)e;
            
            return node;
        };

        static bool decode(const Node& node, entt::entity& e) {
            if (node) {
                e = (entt::entity)node.as<uint32_t>();
                return true;
            }
            return false;
        }


    };


};