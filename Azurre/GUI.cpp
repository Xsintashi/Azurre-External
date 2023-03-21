#include "GUI.h"
#include "Config.h"

#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/Interfaces.h"
#include "SDK/LocalPlayer.h"
#include "SDK/UserInterface.h"

#include "Hacks/SkinChanger.h"
#include "Hacks/Misc.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Lib/imgui/imgui.h"
#include "../Lib/imgui/ImGuiCustom.h"
#include "../Lib/imgui/imgui_stdlib.h"
#include "../Lib/imgui/imgui_impl_dx9.h"
#include "../Lib/imgui/imgui_impl_win32.h"

#include <string>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (GUI::device && wideParameter != SIZE_MINIMIZED)
		{
			GUI::presentParameters.BackBufferWidth = LOWORD(longParameter);
			GUI::presentParameters.BackBufferHeight = HIWORD(longParameter);
			GUI::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		GUI::position = MAKEPOINTS(longParameter); // set click points
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(GUI::window, &rect);

			rect.left += points.x - GUI::position.x;
			rect.top += points.y - GUI::position.y;

			/*if (GUI::position.x >= 0 &&
				GUI::position.x <= GUI::WIDTH &&
				GUI::position.y >= 0 && GUI::position.y <= 19)
				SetWindowPos(
					GUI::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);*/
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void GUI::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "azurreE";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"azurreE",
		windowName,
		WS_POPUP,
		100,
		100,
		320,
		240,
		0,
		0,
		windowClass.hInstance,
		0
	);



	//ShowWindow(window, SW_SHOWDEFAULT); // Shows black main window. We dont wanna see it
	UpdateWindow(window);
}

void GUI::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool GUI::CreateDevice() noexcept
{
	if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Create the D3DDevice
	ZeroMemory(&presentParameters, sizeof(presentParameters));
	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device) < 0)
		return false;

	return true;
}

void GUI::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void GUI::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void GUI::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void GUI::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GUI::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GUI::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

void GUI::updateColors() noexcept
{
	switch (cfg->u.menuColors) {
	default:
	case 0: ImGui::StyleColorsAzurre(); break;
	case 1: ImGui::StyleColorsGenshi(); break;
	case 2: ImGui::StyleColorsLime(); break;
	case 3: ImGui::StyleColorsRed(); break;
	case 4: ImGui::StyleColorsGold(); break;
	case 5: ImGui::StyleColorsPandora(); break;
	case 6: ImGui::StyleColorsLight(); break;
	case 7: ImGui::StyleColorsDeepDark(); break;
	case 8: ImGui::StyleColorsVisualStudio(); break;
	case 9: ImGui::StyleColorsGoldSrc(); break;
	case 10: ImGui::StyleColorsClassic(); break;
	case 11: ImGui::StyleColorsNeverlose(); break;
	case 12: ImGui::StyleColorsAimware(); break;
	case 13: ImGui::StyleColorsOnetap(); break;
	case 14: break; //Custom
	}
}

void GUI::update() noexcept {

	SetWindowPos(window, cfg->u.alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	auto& guiSettings = ImGui::GetStyle();
	guiSettings.AntiAliasedLines = cfg->u.antiAliasing;
    guiSettings.AntiAliasedLinesUseTex = cfg->u.antiAliasing;
    guiSettings.AntiAliasedFill = cfg->u.antiAliasing;
    guiSettings.WindowBorderSize = cfg->u.windowBorder ? 1.f : 0.f;
    guiSettings.PopupBorderSize = cfg->u.windowBorder ? 1.f : 0.f;
    guiSettings.FrameBorderSize = cfg->u.frameBorder ? 1.f : 0.f;
    guiSettings.WindowTitleAlign = cfg->u.centerTitle ? ImVec2{0.5f, 0.5f} : ImVec2{ 0.f, 0.5f };
}

void GUI::loadWindow() noexcept {
	ImGui::SetNextWindowSize({ 320,240 });
	ImGui::Begin(
		"Loading...",
		&isRunning,
		ImGuiWindowFlags_AlwaysAutoResize
	);

	ImGui::Text("Hello xs9 :)");
	ImGui::Text("Waiting For csgo.exe...");
	ImGui::Text("Client: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IClient);
	ImGui::Text("Engine: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IEngine);
	if (ImGui::Button("Run CS:GO")) {
		WinExec("steam://run/730", SW_NORMAL);
	}
	ImGui::End();
}

void GUI::RenderDebugWindow() noexcept {
	ImGui::Begin(
		"Debug",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysAutoResize
	);
	static auto frameRate = 1.0f;
	frameRate = 0.9f * frameRate + 0.1f * globalVars->absoluteFrameTime;
	const int framePerSecond = frameRate != 0.0f ? static_cast<int>(1 / frameRate) : 0;
	const int tickRate = static_cast<int>(1 / globalVars->intervalPerTick); //tps

	int chokedPackets = csgo.Read<int>(IClientState + Offset::signatures::clientstate_choked_commands);

	ImGui::TextUnformatted("Build date: " __DATE__ " " __TIME__);

	ImGui::Text("Fps: %i", framePerSecond);
	ImGui::Text("Tick: %i", tickRate);
	ImGui::Text("Client: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IClient);
	ImGui::Text("ClientState: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IClientState);
	ImGui::Text("Engine: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IEngine);
	ImGui::Text("PlayerResource: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IPlayerResource);
	ImGui::Text("LocalPlayer: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", localPlayer ? localPlayer.get() : 0);
	ImGui::Text("RealTime: %.2f", globalVars->realTime);
	ImGui::Text("FrameCount: %i", globalVars->frameCount);
	ImGui::Text("AbsoluteFrametime: %.2f", globalVars->absoluteFrameTime);
	ImGui::Text("AbsoluteFrameStartTimeStdDev: %.2f", globalVars->absoluteFrameStartTimeStdDev);
	ImGui::Text("CurrentTime: %.2f", globalVars->currentTime);
	ImGui::Text("FrameTime: %.2f", globalVars->frameTime);
	ImGui::Text("MaxClients: %i", globalVars->maxClients);
	ImGui::Text("TickCount: %i", globalVars->tickCount);
	ImGui::Text("IntervalPerTick: %.2f", globalVars->intervalPerTick);
	ImGui::Text("InterpolationAmount: %.2f", globalVars->interpolationAmount);
	ImGui::Text("GameState: %i", gameState);

	ImGui::Text("Choked Packets: %i", chokedPackets);

	ImGui::Checkbox("Bool Debug 0", &cfg->debug.boolDebug0);

	ImGui::PushID("Roll");
	static float roll = 0.f;
	static float tempRoll = 0.f;
	ImGui::SetNextItemWidth(128);
	ImGui::SliderFloat("", &roll, -45.f, 45.f, "Roll: %.1f");
	if (roll != tempRoll) {
		tempRoll = roll;
		const auto& viewAngles = csgo.Read<ImVec2>(IClientState + Offset::signatures::dwClientState_ViewAngles);
		csgo.Write<Vector>(IClientState + Offset::signatures::dwClientState_ViewAngles, { viewAngles.x, viewAngles.y, roll });
	}
	static std::string cmd = "";
	ImGui::InputText("convar", &cmd);
	ImGui::SameLine();
	if (ImGui::Button("Send"))
		usr0::SendConsoleCommand(cmd);
	ImGui::PopID();
	ImGui::End();
}

void GUI::RenderPlayerList() noexcept {
	ImGui::Begin(
		"Player List",
		&cfg->m.playerList,
		ImGuiWindowFlags_AlwaysAutoResize
	);

	ImGui::SetNextWindowSize({ -1, -1 });
	if (ImGui::BeginTable("Players List", 7))
	{
		ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Health", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Armor", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Money", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Weapon", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Last Place", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();
		for (unsigned int row = 0; row < entityData.size(); row++)
		{
			auto teamColor = localPlayer.get() == (uintptr_t)entityData[row].entity ? ImVec4{ 1.0f, 0.25f, 1.0f, 1.f } : entityData[row].teamNumber == 2 ? ImVec4{ 0.92f, 0.82f, .54f, 1.f } : ImVec4{ 0.26f, 0.59f, 0.98f, 1.f };
			auto hpColor = entityData[row].health < 50 ? entityData[row].health < 25 ? ImVec4{ 1.f, .0f, .0f, 1.f } : ImVec4{ 1.f, 1.f, .0f, 1.f } : ImVec4{ 0.f, 1.f, .0f, 1.f };

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%i", entityData[row].idx);
			ImGui::TableNextColumn();
			ImGui::PushID(row);
			ImGui::TextColored(teamColor, "%s", entityData[row].name.c_str());
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", entityData[row].steamID);
			ImGui::PopID();
			ImGui::TableNextColumn();
			ImGui::TextColored(hpColor, "%s", entityData[row].health < 1 ? "DEAD" : std::to_string(entityData[row].health).c_str());
			ImGui::TableNextColumn();
			ImGui::Text("%i%s", entityData[row].armor, entityData[row].hasHelmet ? "+H" : "");
			ImGui::TableNextColumn();
			ImGui::Text("$%i", entityData[row].money);
			ImGui::TableNextColumn();
			ImGui::Text("%s", Skin::getWeaponIDName(entityData[row].weaponID));
			ImGui::TableNextColumn();
			ImGui::Text("%s", entityData[row].placename.c_str());
		}
		ImGui::EndTable();
	}
	ImGui::End();
}


void GUI::RenderMainMenu() noexcept {
	ImGui::SetNextWindowSize({ 640, 480 });
	ImGui::Begin(
		"Azurre External 0.1",
		&isRunning,
		ImGuiWindowFlags_AlwaysAutoResize
	);

	ImGui::Text("Hello xs9 :)");
	if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip)) {
		if (ImGui::BeginTabItem("Aimbot")) {
			ImGui::PushID("aimbot");
			ImGui::Checkbox("Enabled", &cfg->a.enabled);
			ImGui::SameLine();
			ImGui::hotkey("", cfg->a.hotkey);
			ImGui::PopID();
			ImGui::Checkbox("Auto Shot", &cfg->a.autoShot);
			ImGui::Checkbox("Auto Stop", &cfg->a.autoStop);
			ImGui::PushItemWidth(220.0f);
			ImGui::Combo("Bone", &cfg->a.bone, "Head\0Neck\0Sternum\0Chest\0Stomach\0Pelvis\0");
			ImGui::SliderFloat("##fov", &cfg->a.fov, 0.001f, 255.000f, "Fov: %.2f");
			ImGui::SliderFloat("##smooth", &cfg->a.smooth, 1.00f, 100.00f, "Smooth: %.2f");
			ImGui::PopItemWidth();
			ImGui::Checkbox("RCS", &cfg->a.rcs);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("TriggerBot")) {
			ImGui::PushID("triggerbot");
			ImGui::Checkbox("Enabled", &cfg->t.enabled);
			ImGui::SameLine();
			ImGui::hotkey("", cfg->t.hotkey);
			ImGui::PopID();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Glow")) {
			ImGui::Checkbox("Enabled", &cfg->g.enabled);
			ImGuiCustom::colorPicker("Enemies", cfg->g.enemy.color.data(), &cfg->g.enemy.color[3], nullptr, nullptr, &cfg->g.enemy.enabled);
			ImGuiCustom::colorPicker("Allies", cfg->g.ally.color.data(), &cfg->g.ally.color[3], nullptr, nullptr, &cfg->g.ally.enabled);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Chams")) {
			ImGui::Checkbox("Enabled", &cfg->c.enabled);
			ImGuiCustom::colorPicker("Enemies", cfg->c.enemy);
			ImGuiCustom::colorPicker("Allies", cfg->c.ally);
			ImGui::SetNextItemWidth(200.0f);
			ImGui::SliderFloat("Brightness", &cfg->c.brightness, 0.1f, 1.f);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Misc")) {
			ImGui::PushID("menu");
			ImGui::Text("Bring Menu");
			ImGui::SameLine();
			ImGuiCustom::classicHotkey("", cfg->m.bringMenu);
			ImGui::PopID();
			ImGui::Checkbox("Bunny Hop", &cfg->m.bhop);
			ImGui::Checkbox("Fix Tablet Signal", &cfg->m.fixTablet);
			ImGui::Checkbox("Engine Radar", &cfg->m.radarHack);
			ImGui::Checkbox("Fast Stop", &cfg->m.autoStop);
			ImGui::Checkbox("Player List", &cfg->m.playerList);
			if (ImGui::Checkbox("Fake Lag", &cfg->m.fakeLag.enabled)) {
				ImGui::PushItemWidth(220.0f);
				ImGui::Combo("Mode", &cfg->m.fakeLag.type, "Static\0Adaptative\0Random\0");
				ImGui::SliderInt("##limit", &cfg->m.fakeLag.limit, 1, 16, "Limit: %d");
				ImGui::PopItemWidth();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Visuals")) {
			ImGui::PushID("PostProcessingPopup");
			ImGui::Checkbox("Custom post-processing", &cfg->v.customPostProcessing.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::SliderFloat("Bloom", &cfg->v.customPostProcessing.bloomScale, 0.0f, 750.f, "%.3f", ImGuiSliderFlags_Logarithmic);
				ImGui::SliderFloat("Exposure", &cfg->v.customPostProcessing.worldExposure, 0.0f, 2000.f, "%.3f", ImGuiSliderFlags_Logarithmic);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGui::PushID("thirderson");
			ImGui::Checkbox("Thirdperson", &cfg->v.thirdPerson);
			ImGui::SameLine();
			ImGui::hotkey("", cfg->v.thirdPersonKey);
			ImGui::PopID();
			ImGui::Checkbox("Don't render Teammates", &cfg->v.noAllies);
			ImGui::Checkbox("No Smoke", &cfg->v.noSmoke);
			ImGui::SetNextItemWidth(200.0f);
			ImGui::SliderInt("##flash", &cfg->v.flashReduction, 0, 100, "Flash reduction: %d%%");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Changer")) {
			static int itemIndex = 0;
			static int itemIndexTemp = 0;
			ImGui::PushItemWidth(200.0f);
			ImGui::Combo("##1", &itemIndex, [](void* data, int idx, const char** out_text) {
				*out_text = Skin::weaponNames[idx].name;
					return true;
				}, nullptr, Skin::weaponNames.size(), 5);
			ImGui::SameLine();
			if (ImGui::Button("Update")) {
				cfg->s[itemIndex].weaponID = Skin::weaponNames[itemIndex].definitionIndex;
				itemIndexTemp = itemIndex;
				Misc::forceReload();
			}
			ImGui::InputInt("Skin ID", &cfg->s[itemIndex].skinID);
			ImGui::InputFloat("Wear", &cfg->s[itemIndex].wear);
			ImGui::InputInt("Seed", &cfg->s[itemIndex].seed);
			ImGui::InputInt("StatTrak", &cfg->s[itemIndex].statTrak);
			ImGui::Combo("Quality", &cfg->s[itemIndex].quality, "Normal\0Genuine\0Vintage\0?\0Unique\0Community\0Valve\0Protoype\0Customized\0StatTrak\0Completed\0Souvenir\0");
			ImGui::InputText("NameTag", cfg->s[itemIndex].nameTag, sizeof(cfg->s[itemIndex].nameTag));
			ImGui::Separator();
			ImGui::Combo("T Knife", &cfg->ch.TTKnife, [](void* data, int idx, const char** out_text) {
				*out_text = Skin::knifeNames[idx].name;
			return true;
				}, nullptr, Skin::knifeNames.size(), 5);
			ImGui::Combo("CT Knife", &cfg->ch.CTKnife, [](void* data, int idx, const char** out_text) {
				*out_text = Skin::knifeNames[idx].name;
			return true;
				}, nullptr, Skin::knifeNames.size(), 5);
			ImGui::Separator();

			constexpr const char* modelsGUI[]{
				"Default",
				"Special Agent Ava | FBI",
				"Operator | FBI SWAT",
				"Markus Delrow | FBI HRT",
				"Michael Syfers | FBI Sniper",
				"B Squadron Officer | SAS",
				"Seal Team 6 Soldier | NSWC SEAL",
				"Buckshot | NSWC SEAL",
				"Lt. Commander Ricksaw | NSWC SEAL",
				"Third Commando Company | KSK",
				"'Two Times' McCoy | USAF TACP",
				"Dragomir | Sabre",
				"Rezan The Ready | Sabre",
				"'The Doctor' Romanov | Sabre",
				"Maximus | Sabre",
				"Blackwolf | Sabre",
				"The Elite Mr. Muhlik | Elite Crew",
				"Ground Rebel | Elite Crew",
				"Osiris | Elite Crew",
				"Prof. Shahmat | Elite Crew",
				"Enforcer | Phoenix",
				"Slingshot | Phoenix",
				"Soldier | Phoenix",
				"Street Soldier | Phoenix",
				"'Blueberries' Buckshot | NSWC SEAL",
				"'Two Times' McCoy | TACP Cavalry",
				"Rezan the Redshirt | Sabre",
				"Dragomir | Sabre Footsoldier",
				"Cmdr. Mae 'Dead Cold' Jamison | SWAT",
				"001st Lieutenant Farlow | SWAT",
				"John 'Van Healen' Kask | SWAT",
				"Bio-Haz Specialist | SWAT",
				"Sergeant Bombson | SWAT",
				"Chem-Haz Specialist | SWAT",
				"Sir Bloody Miami Darryl | The Professionals",
				"Sir Bloody Silent Darryl | The Professionals",
				"Sir Bloody Skullhead Darryl | The Professionals",
				"Sir Bloody Darryl Royale | The Professionals",
				"Sir Bloody Loudmouth Darryl | The Professionals",
				"Safecracker Voltzmann | The Professionals",
				"Little Kev | The Professionals",
				"Number K | The Professionals",
				"Getaway Sally | The Professionals",
				"Anarchist",
				"Anarchist (Variant A)",
				"Anarchist (Variant B)",
				"Anarchist (Variant C)",
				"Anarchist (Variant D)",
				"Pirate",
				"Pirate (Variant A)",
				"Pirate (Variant B)",
				"Pirate (Variant C)",
				"Pirate (Variant D)",
				"Professional",
				"Professional (Variant 1)",
				"Professional (Variant 2)",
				"Professional (Variant 3)",
				"Professional (Variant 4)",
				"Separatist",
				"Separatist (Variant A)",
				"Separatist (Variant B)",
				"Separatist (Variant C)",
				"Separatist (Variant D)",
				"GIGN",
				"GIGN (Variant A)",
				"GIGN (Variant B)",
				"GIGN (Variant C)",
				"GIGN (Variant D)",
				"GSG-9",
				"GSG-9 (Variant A)",
				"GSG-9 (Variant B)",
				"GSG-9 (Variant C)",
				"GSG-9 (Variant D)",
				"IDF",
				"IDF (Variant B)",
				"IDF (Variant C)",
				"IDF (Variant D)",
				"IDF (Variant E)",
				"IDF (Variant F)",
				"SWAT",
				"SWAT (Variant A)",
				"SWAT (Variant B)",
				"SWAT (Variant C)",
				"SWAT (Variant D)",
				"SAS (Variant A)",
				"SAS (Variant B)",
				"SAS (Variant C)",
				"SAS (Variant D)",
				"ST6",
				"ST6 (Variant A)",
				"ST6 (Variant B)",
				"ST6 (Variant C)",
				"ST6 (Variant D)",
				"Balkan (Variant E)",
				"Balkan (Variant A)",
				"Balkan (Variant B)",
				"Balkan (Variant C)",
				"Balkan (Variant D)",
				"Jumpsuit (Variant A)",
				"Jumpsuit (Variant B)",
				"Jumpsuit (Variant C)",
				"Phoenix Heavy",
				"Heavy",
				"Leet (Variant A)",
				"Leet (Variant B)",
				"Leet (Variant C)",
				"Leet (Variant D)",
				"Leet (Variant E)",
				"Phoenix",
				"Phoenix (Variant A)",
				"Phoenix (Variant B)",
				"Phoenix (Variant C)",
				"Phoenix (Variant D)",
				"FBI",
				"FBI (Variant A)",
				"FBI (Variant C)",
				"FBI (Variant D)",
				"FBI (Variant E)"
			};
			ImGui::Combo("T Player Model", &cfg->ch.TTAgent, modelsGUI, IM_ARRAYSIZE(modelsGUI));
			ImGui::Combo("CT Player Model", &cfg->ch.CTAgent, modelsGUI, IM_ARRAYSIZE(modelsGUI));
			ImGui::PopItemWidth();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Discord")) {
			ImGui::Checkbox("Enabled", &cfg->d.enabled);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("GUI")) {

			if (ImGui::Combo("Menu colors", &cfg->u.menuColors, "Azurre\0Genshi\0Emerald\0Bloddy Red\0Gold Mine\0Pandora\0Holy Light\0Deep Dark\0Visual Studio\0GoldSrc\0ImGui\0Neverlose\0Aimware\0Onetap\0Custom\0"))
				updateColors();

			ImGui::Checkbox("Always on Top",&cfg->u.alwaysOnTop);
			ImGui::Checkbox("AntiAliasing",&cfg->u.antiAliasing);
			ImGui::Checkbox("Center Title",&cfg->u.centerTitle);
			ImGui::Checkbox("Frame Border",&cfg->u.frameBorder);
			ImGui::Checkbox("Window Border",&cfg->u.windowBorder);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Config")) {
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnOffset(1, 170.0f);

			static bool incrementalLoad = false;
			ImGui::Checkbox("Incremental Load", &incrementalLoad);

			ImGui::PushItemWidth(160.0f);

			auto& configItems = cfg->getConfigs();
			static int currentConfig = -1;
			static float timeToNextConfigRefresh = 0.1f;
			static std::string buffer;

			timeToNextConfigRefresh -= ImGui::GetIO().DeltaTime;
			if (timeToNextConfigRefresh <= 0.0f) {
				cfg->listConfigs();
				if (const auto it = std::find(configItems.begin(), configItems.end(), buffer); it != configItems.end())
					currentConfig = std::distance(configItems.begin(), it);
				timeToNextConfigRefresh = 0.1f;
			}

			if (static_cast<std::size_t>(currentConfig) >= configItems.size())
				currentConfig = -1;

			if (ImGui::ListBox("", &currentConfig, [](void* data, int idx, const char** out_text) {
				auto& vector = *static_cast<std::vector<std::string>*>(data);
			*out_text = vector[idx].c_str();
			return true;
			}, &configItems, configItems.size(), 5) && currentConfig != -1)
			buffer = configItems[currentConfig];

			ImGui::PushID(0);
			if (ImGui::InputTextWithHint("", "config name", &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
				if (currentConfig != -1)
					cfg->rename(currentConfig, buffer.c_str());
			}
			ImGui::PopID();
			ImGui::NextColumn();

			ImGui::PushItemWidth(100.0f);

			if (ImGui::Button("Open config directory"))
				cfg->openConfigDir();

			if (ImGui::Button("Create config", { 100.0f, 25.0f }))
				cfg->add(buffer.c_str());

			if (ImGui::Button("Reset config", { 100.0f, 25.0f }))
				ImGui::OpenPopup("Config to reset");

			if (ImGui::BeginPopup("Config to reset")) {
				static constexpr const char* names[]{ "Whole", "Aimbot", "Chams", "Discord", "Glow", "Misc", "Changer", "TriggerBot", "Visuals"};
				for (int i = 0; i < IM_ARRAYSIZE(names); i++) {
					if (i == 1) ImGui::Separator();

					if (ImGui::Selectable(names[i])) {
						switch (i) {
						case 0: cfg->reset(); break;
						case 1:	cfg->a = {}; break;
						case 2:	cfg->c = {};  break;
						case 3:	cfg->d = {}; break;
						case 4:	cfg->g = {}; break;
						case 5:	cfg->m = {}; break;
						case 6:	cfg->s = {}; cfg->ch = {}; break;
						case 7:	cfg->t = {}; break;
						case 8:	cfg->v = {}; break;
						}
					}
				}
				ImGui::EndPopup();
			}
			if (currentConfig != -1) {
				if (ImGui::Button("Load selected", { 100.0f, 25.0f })) {
					cfg->load(currentConfig, incrementalLoad);
				}
				if (ImGui::Button("Save selected", { 100.0f, 25.0f }))
					ImGui::OpenPopup("##reallySave");
				if (ImGui::BeginPopup("##reallySave"))
				{
					ImGui::TextUnformatted("Are you sure?");
					if (ImGui::Button("No", { 45.0f, 0.0f }))
						ImGui::CloseCurrentPopup();
					ImGui::SameLine();
					if (ImGui::Button("Yes", { 45.0f, 0.0f }))
					{
						cfg->save(currentConfig);
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				if (ImGui::Button("Delete selected", { 100.0f, 25.0f }))
					ImGui::OpenPopup("##reallyDelete");
				if (ImGui::BeginPopup("##reallyDelete"))
				{
					ImGui::TextUnformatted("Are you sure?");
					if (ImGui::Button("No", { 45.0f, 0.0f }))
						ImGui::CloseCurrentPopup();
					ImGui::SameLine();
					if (ImGui::Button("Yes", { 45.0f, 0.0f }))
					{
						cfg->remove(currentConfig);
						if (static_cast<std::size_t>(currentConfig) < configItems.size())
							buffer = configItems[currentConfig];
						else
							buffer.clear();
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			ImGui::Columns(1);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}
