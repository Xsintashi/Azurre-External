#pragma once
#include <optional>
#include <Windows.h>

class Memory;

class Interfaces {
public:
	uintptr_t client;
	uintptr_t clientState;
	uintptr_t engine;
	uintptr_t playerResource;
	HWND console;
};

inline std::optional<Interfaces> interfaces;