#pragma once

#include "interfaces.h"

#define OFFSET(name, base, offsetName, returnType) \
returnType name (uintptr_t base) noexcept \
{ \
    return csgo.Read<returnType>(base + offsetName); \
}