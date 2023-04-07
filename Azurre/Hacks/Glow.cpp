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
	while (GUI::isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		if (!localPlayer) continue;

		if (!cfg->g.enabled) continue;

		const auto glowObjectManager = csgo.Read<std::uintptr_t>(IClient.address + Offset::signatures::dwGlowObjectManager);

		for (auto i = 1; i <= 32; ++i)
		{
			const auto entity = getEntity(i);

			if (!entity)
				continue;

			if (entity->isSameTeam() && cfg->v.noAllies)
				continue;

			if ((!cfg->g.ally.enabled && entity->isSameTeam()) || (!cfg->g.enemy.enabled && !entity->isSameTeam())) continue;

			static uint8_tColor4 color;

			if (entity->isSameTeam())
				color = Helpers::ConvertColors4ToUint8_t(cfg->g.ally.color);
			else
				color = Helpers::ConvertColors4ToUint8_t(cfg->g.enemy.color);;

			const auto glowIndex = csgo.Read<std::int32_t>((uintptr_t)entity + Offset::netvars::m_iGlowIndex);

			csgo.Write<uint8_t>(IClient.address + Offset::signatures::force_update_spectator_glow, 235); //Fix Flickering

			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, color[0] / 255.f); //Red
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, color[1] / 255.f); //Green
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, color[2] / 255.f); //Blue
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, color[3] / 255.f); //Alpha

			csgo.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
			csgo.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, true);
		}
	}
}
