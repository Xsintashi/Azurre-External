#pragma warning(disable : 4828)
#include "GUI.h"
#include "Core.h"
#include "Config.h"
#include "resources.h"

#include "Hacks/Aimbot.h"
#include "Hacks/Clantag.h"
#include "Hacks/Glow.h"
#include "Hacks/Minimap.h"
#include "Hacks/Misc.h"
#include "Hacks/SkinChanger.h"
#include "Hacks/TriggerBot.h"
#include "Hacks/Visuals.h"

#include "DiscordSDK/RPC.h"

#include "SDK/GlobalVars.h"
#include "SDK/Interfaces.h"
#include "SDK/LocalPlayer.h"

#include "InputUtil.h"
#include "Offsets.h"
#include "Helpers.h"

#include <thread>
#include "Console.h"

int __stdcall wWinMain(	HINSTANCE instance,	HINSTANCE previousInstance,	PWSTR arguments, int commandShow) {

	char mutexName[8] = "azurre\0";

	HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, 0, mutexName);

	if (!mutex)
		mutex = CreateMutex(0, 0, mutexName);
	else {
		MessageBoxA(nullptr, "Only one instance of the software can be running at one time.", "Azurre External", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

#if !defined(_DEBUG)
#pragma warning( push )
#pragma warning( disable : 4566 )
	TCHAR exeFileName[MAX_PATH];
	GetModuleFileName(NULL, exeFileName, MAX_PATH);
	std::string path = std::string(exeFileName);
	std::string exe = path.substr(path.find_last_of("\\") + 1, path.size());
	time_t* timeRand = 0;
	srand(time(timeRand));

	char letters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_~+";
	char newName[64];

	int z = rand() % 10 + 10;
	for (int i = 0; i < z; i++)
	{
		char x = letters[rand() % (sizeof(letters) - 1)];
		newName[i] = x;
	}
	newName[z] = 0x0;
	strcat_s(newName, ".exe\0");

	rename(exe.c_str(), newName);
#pragma warning( pop )
#endif

	cfg.emplace(Config{});
	globalVars.emplace(GlobalVars{});
	interfaces.emplace(Interfaces{});

	Core::init();
	Discord::Run();
	Misc::changeWindowTitle();

	GUI::CreateHWindow("Azurre External");
	GUI::CreateDevice();
	GUI::CreateImGui();

	//SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT); //Soon
	//while (!IConsole && GUI::isRunning) {
	//	IConsole = FindWindowA("Valve001", NULL);
	//	GUI::BeginRender();
	//	GUI::loadWindow();
	//	GUI::EndRender();
	//}
	//SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);

	std::thread(Visuals::doNotRenderTeammates).detach();
	std::thread(Glow::run).detach();
	std::thread(Aimbot::run).detach();
	std::thread(Core::_).detach();
	std::thread(Discord::Update).detach();
	std::thread(Skin::update).detach();
	std::thread(TriggerBot::run).detach();
	std::thread(Visuals::trailsThread).detach();

#ifdef _DEBUG
	cfg->load(u8"debug", false);
#else
	std::ostringstream wav; //No idea bruh
	for (int t = 0; t < sizeof(resource::introSound); t++)
		wav << resource::introSound[t];

	PlaySound(wav.str().c_str(), NULL, SND_MEMORY | SND_ASYNC);
	cfg->load(u8"default", false);
#endif 

#if _DEBUG
	SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
#endif
	while (GUI::isRunning){
		std::this_thread::sleep_for(std::chrono::milliseconds(8)); // cap to 128tps
#if not _DEBUG
		while (IsDebuggerPresent()) {


			//		Debugger detected:
			//		interfaces->engine->findVar("cl_junkcode")->SetValue(1);
			//		interfaces->engine->findVar("sv_junkcode")->SetValue(1);


			__debugbreak();
			int ymsrszobtam = 3064;
			double gfvczwiczpcy = 15204;
			int ftejlzviddr = 5498;
			if (3064 == 3064) {
				int iwzp;
				for (iwzp = 57; iwzp > 0; iwzp--) {
					int* oisjfdodjs;
					oisjfdodjs = &iwzp;
					__debugbreak();
					continue;
				}
			}
			if (5498 == 5498) {
				int pdqk;
				for (pdqk = 80; pdqk > 0; pdqk--) {
					__debugbreak();
					continue;
				}
			}
			if (15204 == 15204) {
				int frqf;
				for (frqf = 45; frqf > 0; frqf--) {
					__debugbreak();
					continue;
				}
			}
			if (5498 != 5498) {
				int zjezvrw;
				for (zjezvrw = 57; zjezvrw > 0; zjezvrw--) {
					__debugbreak();
					continue;
				}
			}
			if (3064 == 3064) {
				int mmq;
				for (mmq = 73; mmq > 0; mmq--) {
					__debugbreak();
					continue;
				}
			}

			std::string axtzapiziq = "Aimbot\0ESP\0Glow\0Chams\0Skin Changer\0";
			bool mektqojswljrsz = false;
			int jeoutyziqtrjg = 1371;
			bool spvnpg = true;
			bool fvcevpu = true;
			int tdqwivuxep = 1567;
			int lbcdsiwjwhrffp = 4423;
			if (true != true) {
				int grxjmymzb;
				for (grxjmymzb = 23; grxjmymzb > 0; grxjmymzb--) {
					__debugbreak();
					continue;
				}
			}
			if (4423 == 4423) {
				int rwnivn;
				for (rwnivn = 47; rwnivn > 0; rwnivn--) {
					__debugbreak();
					continue;
				}
			}
			if (false != false) {
				int ckzb;
				for (ckzb = 69; ckzb > 0; ckzb--) {
					__debugbreak();
					continue;
				}
			}
			if (true == true) {
				int pljlwvubvk;
				for (pljlwvubvk = 43; pljlwvubvk > 0; pljlwvubvk--) {
					__debugbreak();
					continue;
				}
			}
			if (std::string("Enabled\0") != std::string("Enabled\0")) {
				int orstwvni;
				for (orstwvni = 40; orstwvni > 0; orstwvni--) {
					__debugbreak();
					continue;
				}
			}
		}
#endif
		if (cfg->m.menuKey.isPressed()) {
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			showMenu = !showMenu;
			if (showMenu) {
				SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_APPWINDOW);
				io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
				io.WantCaptureMouse = true;
				io.WantCaptureKeyboard = true;
			}
			else {
				SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
				io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
				io.WantCaptureMouse = false;
				io.WantCaptureKeyboard = false;
			}
		}
		//Here funcs which dont need be updated every ms

		Minimap::_();
		Clan::update();
		Core::gameDataUpdate();
		Misc::forceReload(true);
		Misc::modifyConVars();
		GUI::update();
		GUI::BeginRender();
		GUI::overlay();
		Visuals::renderTrails();
		if (cfg->m.minimap.enabled) Minimap::Render();
		if (cfg->m.playerList.enabled) GUI::RenderPlayerList();
		if (cfg->m.keybinds.enabled) Misc::showKeybinds();
		if (cfg->m.spectatorList.enabled) Misc::spectatorList();
		if (cfg->m.bombTimer.enabled) Misc::bombTimer();
		if (cfg->m.indicators.enabled)  Misc::indicators();
		if (showMenu) GUI::RenderMainMenu();
#if defined(_DEBUG)
		//cmd.Draw();
		GUI::RenderDebugWindow();
		if (showMenu) ImGui::ShowDemoWindow();
#endif
		GUI::EndRender();
	}
	Misc::forceReload();
	Misc::changeWindowTitle(true);
	Misc::modifyConVars(true);
	Clan::setClanTag("", "");

	Discord::Shutdown();

	// destroy gui
	GUI::DestroyImGui();
	GUI::DestroyDevice();
	GUI::DestroyHWindow();

	return EXIT_SUCCESS;
}