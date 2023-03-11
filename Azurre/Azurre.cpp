#include "GUI.h"
#include "Core.h"
#include "Config.h"

#include "Hacks/Aimbot.h"
#include "Hacks/Chams.h"
#include "Hacks/Glow.h"
#include "Hacks/Misc.h"
#include "Hacks/SkinChanger.h"
#include "Hacks/TriggerBot.h"
#include "Hacks/Visuals.h"

#include "DiscordSDK/RPC.h"

#include "SDK/GlobalVars.h"

#include "InputUtil.h"
#include "Helpers.h"

#include <thread>

int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow) {

	// Try to open the mutex.
	HANDLE mutex = OpenMutex(
		MUTEX_ALL_ACCESS, 0, "azurreE");

	if (!mutex)
		// Mutex doesn’t exist. This is
		// the first instance so create
		// the mutex.
		mutex =
		CreateMutex(0, 0, "azurreE");
	else {
		// The mutex exists so this is the
		// the second instance so return.
		MessageBoxA(nullptr, "Only one instance of the software can be running at one time.", "Azurre External", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	cfg.emplace(Config{});
	globalVars.emplace(GlobalVars{});
	Core::init();

	// create gui
	GUI::CreateHWindow("Azurre External");
	GUI::CreateDevice();
	GUI::CreateImGui();

	Discord::Run();

	std::thread noTeammatesThread = std::thread(Visuals::doNotRenderTeammates);
	std::thread glowThread = std::thread(Glow::run);
	std::thread aimbotThread = std::thread(Aimbot::run);

	while (GUI::isRunning){
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		if (GetAsyncKeyState(VK_INSERT))
			::SwitchToThisWindow(GUI::window, true);

		Core::update();

		Skin::update();
		Misc::entityLoop();
		Misc::fakeLag(); // disable while shotting
		Aimbot::recoilSystem();
		TriggerBot::run();
		Chams::run();
		Misc::forceReload(true);
		Misc::bunnyHop();
		Misc::fastStop();
		Visuals::noFlash();
		Visuals::thirdperson();
		Misc::modifyClasses();

		GUI::update();
		GUI::BeginRender();
		GUI::RenderMainMenu();
		if (cfg->m.playerList) GUI::RenderPlayerList();
#if defined(_DEBUG)
		GUI::RenderDebugWindow();
		//ImGui::ShowDemoWindow();
#endif
		GUI::EndRender();
	}
	Misc::forceReload();

	glowThread.join();
	noTeammatesThread.join();
	aimbotThread.join();

	Discord::Shutdown();

	// destroy gui
	GUI::DestroyImGui();
	GUI::DestroyDevice();
	GUI::DestroyHWindow();

	return EXIT_SUCCESS;
}