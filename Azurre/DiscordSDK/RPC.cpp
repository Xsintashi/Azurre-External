#include "RPC.h"

#include "include/discord_register.h"
#include "include/discord_rpc.h"

#include "../Config.h"
#include "../SDK/GlobalVars.h"

#include <string>

void Discord::Run() {
	DiscordEventHandlers Handler;
	memset(&Handler, 0, sizeof(Handler));
	Discord_Initialize("918239241299972156", &Handler, 1, NULL);

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
	discordPresence.state = "Azurre External 0.1";
	discordPresence.details = "Soon I Guess";
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
