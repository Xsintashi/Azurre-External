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

	bool enableStatrak = false;
	int statTrak = 0;
};

WeaponSkin weaponData[WeaponID::MAX_VALUE];

void Skin::add(int weaponID, int skinID, float wear, int seed, int statTrak) {
	weaponData[weaponID].skinID = skinID;
	weaponData[weaponID].wear = wear;
	weaponData[weaponID].seed = seed;
	weaponData[weaponID].enableStatrak = statTrak > 1 ? true : false;
	weaponData[weaponID].statTrak = statTrak;
}

void Skin::update() {
	const auto& weapons = csgo.Read<std::array<unsigned long, 8>>(localPlayer.get() + Offset::netvars::m_hMyWeapons);

	for (const auto& handle : weapons) {
		const auto& weapon = csgo.Read<uintptr_t>((IClient + Offset::signatures::dwEntityList + (handle & 0xFFF) * 0x10) - 0x10);

		if (!weapon) continue;

		const short weaponIndex = csgo.Read<short>(weapon + Offset::netvars::m_iItemDefinitionIndex);

		if (const int paint = weaponData[weaponIndex].skinID) {
			const bool shouldUpdate = csgo.Read<int32_t>(weapon + Offset::netvars::m_nFallbackPaintKit) != paint;

			csgo.Write<int32_t>(weapon + Offset::netvars::m_iItemIDHigh, -1);

			csgo.Write<int32_t>(weapon + Offset::netvars::m_nFallbackPaintKit, paint);
			csgo.Write<float>(weapon + Offset::netvars::m_flFallbackWear, weaponData[weaponIndex].wear);

			csgo.Write<int32_t>(weapon + Offset::netvars::m_nFallbackSeed, weaponData[weaponIndex].seed);

			if (weaponData[weaponIndex].enableStatrak) {
				csgo.Write<int32_t>(weapon + Offset::netvars::m_nFallbackStatTrak, weaponData[weaponIndex].statTrak);
			}

			const int32_t accountID = csgo.Read<int32_t>(localPlayer + Offset::netvars::m_OriginalOwnerXuidLow);;
			csgo.Write<int32_t>(weapon + Offset::netvars::m_iAccountID, accountID);

			if (shouldUpdate)
				csgo.Write<std::int32_t>(csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState) + 0x174, -1);

		}
	}
	
}
