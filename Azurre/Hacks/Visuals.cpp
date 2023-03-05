#include "Visuals.h"

#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include <algorithm>
#include "../SDK/Vector.h"

void Visuals::thirdperson() //shitty method
{
    static bool once = false;

    if (!cfg->v.thirdPerson) return;

    if (!localPlayer) return;

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

void Visuals::remove3dSky() {

    return; //doest work atm

    if (!localPlayer) return;

    int skyDisable = csgo.Read<BYTE>(IClient + 0xDC48A0 + 0x30);

    if (skyDisable == 161 && cfg->v.no3DSky == 1) {
        csgo.Write<BYTE>(IClient + 0xDC48A0 + 0x30, skyDisable - 1);
    }
    else if (skyDisable == 160 && cfg->v.no3DSky == 0) {
        csgo.Write<BYTE>(IClient + 0xDC48A0 + 0x30, skyDisable + 1);
    }
}

void Visuals::removeSmoke() {

    if (!localPlayer) return;

    if (!cfg->v.noSmoke) return;
  
    for (int i = 0; i < 512; i++)
    {
        int EntBase = csgo.Read<int>(IClient + Offset::signatures::dwEntityList + i * 0x10);
        if (EntBase == 0) continue;
        if (GetClassId(EntBase) == ClassID::SmokeGrenadeProjectile)
            csgo.Write<Vector>(EntBase + Offset::netvars::m_vecOrigin, Vector(999.f, 999.f, 999.f));
    }
    
}

void Visuals::noFlash() {

    if (!localPlayer) return;

    if (!cfg->v.flashReduction) return;

    const auto& duration = entityOffsets.flashDuration(localPlayer.get());

    if (duration < 5.f) return;

    const float reducedFlash = 255.0f - cfg->v.flashReduction * 2.55f;

    csgo.Write<float>(localPlayer.get() + Offset::netvars::m_flFlashMaxAlpha, reducedFlash);
}