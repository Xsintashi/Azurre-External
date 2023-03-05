#include "Memory.h"
#include "Core.h"
#include "Offsets.h"

#include <algorithm>    // std::sort

#include "SDK/LocalPlayer.h"
#include "SDK/interfaces.h"
#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/PlayerInfo.h"

void Core::init() {
	interfaces.emplace(Interfaces{});
	IClient = csgo.GetModuleAddress("client.dll");
	IEngine = csgo.GetModuleAddress("engine.dll");
	IClientState = csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState);
};

void Core::update() {
	localPlayer.init((Entity*)csgo.Read<uintptr_t>(IClient + Offset::signatures::dwLocalPlayer));
	globalVars = csgo.Read<GlobalVars>(IEngine + Offset::signatures::dwGlobalVars);

	const auto& userInfoTable = csgo.Read<uintptr_t>(IClientState + Offset::signatures::dwClientState_PlayerInfo);

	entityData.clear();
	for (int unsigned idx = 0; idx <= 32; idx++) {
		const auto& entity = getEntity(idx);
		if (!entity) continue;

		// Player Info
		const auto& items = csgo.Read<uintptr_t>(csgo.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
		playerInfo = csgo.Read<PlayerInfo>(csgo.Read<uintptr_t>(items + 0x28 + (idx * 0x34)));

		const auto& health = entity->health();
		const auto& armor = entity->armor();
		const auto& teamNumber = entity->teamNumber();
		const auto& money = entity->money();
		const std::string name = playerInfo.szName;
		const bool bot = playerInfo.fakeplayer;
		const unsigned long steamID = playerInfo.iSteamID;
		//char placename[18] = adress->lastPlaceName(entity);

		entityData.push_back({ entity, idx, steamID, bot, name , health, armor, teamNumber, money });
	}
};
