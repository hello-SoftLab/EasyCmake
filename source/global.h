#pragma once

#include "general/includes.h"
#include "general/color.h"
#include "general/helpers.h"


using namespace std;




#ifdef NDEBUG
#define DEBUG_LOG(x)
#define DEBUG_WARN(x)
#define DEBUG_ERROR(x)
#else
#define DEBUG_LOG(x) cout << "LOG: " << x << endl << " In Function " << std::source_location::current().function_name() << endl <<   " At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
#define DEBUG_WARN(x) cout << "WARNING: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
#define DEBUG_ERROR(x) cout << "ERROR! -> " << x  << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl; __debugbreak()
#endif

static void ClearGLErrors() {
    while (glGetError() != GL_NO_ERROR) {
        ;
    }
}

static bool GetGLError(int line, std::string file) {
    GLenum code = glGetError();
    if (code != GL_NO_ERROR) {
        cout << "OpenGL error '" << gluErrorString(code) << "' \nAt line: " << line << " \nIn file: " << file << endl;
        return true;
    }
    return false;
}

#ifdef NDEBUG
#define GL_CALL(x) x 
#define GL_CALL_WITH_RESULT(var,x) var = x 
#else
#define GL_CALL(x) ClearGLErrors(); x; if(GetGLError(__LINE__,__FILE__)) {;}
#define GL_CALL_WITH_RESULT(var,x) ClearGLErrors(); var = x; if(GetGLError(__LINE__,__FILE__)) {;}
#endif


struct EventReceiver;
struct EventReceiverAttachmentProperties {
    std::function<void(EventReceiver*)> m_CopyFunc;
    std::function<void(void*, size_t)> m_DeleteFunc;
};


template<typename T>
struct EventLauncher;
template<typename T>
struct FunctionSink;

struct EventReceiver {

    ~EventReceiver() {
        if (m_SubscribedEvents.size() == 0) {
            return;
        }
        auto it = m_SubscribedEvents.begin();
        while (it != m_SubscribedEvents.end()) {
            it->second.m_DeleteFunc(it->first, std::hash<void*>()((void*)this));
            it = m_SubscribedEvents.begin();
        }
    }

    EventReceiver& operator=(const EventReceiver& other) {
        for (auto& [ptr, prop] : other.m_SubscribedEvents) {
            prop.m_CopyFunc(this);
        }
        return *this;
    }

private:
    std::map<void*, EventReceiverAttachmentProperties> m_SubscribedEvents;

    template<typename T>
    friend struct FunctionSink;
    template<typename T>
    friend struct EventLauncher;
};


template<typename R, typename... Args>
struct EventLauncher<R(Args...)> {

    EventLauncher() {
    };

    ~EventLauncher() {
    }


    bool DisconnectReceiver(size_t hash) {
        if (m_Receivers.find(hash) != m_Receivers.end()) {
            m_Receivers.erase(hash);
            return true;
        }
        return false;
    };


    void EmitEvent(Args... args) {

        for (auto& [handle, func] : m_Receivers) {
            if (func) {
                (*func.get())(args...);
            }
        }

    };

    EventLauncher<R(Args...)>& operator=(const EventLauncher<R(Args...)>& other) {
        return *this;
    }

private:
    std::unordered_map<size_t, std::shared_ptr<std::function<R(Args...)>>> m_Receivers;
    size_t m_MyHash = std::hash<void*>()((void*)this);

    template<typename T>
    friend struct FunctionSink;

};







template<typename R, typename... Args>
struct FunctionSink<R(Args...)> {
    FunctionSink(EventLauncher<R(Args...)>& sink) : m_Master(&sink) {};

    size_t Connect(std::function<R(Args...)> windowFunc) {
        static int count = 1;
        size_t hash = std::hash<int>()(count);
        count++;

        std::function<R(Args...)>* func = new std::function<R(Args...)>(windowFunc);
        m_Master->m_Receivers[hash] = std::make_shared<std::function<R(Args...)>>(*func);
        return hash;

    }

    void Connect(EventReceiver* key, std::function<R(EventReceiver*, Args...)> windowFunc) {

        size_t hash = std::hash<void*>()((void*)key);
        auto deleter = [=](std::function<R(Args...)>* func) {
            key->m_SubscribedEvents.erase((void*)m_Master);
            delete func;
        };

        auto func = new std::function<R(Args...)>([=](auto... args) {
            windowFunc(key, args...);
            });
        m_Master->m_Receivers[hash] = std::shared_ptr<std::function<R(Args...)>>(func, deleter);

        EventReceiverAttachmentProperties prop;
        prop.m_CopyFunc = [=](EventReceiver* receiver) {
            FunctionSink<R(Args...)>(*m_Master).Connect(receiver, windowFunc);
        };

        prop.m_DeleteFunc = [](void* ptr, size_t hash) {
            ((EventLauncher<R(Args...)>*)(ptr))->DisconnectReceiver(hash);
        };

        key->m_SubscribedEvents[(void*)m_Master] = std::move(prop);

    };

    bool Disconnect(size_t hashKey) {
        return m_Master->DisconnectReceiver(hashKey);
    }

    bool Disconnect(EventReceiver* key) {
        if (key == nullptr) {
            return false;
        }
        size_t hash = std::hash<void*>()((void*)key);
        if (m_Master->m_Receivers.find(hash) != m_Master->m_Receivers.end()) {
            m_Master->DisconnectReceiver(hash);
        }
        else {
            return false;
        }
        return true;
    }

private:



    EventLauncher<R(Args...)>* m_Master;


};
