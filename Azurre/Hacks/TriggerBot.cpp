#include "TriggerBot.h"

#include "../SDK/LocalPlayer.h"
#include "../SDK/Entity.h"
#include "../SDK/Vector.h"
#include "../Config.h"
#include "../GUI.h"

#include <thread>

void TriggerBot::run() noexcept{
	while (THREAD_LOOP) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (!localPlayer) continue;

		if (!cfg->t.enabled) continue;

		if (localPlayer->isDead()) continue;

		const auto& crosshair = localPlayer->crosshairID();

		if (!crosshair || crosshair > 64) continue;

		const auto& entity = mem.Read<Entity*>(IClient.address + Offset::signatures::dwEntityList + (crosshair - 1) * 0x10);

		int weaponIndex = mem.Read<int>(localPlayer.get() + Offset::netvars::m_hActiveWeapon) & ENT_ENTRY_MASK;

		if (!weaponIndex) continue;

		const auto& activeWeapon = getEntity(weaponIndex - 1);

		if (!activeWeapon || activeWeapon->clip() < 1)
			continue;

		if (entity->isDead() || entity->gunGameImmunity()) continue;

		if (!cfg->t.friendlyFire && entity->isSameTeam()) continue;

		if (cfg->t.hotkey.isActive()) {
			static DWORD time = GetTickCount();
			if (GetTickCount() - time >= static_cast<unsigned int>(cfg->t.delay)) {
				if (cfg->restrictions) {
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(cfg->t.burst));
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

				}
				else {
					int burstBuff = 0;
					do {
						mem.Write<uintptr_t>(IClient.address + Offset::signatures::dwForceAttack, 5);
						std::this_thread::sleep_for(std::chrono::milliseconds(cfg->t.burst));
						mem.Write<uintptr_t>(IClient.address + Offset::signatures::dwForceAttack, 6);
						burstBuff++;
					} while (burstBuff > cfg->t.burst);
				}

				time = GetTickCount();
			}
		}
	}
}