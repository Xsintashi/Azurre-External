#pragma once

#include <cstdint>

class Entity;

class LocalPlayer {
public:
    void init(Entity* entity) noexcept
    {
        localEntity = entity;
    }

    constexpr operator bool() noexcept
    {
        return localEntity != nullptr;
    }

    constexpr auto operator->() noexcept
    {
        return localEntity;
    }

    constexpr auto get() noexcept
    {
        return (uintptr_t)localEntity;
    }
private:
    Entity* localEntity = 0;
};


inline LocalPlayer localPlayer;