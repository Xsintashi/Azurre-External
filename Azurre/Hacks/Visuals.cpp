#include "Visuals.h"
#include "Misc.h"

#include "../GUI.h"
#include "../Config.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/Vector.h"

#include <algorithm>
#include <thread>

void Visuals::thirdperson() //shitty method
{
    static bool once = false;

    if (!cfg->v.thirdPerson) return;

    if (!localPlayer) return;

    if (cfg->v.thirdPersonKey.isActive()) {
        once = true;
        csgo.Write<int>(localPlayer.get() + Offset::netvars::m_iObserverMode, 1);
        return;
    }
    if (once) {
        once = false;
        csgo.Write<int>(localPlayer.get() + Offset::netvars::m_iObserverMode, 0);
    }
}

void Visuals::noFlash() {

    if (!localPlayer) return;

    if (!cfg->v.flashReduction) return;

    const float reducedFlash = 255.0f - cfg->v.flashReduction * 2.55f;

    csgo.Write<float>(localPlayer.get() + Offset::netvars::m_flFlashMaxAlpha, reducedFlash);
}

void Visuals::doNotRenderTeammates() {
    while (GUI::isRunning){
        if (!cfg->v.noAllies || !localPlayer) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        for (unsigned int i = 1; i <= 32; i++) {
            const auto& entity = getEntity(i);
            if (!entity) {
                continue;
            }

            if((uintptr_t)entity == localPlayer.get())

            if (entity->isDead() || entity->dormant()) {
                continue;
            }

            if (entity->isSameTeam())
                csgo.Write<bool>((uintptr_t)entity + Offset::netvars::m_bReadyToDraw, false);
        }
    }
}

void Visuals::fov() noexcept {

    if (!localPlayer) return;

    if (cfg->v.fov == 90) return;

    csgo.Write<int>(localPlayer.get() + Offset::netvars::m_iFOV, cfg->v.fov);
}