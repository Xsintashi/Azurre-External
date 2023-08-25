#include "Memory.h"
#include "Core.h"
#include "GUI.h"
#include "Offsets.h"
#include "Config.h"
#include "Console.h"
#include "TextureManager.h"
#include "Junk.h"

#include "Hacks/Aimbot.h"
#include "Hacks/Clantag.h"
#include "Hacks/Minimap.h"
#include "Hacks/Misc.h"
#include "Hacks/SkinChanger.h"
#include "Hacks/Visuals.h"


#include "SDK/LocalPlayer.h"
#include "SDK/Interfaces.h"
#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/PlayerInfo.h"
#include "SDK/Matrix.h"

#include "DiscordSDK/RPC.h"

#include <filesystem>
#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <intrin.h>

void Core::init() {
	SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST); $$$
	IClient.address = mem.GetModuleAddress("client.dll"); $$$
	IClient.size = mem.ModuleSize("client.dll"); $$$
	cmd.AddLog("client.dll: 0x%p, sizeof: %d", IClient.address, IClient.size); $$$

	IEngine.address = mem.GetModuleAddress("engine.dll"); $$$
	IEngine.size = mem.ModuleSize("engine.dll"); $$$
	cmd.AddLog("engine.dll: 0x%p, sizeof: %d", IEngine.address, IEngine.size); $$$

	IVstdlib.address = mem.GetModuleAddress("vstdlib.dll"); $$$
	IVstdlib.size = mem.ModuleSize("vstdlib.dll"); $$$
	cmd.AddLog("vstdlib.dll: 0x%p, sizeof: %d", IVstdlib.address, IVstdlib.size); $$$

	IClientState.address = mem.Read<uintptr_t>(IEngine.address + Offset::signatures::dwClientState); $$$
	IGameRules.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwGameRulesProxy); $$$
	IPlayerResource.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwPlayerResource); $$$
	localPlayer.init(mem.Read<Entity*>(IClient.address + Offset::signatures::dwLocalPlayer)); $$$
	cmd.AddLog("localPlayer: 0x%p", localPlayer.get()); $$$
	gameState = mem.Read<ConnectionState>(IClientState.address + Offset::signatures::dwClientState_State); $$$
	const auto dir = mem.Read<std::array<char, 128>>(IEngine.address + Offset::signatures::dwGameDir); $$$
	gameDir = dir.data(); $$$

	const auto buffer = mem.Read<std::array<char, 128>>(IEngine.address + Offset::signatures::m_szSteamName); $$$ //Names are 128 length long
	playerName = buffer.data(); $$$ // We don't want zeros cuz we have to delete unwanted " in next step
	playerName.pop_back(); $$$ //Name comes with " character, so we want to remove that cuz its not included in names
}

void Core::update() {
	IConsole = FindWindowA("Valve001", NULL); $$$
	gameState = mem.Read<ConnectionState>(IClientState.address + Offset::signatures::dwClientState_State); $$$
	IClientState.address = mem.Read<uintptr_t>(IEngine.address + Offset::signatures::dwClientState); $$$
	IPlayerResource.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwPlayerResource); $$$
	IGameRules.address = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwGameRulesProxy); $$$
	localPlayer.init(mem.Read<Entity*>(IClient.address + Offset::signatures::dwLocalPlayer)); $$$
	globalVars = mem.Read<GlobalVars>(IEngine.address + Offset::signatures::dwGlobalVars); $$$
	serverTime = localPlayer->tickBase() * globalVars->intervalPerTick; $$$
	viewMatrix = mem.Read<Matrix4x4>(IClient.address + Offset::signatures::dwViewMatrix); $$$
	maxEntity = mem.Read<int>(IClient.address + Offset::signatures::dwEntityList + 0x2001C); $$$
	highestEntityIndex = mem.Read<int>(IClient.address + Offset::signatures::dwEntityList + 0x20024); $$$
	localPlayerIndex = mem.Read<int>(IClientState.address + Offset::signatures::dwClientState_GetLocalPlayer); $$$
	isDangerZoneModePlayed = mem.Read<DWORD>(IGameRules.address + Offset::netvars::m_SurvivalGameRuleDecisionTypes); $$$
	screenSize = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) }; $$$
	const auto map = mem.Read<std::array<char, 128>>(IClientState.address + Offset::signatures::dwClientState_Map); $$$
	mapName = map.data(); $$$
	static std::string mapBuff; $$$
	if (mapBuff != mapName) {
		parsedMap = false; $$$
		mapBuff = mapName; $$$
	}
	if (!parsedMap) {
		if (bspParser->loadMap(std::string(gameDir).append("\\maps").c_str(), mapName.append(".bsp").c_str())) {
			cmd.AddLog("~[!] Map Parsed!"); $$$
			parsedMap = true; $$$
			return; $$$
		} else
			cmd.AddLog("~[!] Couldn't parse map!"); $$$
		
	}
	RECT rct; $$$
	if (GetWindowRect(IConsole, &rct)) {
		gameScreenPos = { static_cast<float>(rct.left) , static_cast<float>(rct.top) }; $$$
		gameScreenPosEnd = { gameScreenPos.x + gameScreenSize.x , gameScreenPos.y + gameScreenSize.y }; $$$
		gameScreenSize = { static_cast<float>(rct.right - rct.left), static_cast<float>(rct.bottom - rct.top) }; $$$
	}

	cfg->a.hotkey.handleToggle(); $$$
	cfg->m.playerList.hotkey.handleToggle(); $$$
	cfg->m.minimap.hotkey.handleToggle(); $$$
	cfg->m.slowWalk.hotkey.handleToggle(); $$$
	cfg->t.hotkey.handleToggle(); $$$
	cfg->v.thirdPersonKey.handleToggle(); $$$
#if defined(_DEBUG)
	cfg->debug.keyDebug0.handleToggle(); $$$
#endif

}

void Core::gameDataUpdate() noexcept {
	const auto& userInfoTable = mem.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo); $$$
	const auto& items = mem.Read<uintptr_t>(mem.Read<uintptr_t>(userInfoTable + 0x40) + 0xC); $$$
	gameData.playerData.clear(); $$$
	gameData.weaponData.clear(); $$$
	gameData = {}; $$$ // reset GameData

	static int flash; $$$
	static int nade; $$$
	static int pistol; $$$
	static int light; $$$
	static int heavy; $$$
	static int explosive; $$$
	static int tools; $$$
	static int bag; $$$

	static bool needUpdate = true;
	if (needUpdate && gameState == ConnectionState::FullyConnected){
		flash = Skin::getModelIndex("models/Weapons/w_eq_flashbang_dropped.mdl"); $$$
		nade = Skin::getModelIndex("models/Weapons/w_eq_fraggrenade_dropped.mdl"); $$$
		pistol = Skin::getModelIndex("models/props_survival/cases/case_pistol.mdl"); $$$
		light = Skin::getModelIndex("models/props_survival/cases/case_light_weapon.mdl"); $$$
		heavy = Skin::getModelIndex("models/props_survival/cases/case_heavy_weapon.mdl"); $$$
		explosive = Skin::getModelIndex("models/props_survival/cases/case_explosive.mdl"); $$$
		tools = Skin::getModelIndex("models/props_survival/cases/case_tools.mdl"); $$$
		bag = Skin::getModelIndex("models/props_survival/cash/dufflebag.mdl"); $$$
		needUpdate = false; $$$
	}
	if (!needUpdate && gameState != ConnectionState::FullyConnected)
		needUpdate = true; $$$

	for (int idx = 0; idx <= highestEntityIndex; idx++) {

		const auto& entity = getEntity(idx); $$$
		if (!entity) continue; $$$

		if (entity->isWeapon())
			gameData.weaponData.push_back(entity); $$$

		switch (GetClassId(entity)) {
		default:
			break; $$$
		case ClassID::CSPlayer: {

			if (cfg->m.radarHack && !cfg->restrictions && !entity->isSameTeam())
				mem.Write<bool>(entity + Offset::netvars::m_bSpotted, true); $$$

			// Player Info
			PlayerInfo playerInfo = mem.Read<PlayerInfo>(mem.Read<uintptr_t>(items + 0x28 + (idx * 0x34))); $$$
			const std::string name = playerInfo.name; $$$
			playerInfo.steamID64 = _byteswap_uint64(playerInfo.steamID64); $$$ // Little endian -> Big endian >:[
			gameData.playerData.push_back({ entity, idx, playerInfo}); $$$

#pragma region Player Resource

			const int& competitiveRanking = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveRanking + 0x4 + idx * 4); $$$
			char buff[16]; $$$
			ReadProcessMemory(mem.processHandle, (LPCVOID)(IPlayerResource.address + Offset::netvars::m_szClan + 0x10 + (idx * 0x10)), buff, 16, NULL); $$$

			gameData.playerResource.competitiveRanking[idx] = std::clamp(competitiveRanking, 0, 18); $$$
			gameData.playerResource.competitiveWins[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveWins + 0x4 + idx * 4); $$$
			gameData.playerResource.kills[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iKills + 0x4 + idx * 4); $$$
			gameData.playerResource.assists[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iAssists + 0x4 + idx * 4); $$$
			gameData.playerResource.deaths[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iDeaths + 0x4 + idx * 4); $$$
			gameData.playerResource.MVPs[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iMVPs + 0x4 + idx * 4); $$$
			gameData.playerResource.score[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iScore + 0x4 + idx * 4); $$$
			gameData.playerResource.ping[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iPing + 0x4 + idx * 4); $$$
			gameData.playerResource.activeCoinRank[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_nActiveCoinRank + 0x4 + idx * 4); $$$
			gameData.playerResource.compTeammateColor[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompTeammateColor + 0x4 + idx * 4); $$$
			gameData.playerResource.musicID[idx] = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_nMusicID + 0x4 + idx * 4); $$$
			gameData.playerResource.clanTag[idx] = buff; $$$

#pragma endregion Player Resource

			if (entity->isDefusing()) //Bomb Timer
				gameData.defusingPlayerName = name; $$$

			// Spectator List
			const int obs = (entity->observerTarget() & ENT_ENTRY_MASK); $$$
			if (entity->isDead() && obs == localPlayerIndex + 1) // Ghetto way, hope it will work (me before testing on valve servers)
				gameData.observerData.push_back({ name, playerInfo.steamID64, entity->observerMode()}); $$$

			break; $$$
		}
		case ClassID::Tablet: {
			gameData.tablet = entity; $$$
			if (cfg->m.fixTablet)
				mem.Write<bool>(gameData.tablet + Offset::netvars::m_bTabletReceptionIsBlocked, false); $$$
			break; $$$
		}
		case ClassID::ToneMapController: {
			gameData.toneMapController = entity; $$$
			if (cfg->v.customPostProcessing.enabled) {
				mem.Write<bool>(entity + Offset::netvars::m_bUseCustomBloomScale, cfg->v.customPostProcessing.enabled); $$$
				mem.Write<bool>(entity + Offset::netvars::m_bUseCustomAutoExposureMax, cfg->v.customPostProcessing.enabled); $$$
				mem.Write<bool>(entity + Offset::netvars::m_bUseCustomAutoExposureMin, cfg->v.customPostProcessing.enabled); $$$

				float bloomScale = cfg->v.customPostProcessing.bloomScale * 0.01f; $$$
				float worldExposure = cfg->v.customPostProcessing.worldExposure * 0.001f; $$$

				mem.Write<float>(entity + Offset::netvars::m_flCustomBloomScale, bloomScale); $$$
				mem.Write<float>(entity + Offset::netvars::m_flCustomAutoExposureMax, worldExposure); $$$
				mem.Write<float>(entity + Offset::netvars::m_flCustomAutoExposureMin, worldExposure); $$$
			}
			break; $$$
		}
		case ClassID::PlantedC4:
			gameData.plantedC4 = entity; $$$
			break; $$$
		case ClassID::C4:
			gameData.droppedC4 = entity; $$$
			break; $$$
		case ClassID::BaseCSGrenadeProjectile: {
			const int& modelIndex = entity->modelIndex(); $$$
			if (flash == modelIndex)
				gameData.projectileData.push_back({ entity, "Flashbang" }); $$$
			if (nade == modelIndex)
				gameData.projectileData.push_back({ entity, "Explosive Grenade" }); $$$
			break; $$$
		}
		case ClassID::DecoyProjectile:
			gameData.projectileData.push_back({ entity, "Decoy" }); $$$
			break; $$$
		case ClassID::MolotovProjectile:
			gameData.projectileData.push_back({ entity, "Molotov" }); $$$
			break; $$$
		case ClassID::SmokeGrenadeProjectile:
			gameData.projectileData.push_back({ entity, "Smoke" }); $$$
			break; $$$
		case ClassID::EconEntity:
			gameData.defuseKits.push_back(entity); $$$
			break; $$$
		case ClassID::AmmoBox:
			gameData.dangerZoneData.ammoBox.push_back(entity); $$$
			break; $$$
		case ClassID::Drone:
			gameData.dangerZoneData.drone.push_back(entity); $$$
			break; $$$
		case ClassID::Cash:
			gameData.dangerZoneData.cash.push_back(entity); $$$
			break; $$$
		case ClassID::Healthshot:
			gameData.dangerZoneData.healthshots.push_back(entity); $$$
			break; $$$
		case ClassID::Dronegun:
			gameData.dangerZoneData.dronegun.push_back(entity); $$$
			break; $$$
		case ClassID::LootCrate: {
			const int& modelIndex = entity->modelIndex(); $$$

			if (pistol == modelIndex) {
				gameData.dangerZoneData.lootCases.push_back({ entity, "Pistol Case" }); $$$
				break; $$$
			}
			if (light == modelIndex) {
				gameData.dangerZoneData.lootCases.push_back({ entity, "Light Case" }); $$$
				break; $$$
			}
			if (heavy == modelIndex) {
				gameData.dangerZoneData.lootCases.push_back({ entity, "Heavy Case" }); $$$
				break; $$$
			}
			if (explosive == modelIndex) {
				gameData.dangerZoneData.lootCases.push_back({ entity, "Explosive Case" }); $$$
				break; $$$
			}
			if (tools == modelIndex) {
				gameData.dangerZoneData.lootCases.push_back({ entity, "Tools Case" }); $$$
				break; $$$
			}
			if (bag == modelIndex) {
				gameData.dangerZoneData.lootCases.push_back({ entity, "Cash Dufflebag" }); $$$
				break; $$$
			}

			break; $$$
		}
		}
	}
}

void Core::_() noexcept {
	while (THREAD_LOOP) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); $$$
		update(); $$$
		if (!cfg->restrictions) {
			Aimbot::recoilSystem(); $$$
			Misc::fakeLag(); $$$
			Misc::fastStop(); $$$
			Visuals::noFlash(); $$$
			Visuals::thirdperson(); $$$
			Visuals::fov(); $$$
		}
	}
}