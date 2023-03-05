#include "Misc.h"
#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include <algorithm>
#include "../SDK/Vector.h"

void Misc::bunnyHop() noexcept {
	if (!cfg->m.bhop) return;

	if (!localPlayer) return;

	const auto flags = entityOffsets.flags(localPlayer.get());

	if (GetAsyncKeyState(VK_SPACE))
		(flags & (1 << 0)) ?
		csgo.Write<std::uintptr_t>(IClient + Offset::signatures::dwForceJump, 6) :
		csgo.Write<std::uintptr_t>(IClient + Offset::signatures::dwForceJump, 4);

}

void Misc::forceReload(bool onKey) {

	if (!onKey) {
		csgo.Write<std::int32_t>(csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState) + 0x174, -1);
		return;
	}
	if (GetAsyncKeyState(VK_INSERT)) {
		csgo.Write<std::int32_t>(csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState) + 0x174, -1);
	}
}

void Misc::modifyClasses() {

	if (!localPlayer) return;

	for (int i = 0; i < 512; i++)
	{
		int EntBase = csgo.Read<int>(IClient + Offset::signatures::dwEntityList + i * 0x10);
		if (!EntBase) continue;
		if (GetClassId(EntBase) == ClassID::SmokeGrenadeProjectile && cfg->v.noSmoke)
			csgo.Write<Vector>(EntBase + Offset::netvars::m_vecOrigin, Vector(999.f, 999.f, 999.f));
		if (GetClassId(EntBase) == ClassID::Tablet && cfg->m.fixTablet)
			csgo.Write<bool>(EntBase + Offset::netvars::m_bTabletReceptionIsBlocked, false);
	}
}