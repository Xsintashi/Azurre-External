#include "Memory.h"
#include "Core.h"
#include "Offsets.h"

#include "Hacks/Aimbot.h"
#include "Hacks/TriggerBot.h"
#include "Hacks/Visuals.h"


#include "SDK/LocalPlayer.h"
#include "SDK/interfaces.h"
#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/PlayerInfo.h"
#include "Config.h"
#include "DiscordSDK/RPC.h"

void Core::init() {
	interfaces.emplace(Interfaces{});
	IClient = csgo.GetModuleAddress("client.dll");
	IEngine = csgo.GetModuleAddress("engine.dll");
	IClientState = csgo.Read<uintptr_t>(IEngine + Offset::signatures::dwClientState);
	IPlayerResource = csgo.Read<uintptr_t>(IClient + Offset::signatures::dwPlayerResource);
	IConsole = FindWindowA("Valve001", NULL);
	localPlayer.init(csgo.Read<Entity*>(IClient + Offset::signatures::dwLocalPlayer));
};

void Core::update() {
	localPlayer.init(csgo.Read<Entity*>(IClient + Offset::signatures::dwLocalPlayer));
	globalVars = csgo.Read<GlobalVars>(IEngine + Offset::signatures::dwGlobalVars);
	gameState = csgo.Read<int>(IClientState + 0x108);
	const auto& userInfoTable = csgo.Read<uintptr_t>(IClientState + Offset::signatures::dwClientState_PlayerInfo);

	cfg->a.hotkey.handleToggle();
	cfg->t.hotkey.handleToggle();
	cfg->v.thirdPersonKey.handleToggle();

	Discord::Update();

	entityData.clear();
	for (int unsigned idx = 0; idx <= 32; idx++) {
		const auto& entity = getEntity(idx);
		if (!entity) continue;

		// Player Info
		const auto& items = csgo.Read<uintptr_t>(csgo.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
		playerInfo = csgo.Read<PlayerInfo>(csgo.Read<uintptr_t>(items + 0x28 + (idx * 0x34)));

		const auto& health = entity->health();
		const auto& armor = entity->armor();
		const auto& hasHelmet = entity->hasHelmet();
		const auto& hasDefuser = entity->hasDefuser();
		const auto& teamNumber = static_cast<int>(entity->teamNumber());
		const auto& money = entity->money();
		const auto& weaponID = entity->getWeaponID();
		const std::string name = playerInfo.name;
		const bool bot = playerInfo.fakeplayer;
		const char* steamID = playerInfo.szSteamID;
		char temp[18];
		ReadProcessMemory(csgo.processHandle, (LPCVOID)(entity + Offset::netvars::m_szLastPlaceName), temp, 18, NULL);
		std::string placename = temp;

		entityData.push_back({ entity, idx, steamID, bot, name , health, armor, hasHelmet, hasDefuser, teamNumber, money, weaponID, placename });
	}
};
