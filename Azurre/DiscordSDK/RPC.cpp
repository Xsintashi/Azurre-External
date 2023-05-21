#include "RPC.h"

#include "include/discord_register.h"
#include "include/discord_rpc.h"

#include "../Config.h"
#include "../SDK/GlobalVars.h"

#include <string>

void Discord::Run() {
	DiscordEventHandlers Handler;
	memset(&Handler, 0, sizeof(Handler));

	constexpr __int64 __s___a__f___453y__s8ohfa34 = 749755080889974835;
	constexpr __int64 u98u98u98u98u98u98u98u98u12 = 493592757869187087;

	constexpr __int64 ID = __s___a__f___453y__s8ohfa34 ^ u98u98u98u98u98u98u98u98u12;

	Discord_Initialize(std::to_string(ID).c_str(), &Handler, 1, NULL);

	Update();
}

void Discord::Update(){

	static float compare = 0;
	static std::string details;
	static std::string state;

	if (compare + 5.f > globalVars->realTime) return;

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
	discordPresence.largeImageKey = "azurre";
	discordPresence.largeImageText = "azurre";
	discordPresence.instance = 1;
	Discord_UpdatePresence(&discordPresence);
	compare = globalVars->realTime;

}

void Discord::Shutdown() {
	Discord_ClearPresence();
	Discord_Shutdown();
}
