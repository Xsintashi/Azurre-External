#pragma once

#include "../Lib/imgui/imgui.h"

#include <string>
#include <vector>

#define IClient interfaces->client
#define IEngine interfaces->engine
#define IClientState interfaces->clientState
#define IPlayerResource interfaces->playerResource
#define IConsole interfaces->console

struct ImVec2;
class Entity;

namespace Core {
	void init();
	void update();
	void gameDataUpdate() noexcept;
	void _() noexcept;
};

struct PlayerData {
	Entity* entity;
	int unsigned idx;
	const char* steamID;
	bool isBot;
	std::string name;
	int health;
	int armor;
	bool hasHelmet;
	bool hasDefuser;
	int teamNumber;
	int money;
	int weaponID;
	std::string placename;
	int rank;
	int wins;
};

struct GameData {
	std::vector<PlayerData> playerData;
	Entity* tablet;
	Entity* toneMapController;
	Entity* plantedC4;
};

#include "Memory.h"

inline bool showMenu = false;
inline std::string mapName = "";
inline std::string gameDir = "";
inline int gameState = 0;
inline ImVec2 screenSize;
inline ImVec2 gameScreenSize = {};
inline ImVec2 gameScreenPos = {};
inline ImVec2 gameScreenPosEnd = {};
inline GameData gameData;
inline Memory csgo = Memory{ "csgo.exe" };