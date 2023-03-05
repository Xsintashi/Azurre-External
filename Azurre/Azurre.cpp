#include "GUI.h"
#include "Core.h"
#include "Config.h"
#include "Hacks/Misc.h"
#include <thread>
#include "Hacks/SkinChanger.h"
#include "SDK/GlobalVars.h"

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

	while (GUI::isRunning)
	{
		Core::update();
		Misc::forceReload(true);
		Misc::noFlash();
		Misc::bunnyHop();
		Misc::thirdperson();
		Skin::update();

		GUI::BeginRender();
		GUI::Render();
		GUI::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// destroy gui
	GUI::DestroyImGui();
	GUI::DestroyDevice();
	GUI::DestroyHWindow();

	return EXIT_SUCCESS;
}
