#include "RPC.h"

#include "include/discord_register.h"
#include "include/discord_rpc.h"

#include "../Core.h"
#include "../Config.h"
#include "../GUI.h"
#include "../SDK/GlobalVars.h"

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
	while (GUI::isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // cap to 1 loop per 5 sec
		static std::string details;
		static std::string state;

		if (!cfg->d.enabled) {
			Discord_ClearPresence();
			return;
		}

		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = "Azurre External";
		discordPresence.details = "Only for Choosen!";
		discordPresence.endTimestamp = NULL;
		discordPresence.smallImageKey = "azurre";
		discordPresence.largeImageKey = mapName.c_str();
		discordPresence.largeImageText = mapName.c_str();
		discordPresence.instance = 1;
		Discord_UpdatePresence(&discordPresence);
	}
}

void Discord::Shutdown() {
	Discord_ClearPresence();
	Discord_Shutdown();
}
