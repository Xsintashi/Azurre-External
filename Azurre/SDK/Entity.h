#pragma once

#include "Macros.h"
#include "Interfaces.h"
#include "../Core.h"
#include "LocalPlayer.h"
#include "Vector.h"
#include "WeaponID.h"
#include "../Offsets.h"

#include <vector>

enum class Team {
    None = 0,
    Spectators,
    TT,
    CT
};

enum FlagsState {
    ONGROUND			 = (1 << 0), // On the floor
    DUCKING				 = (1 << 1), // Crouching
    WATERJUMP			 = (1 << 3),
    ONTRAIN				 = (1 << 4),
    INRAIN				 = (1 << 5),
    FROZEN				 = (1 << 6), // Frozen
    ATCONTROLS			 = (1 << 7),
    CLIENT				 = (1 << 8),
    FAKECLIENT			 = (1 << 9),
    INWATER				 = (1 << 10), // In water
    HIDEHUD_SCOPE		 = (1 << 11)
};

enum LifeState {
    LIFE_ALIVE,         // alive
    LIFE_DYING,         // playing death animation or still falling off of a ledge waiting to hit ground
    LIFE_DEAD,          // dead. lying still.
    LIFE_RESPAWNABLE,
    LIFE_DISCARDBODY,
};

enum class ObsMode {
    None = 0,
    Deathcam,
    Freezecam,
    Fixed,
    InEye,
    Chase,
    Roaming
};

enum class ClassID {
    Ak47 = 1,
    BaseCSGrenadeProjectile = 9,
    WeaponWorldModel = 23, // Wrench, Axe, Hammer
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

static ClassID GetClassId(Entity* entity) {
    return (ClassID)mem.Read<int>(mem.Read<int>(mem.Read<int>(mem.Read<int>((uintptr_t)entity + 8) + 2 * 4) + 1) + 20);
}

class Entity {
public:
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
	OFFSET(stamina, (), Offset::netvars::m_flStamina, float)
	OFFSET(velocityModifier, (), Offset::netvars::m_flVelocityModifier, float)
    OFFSET(aimPunch, (), Offset::netvars::m_aimPunchAngle, Vector)
    OFFSET(origin, (), Offset::netvars::m_vecOrigin, Vector)
    OFFSET(viewOffset, (), Offset::netvars::m_vecViewOffset, Vector)
    OFFSET(hasHelmet, (), Offset::netvars::m_bHasHelmet, bool)
    OFFSET(hasDefuser, (), Offset::netvars::m_bHasDefuser, bool)
    OFFSET(spottedByMask, (), Offset::netvars::m_bSpottedByMask, std::uint32_t)
    OFFSET(spotted, (), Offset::netvars::m_bSpotted, bool)
    OFFSET(isDefusing, (), Offset::netvars::m_bIsDefusing, bool)
    OFFSET(waitForNoAttack, (), Offset::netvars::m_bWaitForNoAttack, bool)
    OFFSET(boneMatrix, (), Offset::netvars::m_dwBoneMatrix, DWORD)
    OFFSET(activeWeapon, (), Offset::netvars::m_hActiveWeapon, int)
    OFFSET(eyeAngleX, (), Offset::netvars::m_angEyeAnglesX, float)
    OFFSET(eyeAngleY, (), Offset::netvars::m_angEyeAnglesY, float)
    OFFSET(duckAmount, (), Offset::netvars::m_flDuckAmount, float)
    OFFSET(duckSpeed, (), Offset::netvars::m_flDuckSpeed, float)
    OFFSET(totalHits, (), Offset::netvars::m_totalHitsOnServer, int)
    OFFSET(observerTarget, (), Offset::netvars::m_hObserverTarget, int)
    OFFSET(observerMode, (), Offset::netvars::m_iObserverMode, ObsMode)
    OFFSET(lifeState, (), Offset::netvars::m_lifeState, LifeState)
    OFFSET(isScoped, (), Offset::netvars::m_bIsScoped, bool)
    OFFSET(modelIndex, (), Offset::netvars::m_nModelIndex, int)
    OFFSET(fov, (), Offset::netvars::m_iFOV, int)
    OFFSET(fovStart, (), Offset::netvars::m_iFOVStart, int)
    OFFSET(lby, (), Offset::netvars::m_flLowerBodyYawTarget, float)
    OFFSET(dormant, (), Offset::signatures::m_bDormant, bool)
    OFFSET(gunGameImmunity, (), Offset::netvars::m_bGunGameImmunity, bool)
    OFFSET(survivalTeam, (), Offset::netvars::m_nSurvivalTeam, int)
    OFFSET(tickBase, (), Offset::netvars::m_nTickBase, int)
    OFFSET(nextAttack, (), Offset::netvars::m_flNextAttack, float)
    OFFSET(nextPrimaryAttack, (), Offset::netvars::m_flNextPrimaryAttack, float)
    OFFSET(getWeaponID, (), Offset::netvars::m_iItemDefinitionIndex, WeaponID)

    //Planted C4
    OFFSET(C4Blow, (), Offset::netvars::m_flC4Blow, float)
    OFFSET(timerLength, (), Offset::netvars::m_flTimerLength, float)
    OFFSET(defuseCountDown, (), Offset::netvars::m_flDefuseCountDown, float)
    OFFSET(defuseLength, (), Offset::netvars::m_flDefuseLength, float)
    OFFSET(bombSite, (), Offset::netvars::m_nBombSite, int)
    OFFSET(bombDefuser, (), Offset::netvars::m_hBombDefuser, int)
    OFFSET(bombTicking, (), Offset::netvars::m_bBombTicking, bool)
    OFFSET(bombDefused, (), Offset::netvars::m_bBombDefused, bool)

    Vector eyeAngles() noexcept {
        return { this->eyeAngleX(), this->eyeAngleY(), 0.f};
    }

    //Weapon
    OFFSET(clip, (), Offset::netvars::m_iClip1, int)
    OFFSET(isInReload, (), Offset::netvars::m_bInReload, bool)
    OFFSET(accuracyPenalty, (), Offset::netvars::m_fAccuracyPenalty, float)

    Vector bonePosition(int bone) {
        const auto buff = Vector{
                    mem.Read<float>(this->boneMatrix() + 0x30 * bone + 0x0C),
                    mem.Read<float>(this->boneMatrix() + 0x30 * bone + 0x1C),
                    mem.Read<float>(this->boneMatrix() + 0x30 * bone + 0x2C)
        };

        return buff;
    }

    bool isValid() {
        return (this != nullptr);
    }

    bool isDead() noexcept {
        return this->health() < 1;
    }

    bool isVisible() noexcept {
        return (localPlayer && localPlayer->isAlive()) ?  (bspParser->isVisible(localPlayer->origin(), this->origin()) || (this->spottedByMask() & (1 << localPlayerIndex))) : false;
    }

    bool isSameTeam() noexcept {
        if (isDangerZoneModePlayed) {
            return this->survivalTeam() == localPlayer->survivalTeam();
        }
        return this->teamNumber() == localPlayer->teamNumber();
    }

    bool isAlive() noexcept {
        return !this->isDead();
    }

    bool isPlayer() noexcept {
        const ClassID classID = GetClassId(this);
        return classID == ClassID::CSPlayer;
    }

    bool isWeapon() noexcept {
        const ClassID classID = GetClassId(this);
        return (classID == ClassID::Ak47 || classID == ClassID::Deagle || (classID > ClassID::Aug && classID < ClassID::XM1014));
    }

    bool isFlashed() noexcept
    {
        return this->flashDuration() > 75.f;
    }

    short getWeaponIDFromPlayer() noexcept {
        int weaponIndex = mem.Read<int>(this + Offset::netvars::m_hActiveWeapon) & ENT_ENTRY_MASK;
        if (!weaponIndex)
            return 0;

        const auto& activeWeapon = mem.Read<Entity*>(IClient.address + Offset::signatures::dwEntityList + (weaponIndex - 1) * 0x10);
        if (!activeWeapon)
            return 0;

        return mem.Read<short>(activeWeapon + Offset::netvars::m_iItemDefinitionIndex);
    }

    Entity* getActiveWeapon() noexcept {
        int weaponIndex = mem.Read<int>(this + Offset::netvars::m_hActiveWeapon) & ENT_ENTRY_MASK;

        if (!weaponIndex) return 0;

        const auto& activeWeapon = mem.Read<Entity*>(IClient.address + Offset::signatures::dwEntityList + (weaponIndex - 1) * 0x10);
       
        return activeWeapon;
    }

    bool isWeaponRifleSniper() noexcept {
        return this->getWeaponID() == WeaponID::Ssg08 || this->getWeaponID() == WeaponID::Awp || this->getWeaponID() == WeaponID::Scar20 || this->getWeaponID() == WeaponID::G3SG1;
    }

    std::string getPlaceName() noexcept {
        char placeName[18];
        ReadProcessMemory(mem.processHandle, (LPCVOID)(this + Offset::netvars::m_szLastPlaceName), &placeName, 18, NULL);

        return placeName;
    }

};

static Entity* getEntity(int idx) {
    const auto& entity = mem.Read<Entity*>(IClient.address + Offset::signatures::dwEntityList + idx * 0x10);
    if (!entity) return 0;
    return entity;
}
