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
	gameState = csgo.Read<ConnectionState>(IClientState.address + Offset::signatures::dwClientState_State);
	const auto dir = csgo.Read<std::array<char, 128>>(IEngine.address + Offset::signatures::dwGameDir);
	gameDir = dir.data();
};

void Core::update() {
	IConsole = FindWindowA("Valve001", NULL);
	gameState = csgo.Read<ConnectionState>(IClientState.address + Offset::signatures::dwClientState_State);
	IClientState.address = csgo.Read<uintptr_t>(IEngine.address + Offset::signatures::dwClientState);
	IPlayerResource.address = csgo.Read<uintptr_t>(IClient.address + Offset::signatures::dwPlayerResource);
	localPlayer.init(csgo.Read<Entity*>(IClient.address + Offset::signatures::dwLocalPlayer));
	globalVars = csgo.Read<GlobalVars>(IEngine.address + Offset::signatures::dwGlobalVars);
	screenSize = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) };
	const auto map = csgo.Read<std::array<char, 128>>(IClientState.address + Offset::signatures::dwClientState_Map);
	mapName = map.data();
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

void Core::gameDataUpdate() noexcept {
	const auto& userInfoTable = csgo.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo);
	gameData = {}; // reset GameData
	for (int unsigned idx = 0; idx <= 1024; idx++) {

		const auto& entity = getEntity(idx);
		if (!entity) continue;

		switch (GetClassId(entity)){
			default:
				break;
			case ClassID::CSPlayer: {

				if (cfg->m.radarHack && !cfg->restrictions && !entity->isSameTeam())
					csgo.Write<bool>(entity + Offset::netvars::m_bSpotted, true);

				if ((uintptr_t)entity == localPlayer.get())
					localPlayerIndex = idx;

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
				std::uint64_t steamID = playerInfo.steamID64;
				char temp[18];
				ReadProcessMemory(csgo.processHandle, (LPCVOID)(entity + Offset::netvars::m_szLastPlaceName), temp, 18, NULL);
				std::string placename = (temp + '\0');

				const auto& rank = csgo.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveRanking + 0x4 + idx * 4);
				const auto& wins = csgo.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveWins + 0x4 + idx * 4);

				const int clampedRank = std::clamp(rank, 0, 18);

				gameData.playerData.push_back({ entity, idx, steamID, bot, name , health, armor, hasHelmet, hasDefuser, teamNumber, money, weaponID, placename, clampedRank , wins });
				
				if (entity->isDefusing()) //Bomb Timer
					gameData.defusingPlayerName = name;

				// Spectator List
				const int obs = (entity->observerTarget() & ENT_ENTRY_MASK);
				if (entity->isDead() && obs == localPlayerIndex + 1) // Ghetto way, hope it will work (me before testing on valve servers)
					gameData.observerData.push_back({ name, entity->observerMode()});

				break;
			}
			case ClassID::Tablet: {
				gameData.tablet = entity;
				if (cfg->m.fixTablet)
					csgo.Write<bool>(gameData.tablet + Offset::netvars::m_bTabletReceptionIsBlocked, false);
				break;
			}
			case ClassID::ToneMapController: {
				gameData.toneMapController = entity;
				if (cfg->v.customPostProcessing.enabled) {
					csgo.Write<bool>(entity + Offset::netvars::m_bUseCustomBloomScale, cfg->v.customPostProcessing.enabled);
					csgo.Write<bool>(entity + Offset::netvars::m_bUseCustomAutoExposureMax, cfg->v.customPostProcessing.enabled);
					csgo.Write<bool>(entity + Offset::netvars::m_bUseCustomAutoExposureMin, cfg->v.customPostProcessing.enabled);

					float bloomScale = cfg->v.customPostProcessing.bloomScale * 0.01f;
					float worldExposure = cfg->v.customPostProcessing.worldExposure * 0.001f;

					csgo.Write<float>(entity + Offset::netvars::m_flCustomBloomScale, bloomScale);
					csgo.Write<float>(entity + Offset::netvars::m_flCustomAutoExposureMax, worldExposure);
					csgo.Write<float>(entity + Offset::netvars::m_flCustomAutoExposureMin, worldExposure);
				}
				break;
			}
			case ClassID::PlantedC4: {
				gameData.plantedC4 = entity;
				break;
			}
		}
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