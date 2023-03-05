#include "Misc.h"
#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include <algorithm>

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

void Misc::thirdperson() //shitty method
{
    static bool once = false;

    if (!cfg->m.thirdPerson) return;

    if (GetAsyncKeyState(0x43) & 0x8000) //C Key
    {
        once = true;
        csgo.Write<int>(localPlayer.get() + Offset::netvars::m_iObserverMode, 1);
    }
    else if (GetAsyncKeyState(0x56) & 0x8000) //V Key
    {
        once = true;
        csgo.Write<int>(localPlayer.get() + Offset::netvars::m_iObserverMode, 3);
    }
    else
    {
        if (once)
        {
            once = false;
            csgo.Write<int>(localPlayer.get() + Offset::netvars::m_iObserverMode, 0);
        }
    }

}

void Misc::noFlash() {

    if (!localPlayer) return;

    if (!cfg->m.flashReduction) return;

    const auto& duration = entityOffsets.flashDuration(localPlayer.get());

    if (duration < 5.f) return;

    const float reducedFlash = 255.0f - cfg->m.flashReduction * 2.55f;

    csgo.Write<float>(localPlayer.get() + Offset::netvars::m_flFlashMaxAlpha, reducedFlash);
}