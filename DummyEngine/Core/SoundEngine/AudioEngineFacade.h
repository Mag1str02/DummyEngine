#ifndef DUMMYENGINE_AUDIOENGINEFACADE_H
#define DUMMYENGINE_AUDIOENGINEFACADE_H

#pragma once
#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "AL/al.h"
#include "AL/alc.h"

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

bool check_al_errors(const std::string& filename, std::uint_fast32_t line);
bool check_alc_errors(const std::string& filename, std::uint_fast32_t line, ALCdevice* device);

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                std::uint_fast32_t line,
                alFunction function,
                Params... params)
    ->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>;

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                std::uint_fast32_t line,
                alFunction function,
                Params... params)
    ->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>;

template<typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename,
                 std::uint_fast32_t line,
                 alcFunction function,
                 ALCdevice* device,
                 Params... params)
    ->typename std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool>;

template<typename alcFunction, typename ReturnType, typename... Params>
auto alcCallImpl(const char* filename,
                 std::uint_fast32_t line,
                 alcFunction function,
                 ReturnType& returnValue,
                 ALCdevice* device,
                 Params... params)
    ->typename std::enable_if_t<!std::is_same_v<void,decltype(function(params...))>,bool>;

#endif  // DUMMYENGINE_AUDIOENGINEFACADE_H
