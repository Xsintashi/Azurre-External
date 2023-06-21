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
#include "Hacks/SkinChanger.h"
#include "Console.h"

void Core::init() {
	SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
	IClient.address = mem.GetModuleAddress("client.dll");
	IClient.size = mem.ModuleSize("client.dll");
	cmd.AddLog("client.dll: 0x%p, sizeof: %d", IClient.address, IClient.size);

	IEngine.address = mem.GetModuleAddress("engine.dll");
	IEngine.size = mem.ModuleSize("engine.dll");
	cmd.AddLog("engine.dll: 0x%p, sizeof: %d", IEngine.address, IEngine.size);

	IVstdlib.address = mem.GetModuleAddress("vstdlib.dll");
	IVstdlib.size = mem.ModuleSize("vstdlib.dll");
	cmd.AddLog("vstdlib.dll: 0x%p, sizeof: %d", IVstdlib.address, IVstdlib.size);

	IClientState.address = mem.Read<uintptr_t>(IEngine.address + Offset::signatures::dwClientState);
	IGameRules.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwGameRulesProxy);
	IPlayerResource.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwPlayerResource);
	localPlayer.init(mem.Read<Entity*>(IClient.address + Offset::signatures::dwLocalPlayer));
	cmd.AddLog("localPlayer: 0x%p", localPlayer.get());
	gameState = mem.Read<ConnectionState>(IClientState.address + Offset::signatures::dwClientState_State);
	const auto dir = mem.Read<std::array<char, 128>>(IEngine.address + Offset::signatures::dwGameDir);
	gameDir = dir.data();
};

void Core::update() {
	static std::string mapBuff;
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
	localPlayerIndex = mem.Read<int>(IClientState.address + Offset::signatures::dwClientState_GetLocalPlayer);
	screenSize = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) };
	const auto map = mem.Read<std::array<char, 128>>(IClientState.address + Offset::signatures::dwClientState_Map);
	mapName = map.data();
	if (mapBuff != mapName) {
		parsedMap = false;
		mapBuff = mapName;
	}
	if (!parsedMap) {
		if (bspParser->loadMap(std::string(gameDir).append("\\maps").c_str(), mapName.append(".bsp").c_str())) {
			cmd.AddLog("~[!] Map Parsed!");
			parsedMap = true;
			return;
		} else
			cmd.AddLog("~[!] Couldn't parse map!");
		
	}
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
			const auto& dormant = entity->dormant();
			const std::string name = playerInfo.name;
			const bool bot = playerInfo.fakePlayer;
			const bool hltv = playerInfo.isHLTV;
			const std::string steamID = playerInfo.steamID;
			char placeName[18];
			ReadProcessMemory(mem.processHandle, (LPCVOID)(entity + Offset::netvars::m_szLastPlaceName), &placeName, 18, NULL);
			gameData.playerData.push_back({ entity, idx, steamID, bot, hltv, name , health, armor, hasHelmet, hasDefuser, teamNumber, money, weaponID, placeName, dormant });

#pragma region Player Resource

			const int& competitiveRanking = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveRanking + 0x4 + idx * 4);
			char buff[16];
			ReadProcessMemory(mem.processHandle, (LPCVOID)(IPlayerResource.address + Offset::netvars::m_szClan + 0x10 + (idx * 0x10)), buff, 16, NULL);

			gameData.playerResource.competitiveRanking[idx] = std::clamp(competitiveRanking, 0, 18);
			gameData.playerResource.competitiveWins[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveWins + 0x4 + idx * 4);
			gameData.playerResource.kills[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iKills + 0x4 + idx * 4);
			gameData.playerResource.assists[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iAssists + 0x4 + idx * 4);
			gameData.playerResource.deaths[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iDeaths + 0x4 + idx * 4);
			gameData.playerResource.MVPs[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iMVPs + 0x4 + idx * 4);
			gameData.playerResource.score[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iScore + 0x4 + idx * 4);
			gameData.playerResource.ping[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iPing + 0x4 + idx * 4);
			gameData.playerResource.activeCoinRank[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_nActiveCoinRank + 0x4 + idx * 4);
			gameData.playerResource.compTeammateColor[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompTeammateColor + 0x4 + idx * 4);
			gameData.playerResource.musicID[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_nMusicID + 0x4 + idx * 4);
			gameData.playerResource.clanTag[idx] = buff;

#pragma endregion Player Resource

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
		case ClassID::BaseCSGrenadeProjectile: {
			const int& modelIndex = entity->modelIndex();
			if (Skin::getModelIndex("models/Weapons/w_eq_flashbang_dropped.mdl") == modelIndex)
				gameData.projectileData.push_back({ entity, "Flashbang" });
			if (Skin::getModelIndex("models/Weapons/w_eq_fraggrenade_dropped.mdl") == modelIndex)
				gameData.projectileData.push_back({ entity, "Explosive Grenade" });
			break;
		}
				
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