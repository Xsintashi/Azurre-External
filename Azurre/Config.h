#pragma once
#include <optional>

class Config {
public:
	struct MiscConfig {
		bool fixTablet = false;
		bool bhop = false;
	}m;

	struct VisualsConfig {
		bool bhop = false;
		bool thirdPerson = false;
		bool no3DSky = false;
		bool noSmoke = false;
		int flashReduction = 0;
	}v;

	struct DiscordConfig {
		bool enabled = true;
	}d;

	struct TriggerBotConfig {
		bool enabled = false;
	}t;
};

inline std::optional<Config> cfg;