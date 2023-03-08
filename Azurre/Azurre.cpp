#include "GUI.h"
#include "Core.h"
#include "Config.h"

#include "Hacks/Chams.h"
#include "Hacks/Glow.h"
#include "Hacks/Misc.h"
#include "Hacks/SkinChanger.h"
#include "Hacks/TriggerBot.h"
#include "Hacks/Visuals.h"

#include "SDK/GlobalVars.h"
#include "InputUtil.h"

#include "DiscordSDK/RPC.h"

#include <thread>
#include "Hacks/Aimbot.h"

int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
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

	while (GUI::isRunning)
	{
		if (GetAsyncKeyState(VK_INSERT))
			::SetForegroundWindow(GUI::window);

		Core::update();
		Discord::Update();
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
		Skin::update();

		GUI::BeginRender();
		GUI::Render();
		GUI::EndRender();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
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