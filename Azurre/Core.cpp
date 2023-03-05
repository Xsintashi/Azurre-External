#include "Memory.h"
#include "Core.h"
#include "Offsets.h"

#include <algorithm>    // std::sort

#include "SDK/LocalPlayer.h"
#include "SDK/interfaces.h"
#include "SDK/Entity.h"
#include "SDK/PlayerInfo.h"

void Core::init() {
	interfaces.emplace(Interfaces{});
	IClient = csgo.GetModuleAddress("client.dll");
	IEngine = csgo.GetModuleAddress("engine.dll");
	IClientState = csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState);
};

void Core::update() {
	localPlayer.init(csgo.Read<uintptr_t>(IClient + Offset::signatures::dwLocalPlayer));

	const auto& userInfoTable = csgo.Read<uintptr_t>(IClientState + Offset::signatures::dwClientState_PlayerInfo);

	entityData.clear();
	for (int unsigned idx = 0; idx <= 32; idx++) {
		const auto& adress = getEntity(idx);
		if (!adress) continue;

		// Player Info
		const auto& items = csgo.Read<uintptr_t>(csgo.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
		playerInfo = csgo.Read<PlayerInfo>(csgo.Read<uintptr_t>(items + 0x28 + (idx * 0x34)));

		const auto& health = entityOffsets.health(adress);
		const auto& armor = entityOffsets.armor(adress);
		const auto& teamNumber = entityOffsets.teamNumber(adress);
		const auto& money = entityOffsets.money(adress);
		const std::string name = playerInfo.szName;
		const bool bot = playerInfo.fakeplayer;
		const unsigned long steamID = playerInfo.iSteamID;
		//char placename[18] = entityOffsets.lastPlaceName(adress);

		entityData.push_back({ adress, idx, steamID, bot, name , health, armor, teamNumber, money });
	}
};
