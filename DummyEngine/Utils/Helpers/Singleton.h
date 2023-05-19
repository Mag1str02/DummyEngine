#pragma once

struct Unit {};

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

#define SINGLETON_BASE(type)                                                                      \
    static const std::string CurrentSingletonName        = #type;                                 \
    template <> type*        Singleton<type>::s_Instance = nullptr;                               \
    template <> type&        Singleton<type>::Get() {                                             \
        DE_ASSERT(s_Instance, "Using " + CurrentSingletonName + " before initialization"); \
        return *s_Instance;                                                                \
    }                                                                                             \
    bool type::Initialized() {                                                                    \
        return s_Instance != nullptr;                                                             \
    }                                                                                             \
    using CurrentSingleton = type

#define DEL_BRACKETS(a) SECOND(FIRST a)
#define FIRST(...) FIRST __VA_ARGS__
#define SECOND(...) THIRD(__VA_ARGS__)
#define THIRD(...) ERASE##__VA_ARGS__
#define ERASEFIRST

#define S_METHOD(return_type, name, signature, variables)            \
    static DEL_BRACKETS(return_type) name(DEL_BRACKETS(signature)) { \
        return Get().I##name(DEL_BRACKETS(variables));               \
    }                                                                \
    DEL_BRACKETS(return_type) I##name(DEL_BRACKETS(signature))

#define S_METHOD_DEF(return_type, name, signature)                  \
    static DEL_BRACKETS(return_type) name(DEL_BRACKETS(signature)); \
    DEL_BRACKETS(return_type) I##name(DEL_BRACKETS(signature));

#define S_METHOD_IMPL(return_type, name, signature, variables)                  \
    DEL_BRACKETS(return_type) CurrentSingleton::name(DEL_BRACKETS(signature)) { \
        return CurrentSingleton::Get().I##name(DEL_BRACKETS(variables));        \
    }                                                                           \
    DEL_BRACKETS(return_type) CurrentSingleton::I##name(DEL_BRACKETS(signature))

#define S_INITIALIZE()                                                                  \
    Unit CurrentSingleton::Initialize() {                                               \
        DE_ASSERT(!s_Instance, "Double initialization of " + CurrentSingletonName);     \
        s_Instance = new CurrentSingleton();                                            \
        DE_ASSERT(s_Instance, "Failed to allocate memory for " + CurrentSingletonName); \
        s_Instance->IInitialize();                                                      \
        return Unit();                                                                  \
    }                                                                                   \
    Unit CurrentSingleton::IInitialize()
#define S_TERMINATE()                                                                          \
    Unit CurrentSingleton::Terminate() {                                                       \
        DE_ASSERT(s_Instance, "Terminating before initialization of " + CurrentSingletonName); \
        s_Instance->ITerminate();                                                              \
        delete s_Instance;                                                                     \
        s_Instance = nullptr;                                                                  \
        return Unit();                                                                         \
    }                                                                                          \
    Unit CurrentSingleton::ITerminate()

namespace DE {
    template <typename T> class Singleton {
    public:
        Singleton(const Singleton&)           = delete;
        Singleton& operator=(const Singleton) = delete;
        Singleton(Singleton&&)                = delete;
        Singleton& operator=(Singleton&&)     = delete;

    protected:
        static T& Get();
        Singleton(){};
        virtual ~Singleton() {}
        static T* s_Instance;
    };
}  // namespace DE
