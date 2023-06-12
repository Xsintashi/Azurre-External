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
#include "SDK/Matrix.h"
#include "Config.h"
#include "DiscordSDK/RPC.h"

void Core::init() {
	SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
	IClient.address = mem.GetModuleAddress("client.dll");
	IClient.size = mem.ModuleSize("client.dll");
	IEngine.address = mem.GetModuleAddress("engine.dll");
	IEngine.size = mem.ModuleSize("engine.dll");
	IVstdlib.address = mem.GetModuleAddress("vstdlib.dll");
	IVstdlib.size = mem.ModuleSize("vstdlib.dll");
	IClientState.address = mem.Read<uintptr_t>(IEngine.address + Offset::signatures::dwClientState);
	IGameRules.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwGameRulesProxy);
	IPlayerResource.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwPlayerResource);
	localPlayer.init(mem.Read<Entity*>(IClient.address + Offset::signatures::dwLocalPlayer));
	gameState = mem.Read<ConnectionState>(IClientState.address + Offset::signatures::dwClientState_State);
	const auto dir = mem.Read<std::array<char, 128>>(IEngine.address + Offset::signatures::dwGameDir);
	gameDir = dir.data();
};

void Core::update() {
	IConsole = FindWindowA("Valve001", NULL);
	gameState = mem.Read<ConnectionState>(IClientState.address + Offset::signatures::dwClientState_State);
	IClientState.address = mem.Read<uintptr_t>(IEngine.address + Offset::signatures::dwClientState);
	IPlayerResource.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwPlayerResource);
	IGameRules.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwGameRulesProxy);
	localPlayer.init(mem.Read<Entity*>(IClient.address + Offset::signatures::dwLocalPlayer));
	globalVars = mem.Read<GlobalVars>(IEngine.address + Offset::signatures::dwGlobalVars);
	viewMatix = mem.Read<Matrix4x4>(IClient.address + Offset::signatures::dwViewMatrix);
	maxEntity = mem.Read<int>(IClient.address + Offset::signatures::dwEntityList + 0x2001C);
	highestEntityIndex = mem.Read<int>(IClient.address + Offset::signatures::dwEntityList + 0x20024);
	screenSize = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) };
	const auto map = mem.Read<std::array<char, 128>>(IClientState.address + Offset::signatures::dwClientState_Map);
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
};

void Core::gameDataUpdate() noexcept {
	const auto& userInfoTable = mem.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo);
	gameData.playerData.clear();
	gameData.weaponData.clear();
	gameData = {}; // reset GameData
	for (int idx = 0; idx <= highestEntityIndex; idx++) {

		const auto& entity = getEntity(idx);
		if (!entity) continue;

		if (entity->isWeapon())
			gameData.weaponData.push_back(entity);

		switch (GetClassId(entity)) {
		default:
			break;
		case ClassID::CSPlayer: {

			if (cfg->m.radarHack && !cfg->restrictions && !entity->isSameTeam())
				mem.Write<bool>(entity + Offset::netvars::m_bSpotted, true);

			if ((uintptr_t)entity == localPlayer.get())
				localPlayerIndex = idx;

			// Player Info
			const auto& items = mem.Read<uintptr_t>(mem.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
			PlayerInfo playerInfo = mem.Read<PlayerInfo>(mem.Read<uintptr_t>(items + 0x28 + (idx * 0x34)));

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
			ReadProcessMemory(mem.processHandle, (LPCVOID)(entity + Offset::netvars::m_szLastPlaceName), temp, 18, NULL);
			std::string placename = (temp + '\0');

			const auto& rank = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveRanking + 0x4 + idx * 4);
			const auto& wins = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveWins + 0x4 + idx * 4);

			const int clampedRank = std::clamp(rank, 0, 18);

			gameData.playerData.push_back({ entity, idx, steamID, bot, name , health, armor, hasHelmet, hasDefuser, teamNumber, money, weaponID, placename, clampedRank , wins });

			if (entity->isDefusing()) //Bomb Timer
				gameData.defusingPlayerName = name;

			// Spectator List
			const int obs = (entity->observerTarget() & ENT_ENTRY_MASK);
			if (entity->isDead() && obs == localPlayerIndex + 1) // Ghetto way, hope it will work (me before testing on valve servers)
				gameData.observerData.push_back({ name, entity->observerMode() });

			break;
		}
		case ClassID::Tablet: {
			gameData.tablet = entity;
			if (cfg->m.fixTablet)
				mem.Write<bool>(gameData.tablet + Offset::netvars::m_bTabletReceptionIsBlocked, false);
			break;
		}
		case ClassID::ToneMapController: {
			gameData.toneMapController = entity;
			if (cfg->v.customPostProcessing.enabled) {
				mem.Write<bool>(entity + Offset::netvars::m_bUseCustomBloomScale, cfg->v.customPostProcessing.enabled);
				mem.Write<bool>(entity + Offset::netvars::m_bUseCustomAutoExposureMax, cfg->v.customPostProcessing.enabled);
				mem.Write<bool>(entity + Offset::netvars::m_bUseCustomAutoExposureMin, cfg->v.customPostProcessing.enabled);

				float bloomScale = cfg->v.customPostProcessing.bloomScale * 0.01f;
				float worldExposure = cfg->v.customPostProcessing.worldExposure * 0.001f;

				mem.Write<float>(entity + Offset::netvars::m_flCustomBloomScale, bloomScale);
				mem.Write<float>(entity + Offset::netvars::m_flCustomAutoExposureMax, worldExposure);
				mem.Write<float>(entity + Offset::netvars::m_flCustomAutoExposureMin, worldExposure);
			}
			break;
		}
		case ClassID::PlantedC4: {
			gameData.plantedC4 = entity;
			break;
		}
		case ClassID::BaseCSGrenadeProjectile:
			gameData.projectileData.push_back({entity, "HE/Flash"});
				break;
		case ClassID::DecoyProjectile:
			gameData.projectileData.push_back({ entity, "Decoy" });
			break;
		case ClassID::MolotovProjectile:
			gameData.projectileData.push_back({ entity, "Molotov" });
			break;
		case ClassID::SmokeGrenadeProjectile:
			gameData.projectileData.push_back({ entity, "Smoke" });
			break;
			
		}
	}
}

void Core::_() noexcept {
	while (GUI::isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		update();
		Aimbot::recoilSystem();
		TriggerBot::run();
		Chams::run();
		Misc::bunnyHop();
		if (!cfg->restrictions) {
			Misc::fakeLag();
			Misc::fastStop();
			Visuals::noFlash();
			Visuals::thirdperson();
			Visuals::fov();
		}
	}
}