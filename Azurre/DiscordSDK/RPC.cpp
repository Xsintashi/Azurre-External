#include "RPC.h"

#include "include/discord_register.h"
#include "include/discord_rpc.h"

#include "../Core.h"
#include "../Config.h"
#include "../GUI.h"
#include "../Offsets.h"

#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/Interfaces.h"

#include <string>

void Discord::Run() {
	DiscordEventHandlers Handler;
	memset(&Handler, 0, sizeof(Handler));

	constexpr __int64 __s___a__f___453y__s8ohfa34 = 749755080889974835;
	constexpr __int64 u98u98u98u98u98u98u98u98u12 = 493592757869187087;

	constexpr __int64 ID = __s___a__f___453y__s8ohfa34 ^ u98u98u98u98u98u98u98u98u12;

	Discord_Initialize(std::to_string(ID).c_str(), &Handler, 1, NULL);
}

void Discord::Update(){
	auto& config = cfg->d;
	while (GUI::isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // cap to 1 loop per 5 sec
		if (!config.enabled) {
			Discord_ClearPresence();
			continue;
		}
		static std::string details;
		static std::string state;
		std::string map = "azurre";

		if (gameData.playerData.empty())
			continue;

		const auto& kills = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iKills + 0x4 + localPlayerIndex * 4);
		const auto& assists = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iAssists + 0x4 + localPlayerIndex * 4);
		const auto& deaths = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iDeaths + 0x4 + localPlayerIndex * 4);
		const auto& score = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iScore + 0x4 + localPlayerIndex * 4);
		const auto& rank = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveRanking + 0x4 + localPlayerIndex * 4);
		const auto& wins = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveWins + 0x4 + localPlayerIndex * 4);
		const auto& mvp = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iMVPs + 0x4 + localPlayerIndex * 4);

		switch (config.details) {
			default:
			case 0: //Azurre External
				details = "Azurre External";
				break;
			case 1: //Nick
				if (gameState == ConnectionState::FullyConnected && localPlayer.get())
					details = "Nick: " + gameData.playerData[localPlayerIndex].name;
				else 
					details = "Nick: unconnected";
				break;
			case 2: //Rank
				if (gameState == ConnectionState::FullyConnected && localPlayer.get())
					details = "Rank: " + std::string(Helpers::convertRankFromNumber(false, rank)) + " [" + std::to_string(wins) + "]";
				else
					details = "Rank: Unknown";
				break;
		}

		switch (config.state) {
		default:
		case 0: //Only for Choosen!
			state = "Only for Choosen!";
			break;
		case 1: //Stats
			if (gameState == ConnectionState::FullyConnected && localPlayer.get())
				state = "K: " + std::to_string(kills) + " | A: " + std::to_string(assists) + " | D: " + std::to_string(deaths) + " | MVP: " + std::to_string(mvp) + " | S: " + std::to_string(score);
			else
				state = "K: ? | A: ? | D: ? | MVP: ? | S: ?";
			break;
		case 2: //Rank
			if (gameState == ConnectionState::FullyConnected && localPlayer.get())
				state = "Rank: " + std::string(Helpers::convertRankFromNumber(false, rank)) + " [" + std::to_string(wins) + "]";
			else
				state = "Rank: Unknown";
			break;
		}

		if (config.showMap) {
			map = mapName;
		}

		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = state.c_str();
		discordPresence.details = details.c_str();
		discordPresence.endTimestamp = NULL;
		discordPresence.smallImageKey = "azurre";
		discordPresence.largeImageKey = map.c_str();
		discordPresence.largeImageText = map.c_str();
		discordPresence.instance = 1;
		Discord_UpdatePresence(&discordPresence);
	}
}

void Discord::Shutdown() {
	Discord_ClearPresence();
	Discord_Shutdown();
}
