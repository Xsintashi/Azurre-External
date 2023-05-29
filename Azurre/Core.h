#pragma once

#include "../Lib/imgui/imgui.h"
#include "Memory.h"
#include "SDK/Matrix.h"

#include <string>
#include <vector>

#define MAX_EDICT_BITS			11							// B
#define NUM_ENT_ENTRY_BITS		(MAX_EDICT_BITS + 1)		// C
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)	// 0x1000
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)		// 0xFFF
#define INVALID_EHANDLE_INDEX	0xFFFFFFFF					// W chuj duzo

#define IClient interfaces->client
#define IEngine interfaces->engine
#define IClientState interfaces->clientState
#define IPlayerResource interfaces->playerResource
#define IConsole interfaces->console

struct ImVec2;
class Entity;
enum class ObsMode;

enum ConnectionState : int {
	None,
	Challenge,
	Connected, //loading screen
	New,
	Prespawn,
	Spawn,
	FullyConnected, //fully connected to the game
	ChangeLevel
};

namespace Core {
	void init();
	void update();
	void gameDataUpdate() noexcept;
	void _() noexcept;
};

struct ObserverData {
	std::string name;
	ObsMode obsMode;
};

struct PlayerData {
	Entity* entity;
	int unsigned idx;
	std::uint64_t steamID;
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
	std::vector<ObserverData> observerData;
	Entity* tablet;
	Entity* toneMapController;
	Entity* plantedC4;
	std::string defusingPlayerName;
};

inline int localPlayerIndex;
inline bool showMenu = false;
inline std::string mapName = "";
inline std::string gameDir = "";
inline ConnectionState gameState = ConnectionState::None;
inline ImVec2 screenSize;
inline ImVec2 gameScreenSize = {};
inline ImVec2 gameScreenPos = {};
inline ImVec2 gameScreenPosEnd = {};
inline GameData gameData;
inline Matrix4x4 viewMatix;
#define mem azurre
inline Memory mem = Memory{ "csgo.exe" };