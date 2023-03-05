#pragma once

#include "Macros.h"
#include "Interfaces.h"
#include "../Core.h"
#include "../Offsets.h"

#include <vector>

class Entity {
public:
	OFFSET(lastPlaceName, (entity), Offset::netvars::m_szLastPlaceName, LPVOID)
	OFFSET(money, (entity), Offset::netvars::m_iAccount, int)
	OFFSET(health, (entity), Offset::netvars::m_iHealth, int)
	OFFSET(armor, (entity), Offset::netvars::m_ArmorValue, int)
	OFFSET(teamNumber, (entity), Offset::netvars::m_iTeamNum, int)
	OFFSET(flags, (entity), Offset::netvars::m_fFlags, int)

};

inline Entity entityOffsets;

static uintptr_t getEntity(int idx) {
	const auto& player = csgo.Read<uintptr_t>(IClient + Offset::signatures::dwEntityList + idx * 0x10);
	if (!player) return 0;
	return player;
}

struct PlayerData {
	uintptr_t adress;
	int unsigned idx;
	unsigned long steamID;
	bool isBot;
	std::string name;
	int health;
	int armor;
	int teamNumber;
	int money;
};

inline std::vector<PlayerData> entityData;