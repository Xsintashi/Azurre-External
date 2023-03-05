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

	if (entity->isDead()) return;

	if (entity->teamNumber() == localPlayer->teamNumber()) return;

	csgo.Write<uintptr_t>(IClient + Offset::signatures::dwForceAttack, 6);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	csgo.Write<uintptr_t>(IClient + Offset::signatures::dwForceAttack, 4);

}