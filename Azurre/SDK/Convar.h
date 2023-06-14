#pragma once

#include "../Core.h"
#include "../Offsets.h"

#include "../SDK/Interfaces.h"

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

unsigned char table[] =
"\xEE\xA4\xBF\xA8\x73\x10\x8E\x0B\xD5\xD6\x39\x97\xF8\xFC\x1A\
\xC6\x0D\x69\x66\x19\x2B\x2A\xE3\x6B\xD2\xFB\x56\x42\x53\xC1\
\x7E\x6C\x83\x03\x40\xBA\xC0\x51\x25\x9E\x27\xF4\x0E\xFE\x4B\
\x1E\x02\x58\xAC\xB0\xFF\x45\x00\x2D\x74\x8B\x17\x41\xB7\x94\
\x21\x2E\xCB\x14\x8F\xCD\x3C\xC5\x76\x09\xAB\x33\xE9\x87\xDC\
\x31\x47\xB8\x52\x6D\x24\xA1\xA9\x96\x3F\x60\xAD\x7D\x71\x43\
\xE0\x4E\xE8\xD7\x23\xDB\x4F\xB5\x29\xE5\x95\x99\x6F\xD9\x15\
\x48\x78\xA3\x85\x28\x7A\x8C\xD0\xE7\xD3\xC8\xA0\xB6\x68\x6E\
\xB2\xED\x0F\x65\x1B\x32\x18\xBD\xB1\x82\xBB\x5C\xFD\x88\x64\
\xD4\x13\xAE\x46\x16\xAA\xCE\xA2\x4A\xF7\x05\x2F\x20\xB3\x75\
\x84\xC3\x7C\x7B\xF5\x80\xEC\xDF\x0C\x54\x36\xDA\x92\xE4\x9D\
\x5E\x6A\x1F\x11\x1D\xC2\x22\x38\x86\xEF\xF6\xF1\xD8\x7F\x62\
\x07\xCC\x9A\x98\xD1\xBC\x30\x3D\x57\x61\xE1\x55\x5A\xA7\x9B\
\x70\x91\x72\x8D\x5D\xFA\x04\xC9\x9C\x26\x59\xE2\xC4\x01\xEB\
\x2C\xB4\x9F\x79\x77\xA6\xBE\x90\x0A\x5B\x4C\xE6\xDD\x50\xCF\
\x37\x3A\x35\xAF\x08\x06\x34\x44\xF2\x12\xDE\x67\xF9\x93\x81\
\x8A\xF3\x1C\xB9\x3E\x3B\xF0\xCA\xEA\x63\x4D\x49\xC7\x89\x5F\xA5";

class ConVar
{
public:
	uintptr_t offset;

	ConVar(int pointer)
	{
		offset = pointer;
	}

	ConVar(uintptr_t pointer)
	{
		offset = pointer;
	}

	ConVar(const char* name)
	{
		offset = getConVarAddress(name);
	}

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

	void clearCallbacks() {
		mem.Write<int>(offset + 0x44 + 0xC, 0);
	}

private:

	static uint32_t toHash(const char* str)
	{
		uint8_t ebx = 0;
		uint8_t edi = 0;
		uint8_t eax = 0;
		uint32_t r;

		if (str && strlen(str) > 0)
		{
			eax = toupper(str[0]);

			uint32_t i = 1;
			while (true)
			{
				eax ^= ebx;
				edi = table[eax];
				eax = toupper(str[i]);
				if (!eax)
					break;

				eax ^= edi;
				ebx = table[eax];
				eax = toupper(str[i + 1]);
				if (!eax)
					break;

				i += 2;
			}
		}

		r = edi;
		r <<= 8;
		r |= ebx;

		return r;
	}

	int getConVarAddress(const char* name)
	{
		int hash = toHash(name);
		const auto& cvarEngine = mem.Read<uintptr_t>(IVstdlib.address + Offset::signatures::interface_engine_cvar);
		uintptr_t ptr = mem.Read<uintptr_t>(mem.Read<int>(cvarEngine + 0x34) + (byte)hash * 4);
		while (ptr != 0)
		{
			if (mem.Read<int>(ptr) == hash)
			{
				const uintptr_t& conVarPointer = mem.Read<uintptr_t>(ptr + 0x4);
				const int ecx = mem.Read<uintptr_t>(conVarPointer + 0xC);
				char str[128] = { 0 };
				if (ReadProcessMemory(mem.processHandle, (LPCVOID)ecx, str, sizeof(str), NULL))
					return conVarPointer;
			}

			ptr = mem.Read<int>(ptr + 0xC);
		}

		return 0;
	}
};