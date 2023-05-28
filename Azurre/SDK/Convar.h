#pragma once

#include "../Core.h"

enum CVarFlags
{
	NONE = 0,
	UNREGISTERED = (1 << 0),
	DEVELOPMENTONLY = (1 << 1),
	GAMEDLL = (1 << 2),
	CLIENTDLL = (1 << 3),
	HIDDEN = (1 << 4),
	PROTECTED = (1 << 5),
	SPONLY = (1 << 6),
	ARCHIVE = (1 << 7),
	NOTIFY = (1 << 8),
	USERINFO = (1 << 9),
	PRINTABLEONLY = (1 << 10),
	UNLOGGED = (1 << 11),
	NEVER_AS_STRING = (1 << 12),
	REPLICATED = (1 << 13),
	CHEAT = (1 << 14),
	DEMO = (1 << 16),
	DONTRECORD = (1 << 17),
	NOT_CONNECTED = (1 << 22),
	ARCHIVE_XBOX = (1 << 24),
	SERVER_CAN_EXECUTE = (1 << 28),
	SERVER_CANNOT_QUERY = (1 << 29),
	CLIENTCMD_CAN_EXECUTE = (1 << 30)
};

class ConVar
{
public:

	ConVar(uintptr_t ofs) { offset = ofs;}

public:

	bool registered() {
		return mem.Read<char*>(offset + 0x8);
	}

	char* name() {
		return mem.Read<char*>(offset + 0xC);
	}

	char* description() {
		return mem.Read<char*>(offset + 0x10);
	}

	int getFlags() {
		return mem.Read<int>(offset + 0x14);
	}

	void flags(int flags) {
		mem.Write<int>(offset + 0x14, flags);
	}

	char* defaultValue() {
		return mem.Read<char*>(offset + 0x20);
	}

	void setValue(char* value) {
		mem.Write<char*>(offset + 0x24, value);
	}

	char* getSzValue() {
		return mem.Read<char*>(offset + 0x24);
	}

	int size() {
		return mem.Read<int>(offset + 0x28);
	}

	float getFloatValue() {
		return mem.Read<float>(offset + 0x2C);
	}

	void setValue(float value) {
		mem.Write<int32_t>(offset + 0x2C, *reinterpret_cast<uintptr_t*>(&value) ^ offset);
	}

	int getIntValue() {
		return mem.Read<BYTE>(offset + 0x30);
	}

	void setValue(int value) {
		mem.Write<int32_t>(offset + 0x30, *reinterpret_cast<uintptr_t*>(&value) ^ offset);
	}

	bool hasMin() {
		return mem.Read<int>(offset + 0x34);
	}

	float minVal() {
		return mem.Read<float>(offset + 0x38);
	}

	bool hasMax() {
		return mem.Read<bool>(offset + 0x3c);
	}

	float maxVal() {
		return mem.Read<float>(offset + 0x40);
	}

public:
	uintptr_t offset;

};//Size=0x0044