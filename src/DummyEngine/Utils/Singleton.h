#pragma once

template <typename T> class Singleton
{
public:
    static T& Get()
    {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&)           = delete;
    Singleton& operator=(const Singleton) = delete;

protected:
    Singleton(){};
    virtual ~Singleton() {}
};
