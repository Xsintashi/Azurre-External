#pragma once

#include "../Lib/imgui/imgui.h"
#include "Memory.h"
#include "SDK/Matrix.h"
#include "SDK/BspParser.h"
#include "SDK/PlayerInfo.h"

#include <string>
#include <vector>

#ifdef _DEBUG
#define THREAD_LOOP					GUI::isRunning
#else
#define THREAD_LOOP					!IsDebuggerPresent() && GUI::isRunning
#endif


#define MAX_EDICT_BITS				11							// B
#define NUM_ENT_ENTRY_BITS			(MAX_EDICT_BITS + 1)		// C
#define NUM_ENT_ENTRIES				(1 << NUM_ENT_ENTRY_BITS)	// 0x1000
#define ENT_ENTRY_MASK				(NUM_ENT_ENTRIES - 1)		// 0xFFF
#define INVALID_EHANDLE_INDEX		0xFFFFFFFF					// W chuj duzo

#define PLAYER_EYE_HEIGHT			64.093811f
#define PLAYER_EYE_HEIGHT_CROUCH	46.076218f
#define PLAYER_HEIGHT				72.0f
#define PLAYER_HEIGHT_CROUCH		54.0f

#define IClient						interfaces->client
#define IEngine						interfaces->engine
#define IClientState				interfaces->clientState
#define IVstdlib					interfaces->vstdlib
#define IPlayerResource				interfaces->playerResource
#define IConsole					interfaces->console
#define IGameRules					interfaces->gameRulesProxy

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
	int idx;
	PlayerInfo playerInfo;
};

struct PlayerResource {
	int competitiveRanking[64];
	int competitiveWins[64];
	int kills[64];
	int assists[64];
	int deaths[64];
	int MVPs[64];
	int score[64];
	int ping[64];
	int activeCoinRank[64];
	int compTeammateColor[64];
	int musicID[64];
	std::string clanTag[64];
};

struct ProjectileData {
	Entity* entity;
	const char* name;
};

struct DangerZoneData {
	std::vector<Entity*> drone;
	std::vector<Entity*> ammoBox;
	std::vector<Entity*> cash;
	std::vector<Entity*> dronegun;
	std::vector<Entity*> healthshots;
	std::vector<ProjectileData> lootCases;
};

struct GameData {
	std::vector<PlayerData> playerData;
	PlayerResource playerResource;
	std::vector<Entity*> weaponData;
	std::vector<ProjectileData> projectileData;
	std::vector<Entity*> defuseKits;
	std::vector<ObserverData> observerData;
	Entity* tablet;
	Entity* toneMapController;
	Entity* plantedC4;
	Entity* droppedC4;
	std::string defusingPlayerName;
	DangerZoneData dangerZoneData;
};

struct TextureInfo {
	int width;
	int height;
	PDIRECT3DTEXTURE9 data;
};

inline WORD equipment[8] = { 0,0,0,0,0,0,0,0 };
inline int localPlayerIndex;
inline bool showMenu = false;
inline std::string mapName = "";
inline std::string gameDir = "";
inline std::string playerName = "";
inline ConnectionState gameState = ConnectionState::None;
inline ImVec2 screenSize;
inline ImVec2 gameScreenSize = {};
inline ImVec2 gameScreenPos = {};
inline ImVec2 gameScreenPosEnd = {};
inline GameData gameData;
inline Matrix4x4 viewMatrix;
inline bool isDangerZoneModePlayed;
inline float serverTime;
inline int maxEntity;
inline int highestEntityIndex;
inline bool parsedMap = false;
inline rn::bsp_parser* bspParser = new rn::bsp_parser;
#define mem azurre
inline Memory mem = Memory{ "csgo.exe" };