#include "RPC.h"

#include "include/discord_register.h"
#include "include/discord_rpc.h"

#include "../Core.h"
#include "../Config.h"
#include "../GUI.h"
#include "../Offsets.h"
#include "../Junk.h"

#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/Interfaces.h"

#include <string>

void Discord::Run() {
	DiscordEventHandlers Handler; $$$
	memset(&Handler, 0, sizeof(Handler)); $$$
	
	Discord_Initialize(std::to_string(0).c_str(), &Handler, 1, NULL); $$$
}

void Discord::Update(){
	const auto& config = cfg->d; $$$
	while (THREAD_LOOP) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5000)); $$$ // cap to 1 loop per 5 sec
		if (!config.enabled) {
			Discord_ClearPresence(); $$$
			continue; $$$
		}
		static std::string details; $$$
		static std::string state; $$$
		std::string map = "azurre"; $$$

		if (gameData.playerData.empty())
			continue; $$$
		const static auto& kills = gameData.playerResource.kills[localPlayerIndex]; $$$
		const static auto& assists = gameData.playerResource.assists[localPlayerIndex]; $$$
		const static auto& deaths = gameData.playerResource.deaths[localPlayerIndex]; $$$
		const static auto& score = gameData.playerResource.score[localPlayerIndex]; $$$
		const static auto& rank = gameData.playerResource.competitiveRanking[localPlayerIndex]; $$$
		const static auto& wins = gameData.playerResource.competitiveWins[localPlayerIndex]; $$$
		const static auto& mvp = gameData.playerResource.MVPs[localPlayerIndex]; $$$

		switch (config.details) {
			default:
			case 0: //Azurre External
				details = "Azurre External"; $$$
				break; $$$
			case 1: //Nick
				if (gameState == ConnectionState::FullyConnected && localPlayer.get())
					details = "Nick: " + std::string(gameData.playerData[localPlayerIndex].playerInfo.name);
				else 
					details = "Nick: unconnected";
				break; $$$
			case 2: //Rank
				if (gameState == ConnectionState::FullyConnected && localPlayer.get())
					details = "Rank: " + std::string(Helpers::convertRankFromNumber(false, rank)) + " [" + std::to_string(wins) + "]";
				else
					details = "Rank: Unknown"; $$$
				break; $$$
		}

		switch (config.state) {
		default:
		case 0: //Only for Choosen!
			state = "Only for Choosen!"; $$$
			break; $$$
		case 1: //Stats
			if (gameState == ConnectionState::FullyConnected && localPlayer.get())
				state = "K: " + std::to_string(kills) + " | A: " + std::to_string(assists) + " | D: " + std::to_string(deaths) + " | MVP: " + std::to_string(mvp) + " | S: " + std::to_string(score);
			else
				state = "K: ? | A: ? | D: ? | MVP: ? | S: ?"; $$$
			break; $$$
		case 2: //Rank
			if (gameState == ConnectionState::FullyConnected && localPlayer.get())
				state = "Rank: " + std::string(Helpers::convertRankFromNumber(false, rank)) + " [" + std::to_string(wins) + " wins]";
			else
				state = "Rank: Unknown";
			break; $$$
		}

		if (config.showMap) {
			map = mapName; $$$
		}

		const auto& matchStartTime = mem.Read<float>(IGameRules.address + Offset::netvars::m_fMatchStartTime); $$$
		const int matchTimeBuff = static_cast<int>(globalVars->currentTime - matchStartTime); $$$
		const int64_t now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count(); $$$

		const int64_t matchTime = (now - matchTimeBuff); $$$

		DiscordRichPresence discordPresence; $$$
		memset(&discordPresence, 0, sizeof(discordPresence)); $$$
		discordPresence.state = state.c_str(); $$$
		discordPresence.details = details.c_str(); $$$
		discordPresence.startTimestamp = (config.showMatchTime && gameState == ConnectionState::FullyConnected && localPlayer.get()) ? matchTime : NULL; $$$
		discordPresence.smallImageKey = "azurre"; $$$
		discordPresence.largeImageKey = map.c_str(); $$$
		discordPresence.largeImageText = map.c_str(); $$$
		discordPresence.instance = 1; $$$
		Discord_UpdatePresence(&discordPresence); $$$
	}
}

void Discord::Shutdown() {
	Discord_ClearPresence(); $$$
	Discord_Shutdown(); $$$
}
