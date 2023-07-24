#include "Glow.h"

#include "../GUI.h"
#include "../Offsets.h"
#include "../Helpers.h"
#include "../Config.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"
#include <thread>

void Glow::run() noexcept {
	while (THREAD_LOOP) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		if (!localPlayer) continue;

		if (cfg->restrictions) continue; //RPM ONLY

		if (!cfg->g.enabled) continue;

		const auto glowObjectManager = mem.Read<std::uintptr_t>(IClient.address + Offset::signatures::dwGlowObjectManager);

		for (const auto& [index, i] : gameData.playerData) {

			if (i.entity->isSameTeam() && cfg->v.noAllies)
				continue;

			if ((!cfg->g.ally.enabled && i.entity->isSameTeam()) || (!cfg->g.enemy.enabled && !i.entity->isSameTeam())) continue;

			static uint8_tColor4 color;

			if (i.entity->isSameTeam())
				color = Helpers::ConvertColors4ToUint8_t(cfg->g.ally.color);
			else
				color = Helpers::ConvertColors4ToUint8_t(cfg->g.enemy.color);;

			const auto glowIndex = mem.Read<std::int32_t>(i.entity + Offset::netvars::m_iGlowIndex);

			mem.Write<uint8_t>(IClient.address + Offset::signatures::force_update_spectator_glow, 235); //Fix Flickering

			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, color[0] / 255.f); //Red
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, color[1] / 255.f); //Green
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, color[2] / 255.f); //Blue
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, color[3] / 255.f); //Alpha

			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, true);
		}
	}
}
