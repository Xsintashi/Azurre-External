#pragma once

#include "Macros.h"
#include "Interfaces.h"
#include "../Core.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/Vector.h"
#include "../Offsets.h"

#include <vector>

enum class Team {
    None = 0,
    Spectators,
    TT,
    CT
};

enum LifeState {
    LIFE_ALIVE,         // alive
    LIFE_DYING,         // playing death animation or still falling off of a ledge waiting to hit ground
    LIFE_DEAD,          // dead. lying still.
    LIFE_RESPAWNABLE,
    LIFE_DISCARDBODY,
};

enum class ClassID {
    Ak47 = 1,
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
    ToneMapController = 69,
    Flashbang = 77,
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
    AWP,
    BaseItem,
    Bizon,
    CSBase,
    CSBaseGun,
    CZ75,
    Elite,
    Famas,
    FiveSeven,
    G3SG1,
    Galil,
    GalilAR,
    Glock,
    P2000,
    M249,
    M4A4,
    M4A1,
    MAC10,
    Mag7,
    MP5,
    MP7,
    MP9,
    Negev,
    NOVA,
    P228,
    P250,
    P90,
    Sawedoff,
    SCAR20,
    Scout,
    SG550,
    SG552,
    SG556,
    Shield,
    SSG08,
    Taser,
    Tec9,
    TMP,
    UMP45,
    USP,
    XM1014,
};

static ClassID GetClassId(int entity) {
    return (ClassID)csgo.Read<int>(csgo.Read<int>(csgo.Read<int>(csgo.Read<int>(entity + 8) + 2 * 4) + 1) + 20);
}

class Entity {
public:
	OFFSET(lastPlaceName, (), Offset::netvars::m_szLastPlaceName, LPVOID)
	OFFSET(money, (), Offset::netvars::m_iAccount, int)
	OFFSET(health, (), Offset::netvars::m_iHealth, int)
	OFFSET(armor, (), Offset::netvars::m_ArmorValue, int)
	OFFSET(teamNumber, (), Offset::netvars::m_iTeamNum, Team)
	OFFSET(flashMaxAlpha, (), Offset::netvars::m_flFlashMaxAlpha, float)
	OFFSET(flashDuration, (), Offset::netvars::m_flFlashDuration, float)
	OFFSET(flags, (), Offset::netvars::m_fFlags, int)
	OFFSET(crosshairID, (), Offset::netvars::m_iCrosshairId, int)
	OFFSET(shotsFired, (), Offset::netvars::m_iShotsFired, int)
	OFFSET(velocity, (), Offset::netvars::m_vecVelocity, Vector)
    OFFSET(aimPunch, (), Offset::netvars::m_aimPunchAngle, Vector)
    OFFSET(origin, (), Offset::netvars::m_vecOrigin, Vector)
    OFFSET(viewOffset, (), Offset::netvars::m_vecViewOffset, Vector)
    OFFSET(hasHelmet, (), Offset::netvars::m_bHasHelmet, bool)
    OFFSET(hasDefuser, (), Offset::netvars::m_bHasDefuser, bool)
    OFFSET(spottedByMask, (), Offset::netvars::m_bSpottedByMask, bool)
    OFFSET(spotted, (), Offset::netvars::m_bSpotted, bool)
    OFFSET(isDefusing, (), Offset::netvars::m_bIsDefusing, bool)
    OFFSET(waitForNoAttack, (), Offset::netvars::m_bWaitForNoAttack, bool)
    OFFSET(boneMatrix, (), Offset::netvars::m_dwBoneMatrix, uintptr_t)
    OFFSET(activeWeapon, (), Offset::netvars::m_hActiveWeapon, int)
    OFFSET(eyeAngleX, (), Offset::netvars::m_angEyeAnglesX, float)
    OFFSET(eyeAngleY, (), Offset::netvars::m_angEyeAnglesY, float)
    OFFSET(duckAmount, (), Offset::netvars::m_flDuckAmount, float)
    OFFSET(duckSpeed, (), Offset::netvars::m_flDuckSpeed, float)
    OFFSET(totalHits, (), Offset::netvars::m_totalHitsOnServer, int)
    OFFSET(lifeState, (), Offset::netvars::m_lifeState, bool)

    OFFSET(dormant, (), Offset::signatures::m_bDormant, bool)

    //Planted C4
    OFFSET(C4Blow, (), Offset::netvars::m_flC4Blow, float)
    OFFSET(timerLength, (), Offset::netvars::m_flTimerLength, float)
    OFFSET(defuseCountDown, (), Offset::netvars::m_flDefuseCountDown, float)
    OFFSET(defuseLength, (), Offset::netvars::m_flDefuseLength, float)
    OFFSET(bombSite, (), Offset::netvars::m_nBombSite, int)
    OFFSET(bombDefuser, (), Offset::netvars::m_hBombDefuser, int)
    OFFSET(BombTicking, (), Offset::netvars::m_bBombTicking, bool)
    OFFSET(BombDefused, (), Offset::netvars::m_bBombDefused, bool)

    Vector eyeAngles() noexcept {
        return { this->eyeAngleX(), this->eyeAngleY(), 0.f};
    }

    //Weapon
    OFFSET(clip, (), Offset::netvars::m_iClip1, int)
    OFFSET(isInReload, (), Offset::netvars::m_bInReload, bool)

    bool isDead() noexcept {
        return this->health() < 1;
    }

    bool isSameTeam() noexcept {
        return this->teamNumber() == localPlayer->teamNumber();
    }

    bool isAlive() noexcept {
        return !this->isDead();
    }

    bool isWeapon() noexcept {
        const ClassID classID = GetClassId((uintptr_t)this);
        return (classID == ClassID::Ak47 || classID == ClassID::Deagle || (classID > ClassID::Aug && classID < ClassID::XM1014));
    }

    short getWeaponIDFromPlayer() noexcept {
        int weaponIndex = csgo.Read<int>((uintptr_t)this + Offset::netvars::m_hActiveWeapon) & 0xFFF;
        if (!weaponIndex)
            return 0;

        const auto& activeWeapon = csgo.Read<Entity*>(IClient.address + Offset::signatures::dwEntityList + (weaponIndex - 1) * 0x10);
        if (!activeWeapon)
            return 0;

        return csgo.Read<short>((uintptr_t)activeWeapon + Offset::netvars::m_iItemDefinitionIndex);
    }

    short getWeaponID() noexcept {
        return csgo.Read<short>((uintptr_t)this + Offset::netvars::m_iItemDefinitionIndex);
    }

};

static Entity* getEntity(int idx) {
    const auto& entity = csgo.Read<Entity*>(IClient.address + Offset::signatures::dwEntityList + idx * 0x10);
    if (!entity) return 0;
    return entity;
}

struct PlayerData {
	Entity* entity;
	int unsigned idx;
    const char* steamID;
	bool isBot;
	std::string name;
	int health;
	int armor;
    bool hasHelmet;
    bool hasDefuser;
	int teamNumber;
	int money;
	int weaponID;
    std::string placename;
    int rank;
    int wins;
};

inline std::vector<PlayerData> entityData;