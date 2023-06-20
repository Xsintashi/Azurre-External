#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Lib/imgui/imgui.h"

#include "ConfigStructs.h"
#include "InputUtil.h"

class Config {
public:
    Config() noexcept;
    void load(size_t, bool incremental) noexcept;
    void load(const char8_t* name, bool incremental) noexcept;
    void save(size_t) const noexcept;
    void add(const char*) noexcept;
    void remove(size_t) noexcept;
    void rename(size_t, const char*) noexcept;
    void reset() noexcept;
    void listConfigs() noexcept;
    void createConfigDir() const noexcept;
    void openConfigDir() const noexcept;

    constexpr auto& getConfigs() noexcept
    {
        return configs;
    }

	struct AimbotConfig {
		bool enabled = false;
		KeyBind hotkey = std::string("Aimbot");
		bool friendlyFire = false;
		bool autoShot = false;
		bool autoStop = false;
		bool disableWhileBeingSpectated = false;
		bool visibleOnly = false;
		int bone = 0;
		float fov = 0.f;
		float smooth = 1.f;
		bool rcs = false;
		ColorToggleOutline drawFov{ 1.0f, 1.0f, 1.0f, 0.25f };
	} a;

	struct ChamsConfig {
		bool enabled = false;
		float brightness = 0.1f;
		ColorToggle3 enemy = { 0.f, 0.4f, 1.f };
		ColorToggle3 ally = { 1.f, 0.f, 0.f };
	} c;

	struct ClanTagConfig {
		struct CustomClanTag {
			std::string prefix = "";
			std::string tag = "";
			std::string teamTag = "";
			std::string postfix = "";
			bool hideName = false;
			int type = 0;
			float speed = 0.5f;
		} custom;
		int mode = 0;
	} clanTag;

	struct DiscordConfig {
		bool enabled = true;
		bool showMap = true;
		bool showMatchTime = true;
		int state = 0;
		int details = 0;
	} d;

	struct ESPConfig {
		bool enabled = false;
		std::unordered_map<std::string, Player> players;
		std::unordered_map<std::string, Weapons> weapons;
		std::unordered_map<std::string, Projectiles> projectiles;
	} esp;

	struct GlowConfig {
		bool enabled = false;
		ColorToggle enemy = { 0.f, 0.4f, 1.f, 1.f };
		ColorToggle ally = { 1.f, 0.f, 0.f, 1.f };
	} g;

	struct MiscConfig {
		KeyBind menuKey = KeyBind::INSERT;
		bool fixTablet = false;
		bool bhop = false;
		bool radarHack = false;
		bool autoStop = false;
		bool grenadeTrajectory = false;
		Crosshair sniperCrosshair;
		Crosshair recoilCrosshair;
		struct KeyBindsList {
			bool enabled = false;
			bool noTitleBar = false;
			bool noBackground = false;
			ImVec2 pos;
		}keybinds;

		struct PlayerList {
			bool enabled = false;
			bool noTitleBar = false;
			bool hideLocalPlayer = false;
			bool hideDormant = false;
			int sort = 0;
			KeyBind hotkey = std::string("Player List");
			ImVec2 pos;
			Color4 localPlayerColor = { 1.0f, 0.25f, 1.0f, 1.f };
		} playerList;

		struct BombTimer {
			bool enabled = false;
			bool noTitleBar = false;
			Color3 barColor{ 0.f, 1.f, 0.f };
			ImVec2 pos;
		} bombTimer;

		struct SpectatorList {
			bool enabled = false;
			bool noTitleBar = false;
			bool noBackground = false;
			ImVec2 pos;
		} spectatorList;

		struct Minimap {
			bool enabled = false;
			bool showPlayers = true;
			bool showDormant = false;
			bool showWeapons = false;
			bool showGrenades = false;
			bool noTitleBar = false;
			bool noBackground = false;
			int colorScheme = 0;
			float scale = 1.f;
			KeyBind hotkey = std::string("Minimap");
			ImVec2 pos;
		} minimap;

		struct FakeLag {
			bool enabled = false;
			int limit = 0;
			int type = 0;
		} fakeLag;

		Marker hitMarker;
		Marker killMarker;

		std::string hitSound = "sound\\";
		std::string killSound = "sound\\";
	} m;

	struct TriggerBotConfig {
		bool enabled = false;
		bool friendlyFire = false;
		int delay = 0;
		KeyBind hotkey = std::string("Triggerbot");
	} t;

	struct GuiConfig {
		bool antiAliasing = true;
		bool windowBorder = false;
		bool centerTitle = true;
		bool frameBorder = false;
		bool roundBorder = false;

		int menuColors = 0;
	} u;

	struct VisualsConfig {
		bool thirdPerson = false;
		KeyBind thirdPersonKey = std::string("Thirdperson");
		bool noParticles = false;
		bool noAllies = false;
		bool no3DSky = false;
		bool noShadows = false;
		bool noPanoramaBlur = false;
		int flashReduction = 0;
		int fov = 90;
		int skybox = 0;
		struct CustomPostProcessing {
			bool enabled = false;
			float worldExposure = 0.0f;
			float bloomScale = 0.0f;
		} customPostProcessing;
	} v;

    struct SkinChangerConfig {
		short weaponID = -1;
		int skinID = 0;
		float wear = 0.0f;
		int seed = 0;
		int quality = 0;
		int statTrak = 0;
		char nameTag[20] = "";
    };

	struct ChangerConfig{
		int CTKnife = 0;
		int TTKnife = 1;
		int CTAgent = 0;
		int TTAgent = 0;
	}ch;

	std::array<SkinChangerConfig, 56> s;

	struct Debug {
		bool boolDebug0 = false;
		KeyBind keyDebug0 = std::string("Debug 0");
	} debug;

#if defined(_DEBUG)
	int restrictions = 0;
#else
	int restrictions = 1;
#endif

private:
    std::filesystem::path path;
    std::vector<std::string> configs;
};

inline std::optional<Config> cfg;