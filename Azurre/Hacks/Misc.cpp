#include "Misc.h"
#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/LocalPlayer.h"

void Misc::bunnyHop() noexcept {
	if (!cfg->m.bhop) return;

	if (!localPlayer) return;

	const auto flags = entityOffsets.flags(localPlayer.get());

	if (GetAsyncKeyState(VK_SPACE))
		(flags & (1 << 0)) ?
		csgo.Write<std::uintptr_t>(IClient + Offset::signatures::dwForceJump, 6) :
		csgo.Write<std::uintptr_t>(IClient + Offset::signatures::dwForceJump, 4);

}