#include "TriggerBot.h"

#include "../SDK/LocalPlayer.h"
#include "../SDK/Entity.h"
#include "../SDK/Vector.h"
#include "../Config.h"

void TriggerBot::run() noexcept{

	if (!localPlayer) return;

	if (!cfg->t.enabled) return;

	//const& auto crosshair = localPlayer->isDead();

}