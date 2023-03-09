#include "TriggerBot.h"

#include "../SDK/LocalPlayer.h"
#include "../SDK/Entity.h"
#include "../SDK/Vector.h"
#include "../Config.h"

#include <thread>

void TriggerBot::run() noexcept{

	if (!localPlayer) return;

	if (!cfg->t.enabled) return;

	if (localPlayer->isDead()) return;

	const auto& crosshair = localPlayer->crosshairID();

	if (!crosshair || crosshair > 64) return;

	const auto& entity = csgo.Read<Entity*>(IClient + Offset::signatures::dwEntityList + (crosshair - 1) * 0x10);

	int weaponIndex = csgo.Read<int>(localPlayer.get() + Offset::netvars::m_hActiveWeapon) & 0xFFF;

	if (!weaponIndex) return;

	const auto& activeWeapon = getEntity(weaponIndex - 1);

	if (!activeWeapon || activeWeapon->clip() < 1)
		return;

	if (entity->isDead()) return;

	if (entity->teamNumber() == localPlayer->teamNumber()) return;

	if(!cfg->t.hotkey.isSet() || (cfg->t.hotkey.isSet() && cfg->t.hotkey.isDown()))
		csgo.Write<uintptr_t>(IClient + Offset::signatures::dwForceAttack, 6);

}