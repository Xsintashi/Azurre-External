#pragma once

#include "interfaces.h"

#define OFFSET_OLD(name, args, offsetName, returnType) \
returnType name args noexcept \
{ \
    return csgo.Read<returnType>((uintptr_t)this + offsetName); \
}

#define OFFSET(name, offsetName, returnType) \
returnType name (returnType *set = 0x00000) noexcept { \
    if(set == nullptr) \
        return csgo.Read<returnType>((uintptr_t)this + offsetName); \
    csgo.Write<returnType>((uintptr_t)this + offsetName, *set); \
    return *set; \
}