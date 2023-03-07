#include "SkinChanger.h"
#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/SkinID.h"
#include "../SDK/WeaponID.h"

struct WeaponSkin {
	int skinID = 0;

	float wear = 0.1f;
	int seed = 0;
	int quality = 0;

	bool enableStatrak = false;
	int statTrak = 0;

	const char* nameTag;
};

WeaponSkin weaponData[WeaponID::MAX_VALUE];

bool pleaseUpdate = false;

void Skin::add(int weaponID, int skinID, float wear, int seed, int statTrak, int quality, std::string nameTag) {
	weaponData[weaponID].skinID = skinID;
	weaponData[weaponID].wear = wear;
	weaponData[weaponID].seed = seed;
	weaponData[weaponID].enableStatrak = statTrak > 1 ? true : false;
	weaponData[weaponID].statTrak = statTrak;
	weaponData[weaponID].quality = quality;
	weaponData[weaponID].nameTag = nameTag.c_str();

	pleaseUpdate = true;
}

void Skin::update() {
	const auto& weapons = csgo.Read<std::array<unsigned long, 8>>(localPlayer.get() + Offset::netvars::m_hMyWeapons);

	for (const auto& handle : weapons) {
		const auto& weapon = csgo.Read<uintptr_t>((IClient + Offset::signatures::dwEntityList + (handle & 0xFFF) * 0x10) - 0x10);

		if (!weapon) continue;

		const short weaponIndex = csgo.Read<short>(weapon + Offset::netvars::m_iItemDefinitionIndex);

		if (const int paint = weaponData[weaponIndex].skinID) {
			const bool shouldUpdate = csgo.Read<int32_t>(weapon + Offset::netvars::m_nFallbackPaintKit) != paint || pleaseUpdate;

			csgo.Write<int32_t>(weapon + Offset::netvars::m_iItemIDHigh, -1);

			csgo.Write<int32_t>(weapon + Offset::netvars::m_nFallbackPaintKit, paint);
			csgo.Write<float>(weapon + Offset::netvars::m_flFallbackWear, weaponData[weaponIndex].wear);

			csgo.Write<int32_t>(weapon + Offset::netvars::m_nFallbackSeed, weaponData[weaponIndex].seed);
			csgo.Write<int>(weapon + Offset::netvars::m_iEntityQuality, weaponData[weaponIndex].quality);

			if (weaponData[weaponIndex].enableStatrak)
				csgo.Write<int32_t>(weapon + Offset::netvars::m_nFallbackStatTrak, weaponData[weaponIndex].statTrak);

			//if(strcmp(weaponData[weaponIndex].nameTag, "\0"))
			//	csgo.Write<LPVOID>(weapon + Offset::netvars::m_szCustomName, (LPVOID)weaponData[weaponIndex].nameTag);

			csgo.Write<int32_t>(weapon + Offset::netvars::m_iAccountID, csgo.Read<int32_t>(weapon + Offset::netvars::m_OriginalOwnerXuidLow));

			if (shouldUpdate) {
				csgo.Write<std::int32_t>(csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState) + 0x174, -1);
				pleaseUpdate = false;
			}

		}
	}
	
}

const char* Skin::getWeaponIDName(short ID) noexcept {

    switch (ID) {
    default: return "?";
    case WeaponID::Ak47: return "AK-47";
    case WeaponID::Aug: return "AUG";
    case WeaponID::Awp: return "AWP";
    case WeaponID::Cz75a: return "CZ75 Auto";
    case WeaponID::Deagle: return "Desert Eagle";
    case WeaponID::Elite: return "Dual Berettas";
    case WeaponID::Famas: return "FAMAS";
    case WeaponID::Fiveseven: return "Five-SeveN";
    case WeaponID::G3SG1: return "G3SG1";
    case WeaponID::GalilAr: return "Galil AR";
    case WeaponID::Glock: return "Glock-18";
    case WeaponID::M249: return "M249";
    case WeaponID::M4a1_s: return "M4A1-S";
    case WeaponID::M4A1: return "M4A4";
    case WeaponID::Mac10: return "MAC-10";
    case WeaponID::Mag7: return "MAG-7";
    case WeaponID::Mp5sd: return "MP5-SD";
    case WeaponID::Mp7: return "MP7";
    case WeaponID::Mp9: return "MP9";
    case WeaponID::Negev: return "Negev";
    case WeaponID::Nova: return "Nova";
    case WeaponID::Hkp2000: return "P2000";
    case WeaponID::P250: return "P250";
    case WeaponID::P90: return "P90";
    case WeaponID::Bizon: return "PP-Bizon";
    case WeaponID::Revolver: return "R8 Revolver";
    case WeaponID::Sawedoff: return "Sawed-Off";
    case WeaponID::Scar20: return "SCAR-20";
    case WeaponID::Ssg08: return "SSG 08";
    case WeaponID::Sg553: return "SG 553";
    case WeaponID::Tec9: return "Tec-9";
    case WeaponID::Ump45: return "UMP-45";
    case WeaponID::Usp_s: return "USP-S";
    case WeaponID::Xm1014: return "XM1014";

    case WeaponID::Bayonet: return "Bayonet Knife";
    case WeaponID::Bowie: return "Bowie Knife";
    case WeaponID::Butterfly: return "Butterfly Knife";
    case WeaponID::ClassicKnife: return "Classic Knife";
    case WeaponID::Falchion: return "Falchion Knife";
    case WeaponID::Flip: return "Flip Knife";
    case WeaponID::Gut: return "Gut Knife";
    case WeaponID::Huntsman: return "Huntsman Knife";
    case WeaponID::Karambit: return "Karambit";
    case WeaponID::M9Bayonet: return "M9 Bayonet Knife";
    case WeaponID::Navaja: return "Navaja";
    case WeaponID::NomadKnife: return "Nomad Knife";
    case WeaponID::Paracord: return "Paracord Knife";
    case WeaponID::Daggers: return "Shadow Daggers";
    case WeaponID::SkeletonKnife: return "Skeleton Knife";
    case WeaponID::Stiletto: return "Stiletto";
    case WeaponID::SurvivalKnife: return "Survival Knife";
    case WeaponID::Talon: return "Talon Knife";
    case WeaponID::Ursus: return "Ursus Knife";
    case WeaponID::GoldenKnife: return "Gold Knife";
    case WeaponID::Knife:
    case WeaponID::KnifeT: return "Knife";
    case WeaponID::Shield: return "Shield";
    case WeaponID::Taser: return "Zeus x27";

    case WeaponID::Molotov: return "Molotov";
    case WeaponID::IncGrenade: return "Incendiary Grenade";
    case WeaponID::HeGrenade: return "Explosiveu Grenade";
    case WeaponID::SmokeGrenade: return "Smoke Grenade";
    case WeaponID::Decoy: return "Decoy";
    case WeaponID::Flashbang: return "Flashbang";
    }
}
