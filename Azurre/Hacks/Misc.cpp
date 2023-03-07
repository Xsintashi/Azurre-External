#include "Misc.h"
#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include <algorithm>
#include "../SDK/Vector.h"
#include "../GUI.h"
#include <chrono>

void Misc::bunnyHop() noexcept {
	if (!cfg->m.bhop) return;

	if (!localPlayer) return;

	if (localPlayer->isDead()) return;

	const auto flags = localPlayer->flags();

	if (GetAsyncKeyState(VK_SPACE))
		(flags & (1 << 0)) ?
		csgo.Write<std::uintptr_t>(IClient + Offset::signatures::dwForceJump, 6) :
		csgo.Write<std::uintptr_t>(IClient + Offset::signatures::dwForceJump, 4);

}

void Misc::fakeLag() {
	if (!localPlayer) return;

	if (localPlayer->isDead()) return;

	const auto& sendPacket = csgo.Read<BYTE>(IEngine + Offset::signatures::dwbSendPackets);
	const auto& chokedPackets = csgo.Read<int>(IClientState + Offset::signatures::clientstate_choked_commands);
	int choke = 0;

	if (cfg->m.fakeLag)
	{
		const float speed = localPlayer->velocity().length2D() >= 15.0f ? localPlayer->velocity().length2D() : 0.0f;

		switch (cfg->m.fakeLagType) {
		case 0: //Static
			choke = cfg->m.fakeLagLimit;
			break;
		case 1: //Adaptive
			choke = std::clamp(static_cast<int>(std::ceilf(64 / (speed * globalVars->intervalPerTick))), 1, cfg->m.fakeLagLimit);
			break;
		case 2: // Random
			srand(static_cast<unsigned int>(time(NULL)));
			choke = rand() % cfg->m.fakeLagLimit + 1;
			break;
		}
	}

	choke = std::clamp(choke, 0, 16);

	csgo.Write<byte>(IEngine + Offset::signatures::dwbSendPackets, chokedPackets >= choke);
}

void Misc::forceReload(bool onKey) {

	if (!onKey) {
		csgo.Write<std::int32_t>(csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState) + 0x174, -1);
		return;
	}
	if (GetAsyncKeyState(VK_END)) {
		csgo.Write<std::int32_t>(csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState) + 0x174, -1);
	}
}

void Misc::entityLoop() {

	if (!localPlayer) return;

	for (unsigned int i = 1; i <= 32; i++) {
		const auto& entity = getEntity(i);
		if (!entity) continue;

		if (entity->teamNumber() == localPlayer->teamNumber()) continue;

		if(cfg->m.radarHack)
			csgo.Write<bool>((uintptr_t)entity + Offset::netvars::m_bSpotted, true);
	}
}

void Misc::modifyClasses() {

	if (!localPlayer) return;

	for (int i = 0; i < 512; i++)
	{
		int entity = csgo.Read<int>(IClient + Offset::signatures::dwEntityList + i * 0x10);
		if (!entity) continue;
		if (cfg->v.noSmoke && GetClassId(entity) == ClassID::SmokeGrenadeProjectile)
			csgo.Write<Vector>(entity + Offset::netvars::m_vecOrigin, Vector(999.f, 999.f, 999.f));
		if (cfg->m.fixTablet && GetClassId(entity) == ClassID::Tablet)
			csgo.Write<bool>(entity + Offset::netvars::m_bTabletReceptionIsBlocked, false);
	}
}