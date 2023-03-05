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
	const auto& weapons = csgo.Read<std::array<unsigned long, 8>>(localPlayer + Offset::netvars::m_hMyWeapons);

	for (const auto& handle : weapons) {
		const auto& weapon = csgo.Read<uintptr_t>((IClient + Offset::signatures::dwEntityList + (handle & 0xFFF) * 0x10) - 0x10);

		if (!weapon) continue;

		const short weaponIndex = csgo.Read<short>(weapon + Offset::netvars::m_iItemDefinitionIndex);
		//if (weaponIndex == WeaponID::Knife || weaponIndex == WeaponID::KnifeT) {
		//	csgo.Write<short>(weapon + Offset::netvars::m_iItemDefinitionIndex, 508);
		//	csgo.Write<int>(weapon + Offset::netvars::m_nViewModelIndex, 391);
		//	csgo.Write<int>(localPlayer + Offset::netvars::m_iWorldModelIndex, 392);
		//	return;
		//}

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
