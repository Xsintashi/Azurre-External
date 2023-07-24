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

    if (cfg->restrictions) return; //RPM ONLY

    if (!cfg->v.thirdPerson) return;

    if (!localPlayer) return;

    if (cfg->v.thirdPersonKey.isActive()) {
        once = true;
        mem.Write<int>(localPlayer.get() + Offset::netvars::m_iObserverMode, 1);
        return;
    }
    if (once) {
        once = false;
        mem.Write<int>(localPlayer.get() + Offset::netvars::m_iObserverMode, 0);
    }
}

void Visuals::noFlash() {

    if (!localPlayer) return;

    if (!cfg->v.flashReduction) return;

    if (cfg->restrictions) return; //RPM ONLY

    const float reducedFlash = 255.0f - cfg->v.flashReduction * 2.55f;

    mem.Write<float>(localPlayer.get() + Offset::netvars::m_flFlashMaxAlpha, reducedFlash);
}

void Visuals::doNotRenderTeammates() {
    while (THREAD_LOOP){
        if (!cfg->v.noAllies || !localPlayer || cfg->restrictions) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        for (unsigned int i = 1; i <= 32; i++) {
            const auto& entity = getEntity(i);
            if (!entity->isValid())
                continue;

            if ((uintptr_t)entity == localPlayer.get() || entity->isDead() || entity->dormant() || !entity->isSameTeam())
                continue;
            mem.Write<bool>(entity + Offset::netvars::m_bReadyToDraw, false);
        }
    }
}

void Visuals::fov() noexcept {

    if (!localPlayer) return;

    if (cfg->restrictions) return; //RPM ONLY

    if (cfg->v.fov == 90) return;

    mem.Write<int>(localPlayer.get() + Offset::netvars::m_iFOV, cfg->v.fov);
}

std::vector<Vector> trailsData;

void Visuals::trailsThread() noexcept {
    while (THREAD_LOOP) {

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (!cfg->v.trails.color.enabled)
            continue;

        if (gameState != ConnectionState::FullyConnected)
            continue;

        if (!localPlayer)
            continue;

        if (localPlayer->isDead())
            continue;

        static unsigned size = cfg->v.trails.size;
        if (size != cfg->v.trails.size) {
            size = cfg->v.trails.size;
            trailsData.clear();
        }

        if (!trailsData.empty() && trailsData.size() > cfg->v.trails.size)
            trailsData.erase(trailsData.begin());

        trailsData.push_back(localPlayer->origin());
    }

}

void Visuals::renderTrails() {
    if (!cfg->v.trails.color.enabled)
        return;

    if (gameState != ConnectionState::FullyConnected)
        return;

    if (!localPlayer)
        return;

    if (localPlayer->isDead())
        return;

    if (trailsData.empty())
        return;


    for (unsigned i = 0; i < (trailsData.size() - 1); i++) {

        auto color = Helpers::calculateColor(cfg->v.trails.color);

        if (cfg->v.trails.rainbowType) { //Chroma Rainbow
            constexpr float pi = std::numbers::pi_v<float>;
            const float r = static_cast<float>(std::sin((cfg->v.trails.color.rainbowSpeed * i / 16) - 5.f * globalVars->realTime / 2.f) * 0.5f + 0.5f);
            const float g = static_cast<float>(std::sin((cfg->v.trails.color.rainbowSpeed * i / 16) - 5.f * globalVars->realTime / 2.f + 2 * pi / 3) * 0.5f + 0.5f);
            const float b = static_cast<float>(std::sin((cfg->v.trails.color.rainbowSpeed * i / 16) - 5.f * globalVars->realTime / 2.f + 4 * pi / 3) * 0.5f + 0.5f);

            color = ImGui::GetColorU32(ImVec4{ r, g, b, 1.f });
        }

        Vector pos = Helpers::world2Screen(gameScreenSize, trailsData[i], viewMatrix);
        Vector pos_ = Helpers::world2Screen(gameScreenSize, trailsData[i + 1], viewMatrix);
        if (pos.z >= 0.001f && pos_.z >= 0.001f)
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ pos.x + gameScreenPos.x, pos.y + gameScreenPos.y }, ImVec2{ pos_.x + gameScreenPos.x, pos_.y + gameScreenPos.y }, color, cfg->v.trails.thickness);
    }
}