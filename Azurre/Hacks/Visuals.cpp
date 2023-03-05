#include "Visuals.h"

#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include <algorithm>
#include "../SDK/Vector.h"
#include "Misc.h"

void Visuals::thirdperson() //shitty method
{
    static bool once = false;

    if (!cfg->v.thirdPerson) return;

    if (!localPlayer) return;

    if (GetAsyncKeyState(0x43) & 0x8000) //C Key
    {
        once = true;
        csgo.Write<int>(localPlayer + Offset::netvars::m_iObserverMode, 1);
    }
    else if (GetAsyncKeyState(0x56) & 0x8000) //V Key
    {
        once = true;
        csgo.Write<int>(localPlayer + Offset::netvars::m_iObserverMode, 3);
    }
    else
    {
        if (once)
        {
            once = false;
            csgo.Write<int>(localPlayer + Offset::netvars::m_iObserverMode, 0);
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

void Visuals::noFlash() {

    if (!localPlayer) return;

    if (!cfg->v.flashReduction) return;

    const auto& duration = localPlayer->flashDuration();

    if (duration < 5.f) return;

    const float reducedFlash = 255.0f - cfg->v.flashReduction * 2.55f;

    csgo.Write<float>(localPlayer + Offset::netvars::m_flFlashMaxAlpha, reducedFlash);
}