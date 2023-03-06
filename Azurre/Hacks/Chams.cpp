#include "Chams.h"

#include "../Offsets.h"
#include "../Helpers.h"
#include "../Config.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"


void Chams::run() noexcept {

	if (!localPlayer) return;

	const Color3 enemyColor = Helpers::ConvertColors3(cfg->c.enemy);
	const Color3 allyColor = Helpers::ConvertColors3(cfg->c.ally);

	static bool toggle = false;

	static float tempCurr = 0.f;	// Limit that for loop/s
	if (tempCurr + 1.f > globalVars->currentTime) return;
	tempCurr = globalVars->currentTime;

	for (unsigned int i = 1; i <= 32; i++) {
		const auto& entity = getEntity(i);
		if (!entity) continue;

		if (cfg->c.enabled) {
			if (entity->teamNumber() == localPlayer->teamNumber())
				csgo.Write<Color3>((uintptr_t)entity + Offset::netvars::m_clrRender, allyColor);
			else
				csgo.Write<Color3>((uintptr_t)entity + Offset::netvars::m_clrRender, enemyColor);

			float brightness = cfg->c.brightness * 10.f;
			const auto _this = static_cast<uintptr_t>(IEngine + Offset::signatures::model_ambient_min - 0x2c);
			csgo.Write<int32_t>(IEngine + Offset::signatures::model_ambient_min, *reinterpret_cast<uintptr_t*>(&brightness) ^ _this);
			toggle = true;
		} else if(toggle){

			if (entity->teamNumber() == localPlayer->teamNumber())
				csgo.Write<Color3>((uintptr_t)entity + Offset::netvars::m_clrRender, Color3(255, 255, 255));
			else
				csgo.Write<Color3>((uintptr_t)entity + Offset::netvars::m_clrRender, Color3(255, 255, 255));

			float brightness = 0;
			const auto _this = static_cast<uintptr_t>(IEngine + Offset::signatures::model_ambient_min - 0x2c);
			csgo.Write<int32_t>(IEngine + Offset::signatures::model_ambient_min, *reinterpret_cast<uintptr_t*>(&brightness) ^ _this);
		}
	}
	if(toggle)
		toggle = false;
}
