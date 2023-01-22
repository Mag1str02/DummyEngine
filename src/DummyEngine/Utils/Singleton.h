#pragma once

struct Unit
{};

#define DEL_BRACKETS(a) SECOND(FIRST a)
#define FIRST(...) FIRST __VA_ARGS__
#define SECOND(...) THIRD(__VA_ARGS__)
#define THIRD(...) ERASE##__VA_ARGS__
#define ERASEFIRST

#define S_METHOD(return_type, name, signature, variables)          \
    static DEL_BRACKETS(return_type) name(DEL_BRACKETS(signature)) \
    {                                                              \
        return Get().I##name(DEL_BRACKETS(variables));             \
    }                                                              \
    DEL_BRACKETS(return_type) I##name(DEL_BRACKETS(signature))

#define S_METHOD_DEF(return_type, name, signature)                  \
    static DEL_BRACKETS(return_type) name(DEL_BRACKETS(signature)); \
    DEL_BRACKETS(return_type) I##name(DEL_BRACKETS(signature));

#define S_METHOD_IMPL(type, return_type, name, signature, variables) \
    DEL_BRACKETS(return_type) type::name(DEL_BRACKETS(signature))    \
    {                                                                \
        return type::Get().I##name(DEL_BRACKETS(variables));         \
    }                                                                \
    DEL_BRACKETS(return_type) type::I##name(DEL_BRACKETS(signature))

#define SINGLETON(type)                    \
private:                                   \
    friend class Singleton<type>;          \
                                           \
public:                                    \
    type(const type&)            = delete; \
    type(type&&)                 = delete; \
    type& operator=(const type&) = delete; \
    type& operator=(type&&)      = delete;

#define SINGLETON_BASE(type)                                 \
    template <> type* Singleton<type>::s_Instance = nullptr; \
    template <> type& Singleton<type>::Get()                 \
    {                                                        \
        return *s_Instance;                                  \
    }

namespace DE
{
    template <typename T> class Singleton
    {
    public:
        static T& Get();

        Singleton(const Singleton&)           = delete;
        Singleton& operator=(const Singleton) = delete;
        Singleton(Singleton&&)                = delete;
        Singleton& operator=(Singleton&&)     = delete;

    protected:
        Singleton(){};
        virtual ~Singleton() {}
        static T* s_Instance;
    };
}  // namespace DE
