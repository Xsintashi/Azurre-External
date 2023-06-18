#include "Clantag.h"

#include "../Config.h"
#include "../Helpers.h"

#include "../SDK/Entity.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Interfaces.h"

#include <algorithm>
#include <iostream>
#include <vector>

void Clan::setClanTag(const char* name, const char* team) {

    if (cfg->restrictions) return;

	unsigned char shellcode[] =
		"\x51"                    //push ecx 
		"\x52"                    //push edx 
		"\xB9\x00\x00\x00\x00"    //mov ecx,00000000 { 0 } 
		"\xBA\x00\x00\x00\x00"    //mov edx,00000000 { 0 } 
		"\xE8\x00\x00\x00\x00"    //call 0 
		"\x83\x04\x24\x0A"        //add dword ptr [esp],0A { 10 } 
		"\x68\x00\x00\x00\x00"    //push engine.dll+9AC90 
		"\xC3"                    //ret 
		"\x5A"                    //pop edx 
		"\x59"                    //pop ecx 
		"\xC3"                    //ret 
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //reserve memory[0x10] 
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //reserve memory[0x10] 
		;

	static unsigned int SHELLCODE_SIZE = sizeof(shellcode) - 0x21;
	unsigned int name_SIZE = (strlen(name) > 15) ? 15 : strlen(name);
	unsigned int team_SIZE = (strlen(team) > 15) ? 15 : strlen(team);
	unsigned int DATA_SIZE = name_SIZE + team_SIZE + 2;

	LPVOID pShellCodeAdress = VirtualAllocEx(
		mem.processHandle,
		0,
		SHELLCODE_SIZE + DATA_SIZE,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE
	);

	DWORD nameAdress = (DWORD)pShellCodeAdress + SHELLCODE_SIZE;
	DWORD teamAdress = (DWORD)pShellCodeAdress + SHELLCODE_SIZE + name_SIZE + 1;
	DWORD fnSetClanAdress = mem.grabSig(IEngine.address, IEngine.size, (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", (char*)"xxxxxxxxx");  //Engine.dll + 0x9AC90 

	memcpy(shellcode + 0x3, &nameAdress, sizeof(DWORD));
	memcpy(shellcode + 0x8, &teamAdress, sizeof(DWORD));
	memcpy(shellcode + 0x16, &fnSetClanAdress, sizeof(DWORD));
	memcpy(shellcode + SHELLCODE_SIZE, name, name_SIZE);
	memcpy(shellcode + SHELLCODE_SIZE + name_SIZE + 1, team, team_SIZE);

	WriteProcessMemory(mem.processHandle, pShellCodeAdress, shellcode, SHELLCODE_SIZE + DATA_SIZE, 0);

	HANDLE hThread = CreateRemoteThread(mem.processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)pShellCodeAdress, NULL, NULL, NULL);
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(mem.processHandle, pShellCodeAdress, 0, MEM_RELEASE);
}

void azurreClanTag() noexcept {

    const char* clantagAzurre[] = {
    "rre             ",
    "re              ",
    "e               ",
    "                ",
    "                ",
    "               a",
    "              az",
    "             azu",
    "            azur",
    "           azurr",
    "          azurre",
    "         azurre ",
    "        azurre  ",
    "       azurre   ",
    "      azurre    ",
    "     azurre     ",
    "    azurre      ",
    "   azurre       ",
    "  azurre        ",
    " azurre         ",
    "azurre          ",
    "zurre           ",
    "urre            ",
    };
    
    int serverTime = static_cast<int>(((globalVars->currentTime / 0.296875f) + 6.60925f - 0.07f));
    Clan::setClanTag(std::string(clantagAzurre[serverTime % 23]).append("\n").c_str(), clantagAzurre[serverTime % 23]);
}

struct neededVars {
    int index;
    std::string name;
};

static  int stealFromIdx = -1;
static bool stealEnabled = false;

std::vector<neededVars>players;

void Clan::update(bool reset, bool update) noexcept
{
    if (!localPlayer) return;

    if (cfg->restrictions) return; //RPM ONLY

    static float prevTime = 0.f;
    static float timeToUpdate = 0.f;
    float realTime = globalVars->realTime;

    static auto custom = [](bool updateCustom) {

        static std::string clan;
        static std::string clanTemp;
        static std::string addRemoveTemp;
        float realTime = globalVars->realTime;
        static float realTimeSwitcher = 0.f;
        static unsigned int addRemoveMod = 0;

        auto upd = []() {

            if (clanTemp == cfg->clanTag.custom.tag) return;

            clanTemp = cfg->clanTag.custom.tag;
            clan = cfg->clanTag.custom.tag;


            while (clan.find("\\a") != std::string::npos)
                clan.replace(clan.find("\\a"), 2, "\a");

            while (clan.find("\\b") != std::string::npos)
                clan.replace(clan.find("\\b"), 2, "\b");

            while (clan.find("\\f") != std::string::npos)
                clan.replace(clan.find("\\f"), 2, "\f");

            while (clan.find("\\n") != std::string::npos)
                clan.replace(clan.find("\\n"), 2, "\n");

            while (clan.find("\\r") != std::string::npos)
                clan.replace(clan.find("\\r"), 2, "\r");

            while (clan.find("\\t") != std::string::npos)
                clan.replace(clan.find("\\t"), 2, "\t");

            while (clan.find("\\v") != std::string::npos)
                clan.replace(clan.find("\\v"), 2, "\v");

            return;
        };

        upd();

        if (realTime - realTimeSwitcher < cfg->clanTag.custom.speed) return;
        addRemoveMod++;

        switch (cfg->clanTag.custom.type) {
        default:
        case 0: // static
            break;
        case 1: // rotate
            if (const auto offset = Helpers::utf8SeqLen(clan[0]); offset <= clan.length())
                std::rotate(clan.begin(), clan.begin() + offset, clan.end());
            break;
        case 2: // rotate backwards
            if (const auto offset = Helpers::utf8SeqLen(clan[0]); offset <= clan.length())
                std::rotate(clan.rbegin(), clan.rbegin() + offset, clan.rend());
            break;
        case 3: // add
            addRemoveTemp = clanTemp;
            clan = addRemoveTemp.substr(0, addRemoveMod % addRemoveTemp.size() + 1);
            break;
        case 4: // remove
            std::string addRemoveTemp;
            addRemoveTemp = clanTemp;
            clan = addRemoveTemp.substr(0, clanTemp.size() - addRemoveMod % addRemoveTemp.size());
            break;
        }

        realTimeSwitcher = realTime;

        setClanTag(std::string(cfg->clanTag.custom.prefix).append(clan).append(cfg->clanTag.custom.postfix).append(cfg->clanTag.custom.hideName ? "\xE2\x80\xA9" : "").c_str(), cfg->clanTag.custom.teamTag.c_str());
    };

    static auto string = [](const char* clanText) {
        setClanTag(clanText, "| azurre |");
    };
    static auto clock = []() {
        if (cfg->clanTag.mode != 2) return;

        static auto lastTime = 0.0f;

        if (globalVars->realTime - lastTime < 1.0f)
            return;

        const auto time = std::time(nullptr);
        const auto localTime = std::localtime(&time);
        char s[11];
        s[0] = '\0';
        snprintf(s, sizeof(s), "[%02d:%02d:%02d]", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
        lastTime = globalVars->realTime;
        setClanTag(s, "| azurre |");
    };
    static auto velocity = []() {
        const auto vel = localPlayer->velocity().length2D();
        std::string velstring = std::to_string((int)vel);
        setClanTag(velstring.c_str(), std::string(velstring).append("km/h WPIERDOL PEEK").c_str());
    };
    static auto position = []() {
        char buff[18];
        ReadProcessMemory(mem.processHandle, (LPCVOID)(localPlayer.get() + Offset::netvars::m_szLastPlaceName), buff, 18, NULL);
        setClanTag(buff, "| azurre |");
    };
    static auto health = []() {
        const int hp = localPlayer->health();
        std::string result = std::to_string(hp) + "HP";
        setClanTag(result.c_str(), "| azurre |");
    };

    if (realTime - prevTime < 0.15f) return;

    prevTime = realTime;

    switch (cfg->clanTag.mode) {
    default: return;

    case 1: azurreClanTag(); return;
    case 2: clock(); return;
    case 3: string("\xE2\x80\xAE\xE2\x80\xAE"); return;
    case 4: velocity(); return;
    case 5: position(); return;
    case 6: health(); return;
    case 7: string("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); return;
    case 8: custom(update); return;
    }

}
