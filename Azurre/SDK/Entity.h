#pragma once

#include "Macros.h"
#include "Interfaces.h"
#include "../Core.h"
#include "../Offsets.h"

#include <vector>

class Entity {
public:
	OFFSET(health, (entity), Offset::netvars::m_iHealth, int)
	OFFSET(armor, (entity), Offset::netvars::m_ArmorValue, int)
	OFFSET(teamNumber, (entity), Offset::netvars::m_iTeamNum, int)
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
};

inline std::vector<PlayerData> entityData;