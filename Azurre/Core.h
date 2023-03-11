#pragma once

#define IClient interfaces->client
#define IEngine interfaces->engine
#define IClientState interfaces->clientState
#define IPlayerResource interfaces->playerResource
#define IConsole interfaces->console

namespace Core {
	void init();
	void update();
};

#include "Memory.h"

inline Memory csgo = Memory{ "csgo.exe" };