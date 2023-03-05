#pragma once
#include <optional>

class Config {
public:
	struct MiscConfig {
		bool bhop = false;
		bool thirdPerson = false;
		int flashReduction = 0;
	}m;
};

inline std::optional<Config> cfg;