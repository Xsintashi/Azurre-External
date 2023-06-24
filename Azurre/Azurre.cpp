#pragma warning(disable : 4828)
#include "GUI.h"
#include "Core.h"
#include "Config.h"

#include "Hacks/Aimbot.h"
#include "Hacks/Clantag.h"
#include "Hacks/Glow.h"
#include "Hacks/Minimap.h"
#include "Hacks/Misc.h"
#include "Hacks/SkinChanger.h"
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

	HANDLE mutex = OpenMutex(
		MUTEX_ALL_ACCESS, 0, mutexName);

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

#ifdef _DEBUG
	cfg->load(u8"debug", false);
#else
	cfg->load(u8"default", false);
#endif 

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


#if _DEBUG
	SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
#endif
	while (GUI::isRunning){
		std::this_thread::sleep_for(std::chrono::milliseconds(8)); // cap to 128tps
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
		if (cfg->m.minimap.enabled) Minimap::Render();
		if (cfg->m.playerList.enabled) GUI::RenderPlayerList();
		if (cfg->m.keybinds.enabled) Misc::showKeybinds();
		if (cfg->m.spectatorList.enabled) Misc::spectatorList();
		if (cfg->m.bombTimer.enabled) Misc::bombTimer();
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