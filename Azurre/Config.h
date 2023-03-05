#pragma once
#include <optional>

class Config {
public:
	struct MiscConfig {
		bool bhop = false;
	}m;
};

inline std::optional<Config> cfg;