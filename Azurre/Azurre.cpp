#include "GUI.h"
#include "Core.h"
#include "Config.h"

#include "Hacks/Aimbot.h"
#include "Hacks/Clantag.h"
#include "Hacks/Glow.h"
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
	std::thread skinUpdateThread = std::thread(Skin::update);
	std::thread coreThread = std::thread(Core::_);

	Misc::changeWindowTitle();

	while (GUI::isRunning){
		SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
		std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~ 30 loops per second cuz of huge framedrop caused by overlay
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			showMenu = !showMenu;
			if (showMenu) {
				SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
				io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
				io.WantCaptureMouse = true;
				io.WantCaptureKeyboard = true;
			}
			else {
				SetWindowLongPtr(GUI::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
				io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
				io.WantCaptureMouse = false;
				io.WantCaptureKeyboard = false;
			}
		}
		Misc::entityLoop();
		Misc::forceReload(true);
		Misc::modifyClasses();

		GUI::update();
		GUI::BeginRender();
		if (showMenu) GUI::RenderMainMenu();
		if (cfg->m.playerList) GUI::RenderPlayerList();
		GUI::overlay();
#if defined(_DEBUG)
		if (showMenu) GUI::RenderDebugWindow();
		// if (showMenu) ImGui::ShowDemoWindow();
#endif
		GUI::EndRender();
	}
	csgo.Write<byte>(IEngine.address + Offset::signatures::dwbSendPackets, true);

	Misc::forceReload();
	Misc::changeWindowTitle(true);
	Clan::setClanTag("","");
	glowThread.join();
	noTeammatesThread.join();
	aimbotThread.join();
	skinUpdateThread.join();
	coreThread.join();

	Discord::Shutdown();

	// destroy gui
	GUI::DestroyImGui();
	GUI::DestroyDevice();
	GUI::DestroyHWindow();

	return EXIT_SUCCESS;
}