#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

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
		KeyBind hotkey = "aimbot";
		bool autoShot = false;
		bool autoStop = false;
		int bone = 0;
		float fov = 0.f;
		float smooth = 1.f;
		bool rcs = false;
	} a;

	struct ChamsConfig {
		bool enabled = false;
		float brightness = 0.1f;
		ColorToggle3 enemy = { 0.f, 0.4f, 1.f };
		ColorToggle3 ally = { 1.f, 0.f, 0.f };
	} c;

	struct DiscordConfig {
		bool enabled = true;
	} d;

	struct GlowConfig {
		bool enabled = false;
		ColorToggle enemy = { 0.f, 0.4f, 1.f, 1.f };
		ColorToggle ally = { 1.f, 0.f, 0.f, 1.f };
	} g;

	struct MiscConfig {
		bool fixTablet = false;
		bool bhop = false;
		bool radarHack = false;
		bool autoStop = false;
		struct FakeLag {
			bool enabled = false;
			int limit = 0;
			int type = 0;
		} fakeLag;
	} m;

	struct TriggerBotConfig {
		bool enabled = false;
		KeyBind hotkey = "trigger";
	} t;

	struct VisualsConfig {
		bool thirdPerson = false;
		KeyBind thirdPersonKey = "thirdperson";
		bool noSmoke = false;
		bool noAllies = false;
		int flashReduction = 0;
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
		char nameTag[161] = "";
    };

	std::array<SkinChangerConfig, 36> s;

	struct Debug {
		bool boolDebug0 = false;
	} debug;

private:
    std::vector<std::string> scheduledFonts{ "Default" };
    std::vector<std::string> systemFonts{ "Default" };
    std::filesystem::path path;
    std::vector<std::string> configs;
};

inline std::optional<Config> cfg;