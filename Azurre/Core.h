#pragma once

#define IClient interfaces->client
#define IEngine interfaces->engine
#define IClientState interfaces->clientState

namespace Core {
	void init();
	void update();
};

#include "Memory.h"

inline Memory csgo = Memory{ "csgo.exe" };