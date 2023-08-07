#pragma once

struct PlayerInfo {
	__int64         version;
	union
	{
		__int64       steamID64;          //_byteswap_uint64 them!
		struct
		{
			__int32     xuidLow;
			__int32     xuidHigh;
		};
	};
	char            name[128];			//0x0010 - Player Name
	int             userID;             //0x0090 - Unique Server Identifier
	char            steamID[20];		//0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID
	char            friendsName[128];	//0x---- - Friends Name
	bool            fakePlayer;         //0x---- - true, if player is a bot controlled by game.dll
	bool            isHLTV;             //0x---- - true if player is the HLTV proxy
	unsigned int    customFiles[4];     //0x---- - custom files CRC for this player
	unsigned char   filesDownloaded;    //0x---- - this counter increases each time the server downloaded a new file
};