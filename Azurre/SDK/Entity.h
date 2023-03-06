#pragma once

#include "Macros.h"
#include "Interfaces.h"
#include "../Core.h"
#include "../SDK/Vector.h"
#include "../Offsets.h"

#include <vector>

enum class ClassID {
    BaseCSGrenadeProjectile = 9,
    BreachChargeProjectile = 29,
    BumpMineProjectile = 33,
    C4,
    Chicken = 36,
    CSPlayer = 40,
    CSPlayerResource,
    CSRagdoll = 42,
    Deagle = 46,
    DecoyProjectile = 48,
    Drone,
    Dronegun,
    DynamicProp = 52,
    EconEntity = 53,
    EconWearable,
    Hostage = 97,
    Inferno = 100,
    Healthshot = 104,
    Cash,
    Knife = 107,
    KnifeGG,
    MolotovProjectile = 114,
    AmmoBox = 125,
    LootCrate,
    RadarJammer,
    WeaponUpgrade,
    PlantedC4,
    PropDoorRotating = 143,
    SensorGrenadeProjectile = 153,
    SmokeGrenadeProjectile = 157,
    SnowballPile = 160,
    SnowballProjectile,
    Tablet = 172,
    Aug = 232,
    Awp,
    Elite = 239,
    FiveSeven = 241,
    G3sg1,
    Glock = 245,
    P2000,
    P250 = 258,
    Scar20 = 261,
    Sg553 = 265,
    Ssg08 = 267,
    Tec9 = 269
};

class Entity {
public:
	OFFSET(lastPlaceName, (), Offset::netvars::m_szLastPlaceName, LPVOID)
	OFFSET(money, (), Offset::netvars::m_iAccount, int)
	OFFSET(health, (), Offset::netvars::m_iHealth, int)
	OFFSET(armor, (), Offset::netvars::m_ArmorValue, int)
	OFFSET(teamNumber, (), Offset::netvars::m_iTeamNum, int)
	OFFSET(flashMaxAlpha, (), Offset::netvars::m_flFlashMaxAlpha, float)
	OFFSET(flashDuration, (), Offset::netvars::m_flFlashDuration, float)
	OFFSET(flags, (), Offset::netvars::m_fFlags, int)
	OFFSET(crosshairID, (), Offset::netvars::m_iCrosshairId, int)
	OFFSET(velocity, (), Offset::netvars::m_vecVelocity, Vector)

    bool isDead() noexcept {
        return this->health() < 1;
    }

    bool isAlive() noexcept {
        return !this->isDead();
    }

};

static Entity* getEntity(int idx) {
	const auto& entity = csgo.Read<Entity*>(IClient + Offset::signatures::dwEntityList + idx * 0x10);
	if (!entity) return 0;
	return entity;
}

static ClassID GetClassId(int EntBase)
{
	return (ClassID)csgo.Read<int>(csgo.Read<int>(csgo.Read<int>(csgo.Read<int>(EntBase + 8) + 2 * 4) + 1) + 20);
}

struct PlayerData {
	Entity* entity;
	int unsigned idx;
	unsigned long steamID;
	bool isBot;
	std::string name;
	int health;
	int armor;
	int teamNumber;
	int money;
};

inline std::vector<PlayerData> entityData;