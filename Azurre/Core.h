#pragma once

#include "../Lib/imgui/imgui.h"

#define IClient interfaces->client
#define IEngine interfaces->engine
#define IClientState interfaces->clientState
#define IPlayerResource interfaces->playerResource
#define IConsole interfaces->console

struct ImVec2;

namespace Core {
	void init();
	void update();
	void entityDataUpdate() noexcept;
	void _() noexcept;
};

#include "Memory.h"

inline bool showMenu =
#if _DEBUG
false;
#else
true;
#endif;
inline int gameState = 0;
inline ImVec2 screenSize;
inline ImVec2 gameScreenSize = {};
inline ImVec2 gameScreenPos = {};
inline ImVec2 gameScreenPosEnd = {};

inline Memory csgo = Memory{ "csgo.exe" };