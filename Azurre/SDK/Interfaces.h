#pragma once
#include <optional>

class Memory;

class Interfaces {
public:
	uintptr_t client;
	uintptr_t clientState;
	uintptr_t engine;
};

inline std::optional<Interfaces> interfaces;