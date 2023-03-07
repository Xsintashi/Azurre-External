#include "Glow.h"

#include "../GUI.h"
#include "../Offsets.h"
#include "../Helpers.h"
#include "../Config.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"

void Glow::run() noexcept {
	while (GUI::isRunning) {
		if (!localPlayer) continue;

		if (!cfg->g.enabled) continue;

		const auto glowObjectManager = csgo.Read<std::uintptr_t>(IClient + Offset::signatures::dwGlowObjectManager);

		const Color4 enemyColor = Helpers::ConvertColors4(cfg->g.enemy);
		const Color4 allyColor = Helpers::ConvertColors4(cfg->g.ally);

		for (auto i = 1; i <= 32; ++i)
		{
			const auto entity = getEntity(i);

			if (!entity)
				continue;

			if (entity->teamNumber() == localPlayer->teamNumber() && cfg->v.noAllies)
				continue;

			static Color4 color;

			if (entity->teamNumber() == localPlayer->teamNumber())
				color = allyColor;
			else
				color = enemyColor;

			const auto glowIndex = csgo.Read<std::int32_t>((uintptr_t)entity + Offset::netvars::m_iGlowIndex);

			// do glow by writing each variable
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, color.r); //Red
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, color.g); //Green
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, color.b); //Blue
			csgo.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, color.a); //Alpha

			csgo.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
			csgo.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, true);
		}
	}
}
