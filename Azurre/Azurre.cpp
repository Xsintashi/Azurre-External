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

int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow) {

	// Try to open the mutex.
	HANDLE mutex = OpenMutex(
		MUTEX_ALL_ACCESS, 0, "azurreX");

	if (!mutex)
		// Mutex doesn’t exist. This is
		// the first instance so create
		// the mutex.
		mutex =
		CreateMutex(0, 0, "azurreX");
	else {
		// The mutex exists so this is the
		// the second instance so return.
		MessageBoxA(nullptr, "Only one instance of the software can be running at one time.", "Azurre External", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	cfg.emplace(Config{});
	globalVars.emplace(GlobalVars{});
	interfaces.emplace(Interfaces{});

	// create gui
	GUI::CreateHWindow("Azurre External");
	GUI::CreateDevice();
	GUI::CreateImGui();

	Discord::Run();
	//SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT); //Soon
	//while (!IConsole && GUI::isRunning) {
	//	IConsole = FindWindowA("Valve001", NULL);
	//	GUI::BeginRender();
	//	GUI::loadWindow();
	//	GUI::EndRender();
	//}
	//SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
	Core::init();
	if (!cfg->restrictions)
	Misc::changeWindowTitle();

	std::thread noTeammatesThread = std::thread(Visuals::doNotRenderTeammates);
	std::thread glowThread = std::thread(Glow::run);
	std::thread aimbotThread = std::thread(Aimbot::run);
	std::thread coreThread = std::thread(Core::_);
#if _DEBUG
	SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
#endif
	while (GUI::isRunning){
		std::this_thread::sleep_for(std::chrono::milliseconds(8)); // cap to 128tps
		if (GetAsyncKeyState(VK_INSERT) & 1) {

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
		Misc::entityLoop();
		Misc::forceReload(true);
		Misc::modifyClasses();
		Misc::modifyConVars();
		Skin::update();
		GUI::update();
		GUI::BeginRender();
		Core::entityDataUpdate();
		if (showMenu) GUI::RenderMainMenu();
		if (cfg->m.playerList.enabled) GUI::RenderPlayerList();
		if (cfg->m.minimap.enabled) Minimap::Render();
		GUI::overlay();
#if defined(_DEBUG)
		// if (showMenu) GUI::RenderDebugWindow();
		// if (showMenu) ImGui::ShowDemoWindow();
#endif
		GUI::EndRender();
	}
	if (!cfg->restrictions) {
		Misc::forceReload();
		Misc::changeWindowTitle(true);
		Misc::modifyConVars(true);
		Clan::setClanTag("", "");
	}

	glowThread.join();
	noTeammatesThread.join();
	aimbotThread.join();
	coreThread.join();

	Discord::Shutdown();

	// destroy gui
	GUI::DestroyImGui();
	GUI::DestroyDevice();
	GUI::DestroyHWindow();

	return EXIT_SUCCESS;
}