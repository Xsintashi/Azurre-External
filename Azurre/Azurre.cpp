#include "GUI.h"
#include "Core.h"
#include "Config.h"
#include "Hacks/Misc.h"
#include "Hacks/Visuals.h"
#include <thread>
#include "Hacks/SkinChanger.h"
#include "SDK/GlobalVars.h"
#include "Hacks/TriggerBot.h"
#include "DiscordSDK/RPC.h"
#include "Hacks/Chams.h"

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

	while (GUI::isRunning)
	{
		Core::update();
		Discord::Update();
		TriggerBot::run();
		Chams::run();
		Misc::forceReload(true);
		Misc::bunnyHop();
		Visuals::noFlash();
		Visuals::thirdperson();
		Visuals::remove3dSky();
		Misc::modifyClasses();
		Skin::update();

		GUI::BeginRender();
		GUI::Render();
		GUI::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	Discord::Shutdown();

	// destroy gui
	GUI::DestroyImGui();
	GUI::DestroyDevice();
	GUI::DestroyHWindow();

	return EXIT_SUCCESS;
}
