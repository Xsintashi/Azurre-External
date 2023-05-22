#include "Memory.h"
#include "Core.h"
#include "GUI.h"
#include "Offsets.h"

#include "Hacks/Aimbot.h"
#include "Hacks/Chams.h"
#include "Hacks/Clantag.h"
#include "Hacks/Minimap.h"
#include "Hacks/Misc.h"
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
	SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
	IClient.address = csgo.GetModuleAddress("client.dll");
	IClient.size = csgo.ModuleSize("client.dll");
	IEngine.address = csgo.GetModuleAddress("engine.dll");
	IEngine.size = csgo.ModuleSize("engine.dll");
	IClientState.address = csgo.Read<uintptr_t>(IEngine.address + Offset::signatures::dwClientState);
	IPlayerResource.address = csgo.Read<uintptr_t>(IClient.address + Offset::signatures::dwPlayerResource);
	localPlayer.init(csgo.Read<Entity*>(IClient.address + Offset::signatures::dwLocalPlayer));
};

void Core::update() {
	IConsole = FindWindowA("Valve001", NULL);
	localPlayer.init(csgo.Read<Entity*>(IClient.address + Offset::signatures::dwLocalPlayer));
	globalVars = csgo.Read<GlobalVars>(IEngine.address + Offset::signatures::dwGlobalVars);
	gameState = csgo.Read<int>(IClientState.address + 0x108);
	screenSize = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) };
	RECT rct;
	if (GetWindowRect(IConsole, &rct)) {
		gameScreenPos = { static_cast<float>(rct.left) , static_cast<float>(rct.top) };
		gameScreenPosEnd = { gameScreenPos.x + gameScreenSize.x , gameScreenPos.y + gameScreenSize.y };
		gameScreenSize = { static_cast<float>(rct.right - rct.left), static_cast<float>(rct.bottom - rct.top) };
	}

	cfg->a.hotkey.handleToggle();
	cfg->t.hotkey.handleToggle();
	cfg->v.thirdPersonKey.handleToggle();

	Discord::Update();
};

void Core::entityDataUpdate() noexcept {
	const auto& userInfoTable = csgo.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo);

	entityData.clear();
	for (int unsigned idx = 1; idx <= 32; idx++) {
		const auto& entity = getEntity(idx);
		if (!entity) continue;

		// Player Info
		const auto& items = csgo.Read<uintptr_t>(csgo.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
		PlayerInfo playerInfo = csgo.Read<PlayerInfo>(csgo.Read<uintptr_t>(items + 0x28 + (idx * 0x34)));

		const auto& health = entity->health();
		const auto& armor = entity->armor();
		const auto& hasHelmet = entity->hasHelmet();
		const auto& hasDefuser = entity->hasDefuser();
		const auto& teamNumber = static_cast<int>(entity->teamNumber());
		const auto& money = entity->money();
		const auto& weaponID = entity->getWeaponIDFromPlayer();
		const std::string name = playerInfo.name;
		const bool bot = playerInfo.fakeplayer;
		const char* steamID = playerInfo.szSteamID;
		char temp[18];
		ReadProcessMemory(csgo.processHandle, (LPCVOID)(entity + Offset::netvars::m_szLastPlaceName), temp, 18, NULL);
		std::string placename = (temp + '\0');

		const auto& rank = csgo.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveRanking + (idx * 4));
		const auto& wins = csgo.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveWins + (idx * 4));

		entityData.push_back({ entity, idx, steamID, bot, name , health, armor, hasHelmet, hasDefuser, teamNumber, money, weaponID, placename, rank, wins });
	}
}

void Core::_() noexcept {
	while (GUI::isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5)); // ~ 30 loops per second
		update();
		Minimap::_();
		Misc::fakeLag();
		Aimbot::recoilSystem();
		TriggerBot::run();
		Chams::run();
		Clan::update();
		Misc::bunnyHop();
		Misc::fastStop();
		Visuals::noFlash();
		Visuals::thirdperson();
		Visuals::fov();
	}
}