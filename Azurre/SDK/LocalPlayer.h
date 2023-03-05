#pragma once

class Entity;

class LocalPlayer {
public:
    void init(uintptr_t entity) noexcept
    {
        localEntity = entity;
    }

    constexpr operator bool() noexcept
    {
        return localEntity != 0;
    }

    constexpr auto operator->() noexcept
    {
        return localEntity;
    }

    constexpr auto get() noexcept
    {
        return localEntity;
    }
private:
    uintptr_t localEntity = 0;
};


inline LocalPlayer localPlayer;