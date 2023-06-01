#pragma once
#include <optional>
#include <Windows.h>

class Memory;

struct Module{
	uintptr_t address;
	DWORD size;
};

class Interfaces {
public:
	Module client;
	Module clientState;
	Module vstdlib;
	Module engine;
	Module playerResource;
	HWND console;
};

inline std::optional<Interfaces> interfaces;