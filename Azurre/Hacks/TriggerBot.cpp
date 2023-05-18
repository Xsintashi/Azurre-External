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

	const auto& entity = csgo.Read<Entity*>(IClient.address + Offset::signatures::dwEntityList + (crosshair - 1) * 0x10);

	int weaponIndex = csgo.Read<int>(localPlayer.get() + Offset::netvars::m_hActiveWeapon) & 0xFFF;

	if (!weaponIndex) return;

	const auto& activeWeapon = getEntity(weaponIndex - 1);

	if (!activeWeapon || activeWeapon->clip() < 1)
		return;

	if (entity->isDead()) return;

	if (!cfg->t.friendlyFire && entity->isSameTeam()) return;

	if (cfg->t.hotkey.isActive()) {
		static DWORD time = GetTickCount();
		if (GetTickCount() - time >= static_cast<unsigned int>(cfg->t.delay)) {
			if (cfg->restrictions) {
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
			else csgo.Write<uintptr_t>(IClient.address + Offset::signatures::dwForceAttack, 6);

			time = GetTickCount();
		}
	}
		

}