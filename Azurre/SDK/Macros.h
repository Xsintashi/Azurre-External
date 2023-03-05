#pragma once

#include "interfaces.h"

#define OFFSET(name, args, offsetName, returnType) \
returnType name args noexcept \
{ \
    return csgo.Read<returnType>((uintptr_t)this + offsetName); \
}