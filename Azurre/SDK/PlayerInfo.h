#pragma once

struct PlayerInfo {
	std::uint64_t         unknown;            //0x0000 
	union
	{
		std::uint64_t       steamID64;          //0x0008 - SteamID64
		struct
		{
			std::uint32_t     xuid_low;
			std::uint32_t     xuid_high;
		};
	};
	char            name[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID 
	char            friendsName[128];
	bool            fakeplayer;
	bool            ishltv;
	unsigned int    customfiles[4];
	unsigned char   filesdownloaded;
} playerInfo;