#pragma once
#include <optional>

class Memory;

class Interfaces {
public:
	uintptr_t client;
	uintptr_t clientState;
	uintptr_t engine;
	uintptr_t playerResource;
};

inline std::optional<Interfaces> interfaces;