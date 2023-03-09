#include "Chams.h"

#include "../Offsets.h"
#include "../Helpers.h"
#include "../Config.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"

void Chams::run() noexcept {

	if (!localPlayer) return;

	const auto enemyColor = Helpers::ConvertColors3ToUint8_t(cfg->c.enemy.color);
	const auto allyColor = Helpers::ConvertColors3ToUint8_t(cfg->c.ally.color);

	static bool toggle = false;

	static float tempCurr = 0.f;	// Limit that for loop/s
	if (tempCurr + 1.f > globalVars->currentTime) return;
	tempCurr = globalVars->currentTime;

	for (unsigned int i = 1; i <= 32; i++) {
		const auto& entity = getEntity(i);
		if (!entity) continue;

		if (cfg->c.enabled) {
			if (entity->teamNumber() == localPlayer->teamNumber())
				csgo.Write<uint8_tColor3>((uintptr_t)entity + Offset::netvars::m_clrRender, allyColor);
			else
				csgo.Write<uint8_tColor3>((uintptr_t)entity + Offset::netvars::m_clrRender, enemyColor);

			float brightness = cfg->c.brightness * 10.f;
			const auto _this = static_cast<uintptr_t>(IEngine + Offset::signatures::model_ambient_min - 0x2c);
			csgo.Write<int32_t>(IEngine + Offset::signatures::model_ambient_min, *reinterpret_cast<uintptr_t*>(&brightness) ^ _this);
			toggle = true;
		} else if(toggle){

			if (entity->teamNumber() == localPlayer->teamNumber())
				csgo.Write<uint8_tColor3>((uintptr_t)entity + Offset::netvars::m_clrRender, uint8_tColor3{ 255, 255, 255 });
			else
				csgo.Write<uint8_tColor3>((uintptr_t)entity + Offset::netvars::m_clrRender, uint8_tColor3{ 255, 255, 255 });

			float brightness = 0;
			const auto _this = static_cast<uintptr_t>(IEngine + Offset::signatures::model_ambient_min - 0x2c);
			csgo.Write<int32_t>(IEngine + Offset::signatures::model_ambient_min, *reinterpret_cast<uintptr_t*>(&brightness) ^ _this);
		}
	}
	if(toggle)
		toggle = false;
}
