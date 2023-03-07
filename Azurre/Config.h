#pragma once
#include <optional>


class Config {
public:
	struct MiscConfig {
		bool fixTablet = false;
		bool bhop = false;
		bool radarHack = false;
		bool fakeLag = false;
		int fakeLagLimit = 0;
		int fakeLagType = 0;
	}m;

	struct VisualsConfig {
		bool bhop = false;
		bool thirdPerson = false;
		bool no3DSky = false;
		bool noSmoke = false;
		bool noAllies = false;
		int flashReduction = 0;
	}v;

	struct AimbotConfig {
		bool enabled = false;
		bool autoShot = false;
		bool autoStop = false;
		int bone = 0;
		float fov = 0.f;
		float smooth = 1.f;
		bool rcs = false;

	}a;

	struct DiscordConfig {
		bool enabled = true;
	}d;

	struct ChamsConfig {
		bool enabled = false;
		float brightness = 0.1f;
		float enemy[3] = {0.f, 0.4f, 1.f};
		float ally[3] = { 1.f, 0.f, 0.f };
	}c;

	struct Glow {
		bool enabled = false;
		float enemy[4] = { 0.f, 0.4f, 1.f, 1.f };
		float ally[4] = { 1.f, 0.f, 0.f, 1.f };
	}g;

	struct Debug {
		bool boolDebug0 = false;
	} debug;

	struct TriggerBotConfig {
		bool enabled = false;
	}t;
};

inline std::optional<Config> cfg;