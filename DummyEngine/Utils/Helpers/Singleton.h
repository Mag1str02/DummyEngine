#pragma once

#include "DummyEngine/Utils/Debug/Assert.h"

#define SINGLETON(type)                           \
private:                                          \
    friend class Singleton<type>;                 \
                                                  \
public:                                           \
    type()                             = default; \
    ~type()                            = default; \
    type(const type&)                  = delete;  \
    type(type&&)                       = delete;  \
    type&       operator=(const type&) = delete;  \
    type&       operator=(type&&)      = delete;  \
    static bool Initialized();                    \
    S_METHOD_DEF(Unit, Initialize, ());           \
    S_METHOD_DEF(Unit, Terminate, ());

#define SINGLETON_BASE(type)                                                                 \
    static const std::string CurrentSingletonName       = #type;                             \
    template <> type*        Singleton<type>::gInstance = nullptr;                           \
    template <> type&        Singleton<type>::GetInstance() {                                \
        DE_ASSERT(gInstance, "Using {} before initialization", CurrentSingletonName); \
        return *gInstance;                                                            \
    }                                                                                        \
    bool type::Initialized() {                                                               \
        return gInstance != nullptr;                                                         \
    }                                                                                        \
    using CurrentSingleton = type

#define DEL_BRACKETS(a) SECOND(FIRST a)
#define FIRST(...) FIRST __VA_ARGS__
#define SECOND(...) THIRD(__VA_ARGS__)
#define THIRD(...) ERASE##__VA_ARGS__
#define ERASEFIRST

#define S_METHOD(return_type, name, signature, variables)            \
    static DEL_BRACKETS(return_type) name(DEL_BRACKETS(signature)) { \
        return GetInstance().I##name(DEL_BRACKETS(variables));       \
    }                                                                \
    DEL_BRACKETS(return_type) I##name(DEL_BRACKETS(signature))

#define S_METHOD_DEF(return_type, name, signature)                  \
    static DEL_BRACKETS(return_type) name(DEL_BRACKETS(signature)); \
    DEL_BRACKETS(return_type) I##name(DEL_BRACKETS(signature));

#define S_METHOD_IMPL(return_type, name, signature, variables)                   \
    DEL_BRACKETS(return_type) CurrentSingleton::name(DEL_BRACKETS(signature)) {  \
        return CurrentSingleton::GetInstance().I##name(DEL_BRACKETS(variables)); \
    }                                                                            \
    DEL_BRACKETS(return_type) CurrentSingleton::I##name(DEL_BRACKETS(signature))

#define S_INITIALIZE()                                                                  \
    Unit CurrentSingleton::Initialize() {                                               \
        DE_ASSERT(!gInstance, "Double initialization of {}", CurrentSingletonName);     \
        gInstance = new CurrentSingleton();                                             \
        DE_ASSERT(gInstance, "Failed to allocate memory for {}", CurrentSingletonName); \
        gInstance->IInitialize();                                                       \
        return Unit();                                                                  \
    }                                                                                   \
    Unit CurrentSingleton::IInitialize()
#define S_TERMINATE()                                                                          \
    Unit CurrentSingleton::Terminate() {                                                       \
        DE_ASSERT(gInstance, "Terminating before initialization of {}", CurrentSingletonName); \
        gInstance->ITerminate();                                                               \
        delete gInstance;                                                                      \
        gInstance = nullptr;                                                                   \
        return Unit();                                                                         \
    }                                                                                          \
    Unit CurrentSingleton::ITerminate()

namespace DummyEngine {

    struct Unit {};

    template <typename T> class Singleton {
    public:
        Singleton(const Singleton&)           = delete;
        Singleton& operator=(const Singleton) = delete;
        Singleton(Singleton&&)                = delete;
        Singleton& operator=(Singleton&&)     = delete;

    protected:
        static T& GetInstance();

        Singleton()          = default;
        virtual ~Singleton() = default;
        static T* gInstance;
    };

}  // namespace DummyEngine
