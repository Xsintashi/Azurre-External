#pragma once

#include "../Core.h"

class ConVar
{
public:

	ConVar(uintptr_t ofs) { offset = ofs;}

public:

	bool registered() {
		csgo.Read<char*>(offset + 0x8);
	}

	char* name() {
		return csgo.Read<char*>(offset + 0xC);
	}

	char* description() {
		return csgo.Read<char*>(offset + 0x10);
	}

	int flags() {
		return csgo.Read<int>(offset + 0x30);
	}

	char* defaultValue() {
		return csgo.Read<char*>(offset + 0x20);
	}

	char* setValue(char* value) {
		csgo.Write<char*>(offset + 0x24, value);
	}

	char* getSzValue() {
		csgo.Read<char*>(offset + 0x24);
	}

	int size() {
		return csgo.Read<int>(offset + 0x28);
	}

	float getFloatValue() {
		return csgo.Read<float>(offset + 0x2C);
	}

	float setValue(float value) {
		csgo.Write<float>(offset + 0x2C, value);
	}

	int getIntValue() {
		return csgo.Read<BYTE>(offset + 0x30);
	}

	int setValue(int value) {
		csgo.Write<BYTE>(offset + 0x30, value);
		return 0;
	}

	bool hasMin() {
		return csgo.Read<int>(offset + 0x34);
	}

	float minVal() {
		return csgo.Read<float>(offset + 0x38);
	}

	bool hasMax() {
		return csgo.Read<bool>(offset + 0x3c);
	}

	float maxVal() {
		return csgo.Read<float>(offset + 0x40);
	}

public:
	uintptr_t offset;

};//Size=0x0044