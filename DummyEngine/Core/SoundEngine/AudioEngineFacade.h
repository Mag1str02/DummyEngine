#pragma once
#ifndef DUMMYENGINE_AUDIOENGINEFACADE_H
#define DUMMYENGINE_AUDIOENGINEFACADE_H
#include "AL/al.h"
#include "AL/alc.h"
#include "DummyEngine/Utils/Base/STDIncludes.h"
#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

bool check_al_errors(const std::string& filename, const std::uint_fast32_t line);

template <typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params) ->
    typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> {
    auto ret = function(std::forward<Params>(params)...);
    check_al_errors(filename, line);
    return ret;
}

template <typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params) ->
    typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
    function(std::forward<Params>(params)...);
    return check_al_errors(filename, line);
}

bool check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);

template <typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename, const std::uint_fast32_t line, alcFunction function, ALCdevice* device, Params... params) ->
    typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
    function(std::forward<Params>(params)...);
    return check_alc_errors(filename, line, device);
}

template <typename alcFunction, typename ReturnType, typename... Params>
auto alcCallImpl(
    const char* filename, const std::uint_fast32_t line, alcFunction function, ReturnType& returnValue, ALCdevice* device, Params... params) ->
    typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool> {
    returnValue = function(std::forward<Params>(params)...);
    return check_alc_errors(filename, line, device);
}
#endif  // DUMMYENGINE_AUDIOENGINEFACADE_H
