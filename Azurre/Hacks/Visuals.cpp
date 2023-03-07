#include "Visuals.h"
#include "Misc.h"

#include "../GUI.h"
#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/Vector.h"

#include <algorithm>

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

void Visuals::noFlash() {

    if (!localPlayer) return;

    if (!cfg->v.flashReduction) return;

    const auto& duration = localPlayer->flashDuration();

    if (duration < 5.f) return;

    const float reducedFlash = 255.0f - cfg->v.flashReduction * 2.55f;

    csgo.Write<float>(localPlayer.get() + Offset::netvars::m_flFlashMaxAlpha, reducedFlash);
}

void Visuals::doNotRenderTeammates() {
    while (GUI::isRunning){
        if (!cfg->v.noAllies) continue;

        if (!localPlayer) continue;

        for (unsigned int i = 1; i <= 32; i++) {
            const auto& entity = getEntity(i);
            if (!entity) continue;

            if (entity->teamNumber() == localPlayer->teamNumber())

                csgo.Write<bool>((uintptr_t)entity + Offset::netvars::m_bReadyToDraw, false);
        }
    }
}