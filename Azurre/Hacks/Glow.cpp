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

		const auto glowObjectManager = csgo.Read<std::uintptr_t>(IClient + Offset::signatures::dwGlowObjectManager);

		for (auto i = 1; i <= 32; ++i)
		{
			const auto entity = getEntity(i);

			if (!entity)
				continue;

			if (entity->teamNumber() == localPlayer->teamNumber() && cfg->v.noAllies)
				continue;

			static uint8_tColor4 color;

			if (entity->teamNumber() == localPlayer->teamNumber())
				color = Helpers::ConvertColors4ToUint8_t(cfg->g.ally.color);
			else
				color = Helpers::ConvertColors4ToUint8_t(cfg->g.enemy.color);;

			const auto glowIndex = csgo.Read<std::int32_t>((uintptr_t)entity + Offset::netvars::m_iGlowIndex);

			// do glow by writing each variable
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, color[0]); //Red
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, color[1]); //Green
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, color[2]); //Blue
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, color[3]); //Alpha

			csgo.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
			csgo.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, true);
		}
	}
}
