#include "GUI.h"
#include "Config.h"
#include "Core.h"
#include "Helpers.h"

#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/Interfaces.h"
#include "SDK/LocalPlayer.h"
#include "SDK/UserInterface.h"

#include "Hacks/Clantag.h"
#include "Hacks/ESP.h"
#include "Hacks/SkinChanger.h"
#include "Hacks/Misc.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Lib/imgui/imgui.h"
#include "../Lib/imgui/ImGuiCustom.h"
#include "../Lib/imgui/imgui_stdlib.h"
#include "../Lib/imgui/imgui_impl_dx9.h"
#include "../Lib/imgui/imgui_impl_win32.h"

#include "PNGTexture.h"
#include "resources.h"

#include <string>
#include <dwmapi.h>
#include <d3d9.h>
#include <strsafe.h>
#include <shlobj_core.h>
#include <KnownFolders.h>

#pragma execution_character_set("utf-8")

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
	screenSize = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) };

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = 0;
	windowClass.hIcon = 0;
	windowClass.hCursor = LoadCursor(NULL, IDC_CROSS);
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "azurre";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	int x = static_cast<int>(screenSize.x);
	int y = static_cast<int>(screenSize.y);

	window = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TRANSPARENT,
		"azurre",
		windowName,
		WS_POPUP | WS_VISIBLE,
		0,
		0,
		x,
		y,
		0,
		0,
		windowClass.hInstance,
		0
	);

	SetLayeredWindowAttributes(window, NULL, 255, LWA_ALPHA);

	RECT clientArea{};
	GetClientRect(window, &clientArea);
	RECT windowArea{};
	GetWindowRect(window, &windowArea);
	POINT diff{};
	ClientToScreen(window, &diff);

	const MARGINS margins{
		windowArea.left + (diff.x - windowArea.left),
		windowArea.top + (diff.y - windowArea.top),
		clientArea.right,
		clientArea.bottom
	};
	DwmExtendFrameIntoClientArea(window, &margins);

	ShowWindow(window, SW_SHOW); 
	SetWindowPos(window, HWND_TOPMOST, windowArea.left, windowArea.top, 1920, 1080, SWP_NOMOVE | SWP_NOSIZE);
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
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
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

static ImFont* addFontFromVFONT(const std::string& path, float size, const ImWchar* glyphRanges, bool merge) noexcept
{
	auto file = Helpers::loadBinaryFile(path);
	if (!Helpers::decodeVFONT(file))
		return nullptr;

	ImFontConfig cfg;
	cfg.FontData = file.data();
	cfg.FontDataSize = file.size();
	cfg.FontDataOwnedByAtlas = false;
	cfg.MergeMode = merge;
	cfg.GlyphRanges = glyphRanges;
	cfg.SizePixels = size;

	return ImGui::GetIO().Fonts->AddFont(&cfg);
}

void GUI::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

#if !defined(_DEBUG)

	ImFontConfig cfgFont;
	cfgFont.SizePixels = 15.0f;

	ImFont* font;
	if (PWSTR pathToFonts; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &pathToFonts))) {
		const std::filesystem::path path{ pathToFonts };
		CoTaskMemFree(pathToFonts);
		font = io.Fonts->AddFontFromMemoryTTF((void*)resource::ubuntuFont, sizeof(resource::ubuntuFont), 15.0f, &cfgFont, Helpers::getFontGlyphRanges());
		if (!font)
			io.Fonts->AddFontDefault(&cfgFont);
	}
#endif
	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	auto& guiSettings = ImGui::GetStyle();
	guiSettings.WindowBorderSize = cfg->u.windowBorder ? 1.f : 0.f;
	guiSettings.PopupBorderSize = cfg->u.windowBorder ? 1.f : 0.f;
	guiSettings.FrameBorderSize = cfg->u.frameBorder ? 1.f : 0.f;

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

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.f, 0);
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

	device->Present(0, 0, 0, 0);
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
	auto& guiSettings = ImGui::GetStyle();

	if (cfg->u.roundBorder) {
		guiSettings.WindowRounding = 7.f;
		guiSettings.ChildRounding = 10.0f;
		guiSettings.FrameRounding = 10.0f;
		guiSettings.GrabRounding = 10.0f;
		guiSettings.PopupRounding = 10.0f;
		guiSettings.ScrollbarRounding = 10.0f;
		guiSettings.LogSliderDeadzone = 10.0f;
		guiSettings.TabRounding = 7.0f;
	}
	else {
		guiSettings.WindowRounding = 0.f;
		guiSettings.ChildRounding = 0.0f;
		guiSettings.FrameRounding = 0.0f;
		guiSettings.GrabRounding = 0.0f;
		guiSettings.PopupRounding = 0.0f;
		guiSettings.ScrollbarRounding = 0.0f;
		guiSettings.LogSliderDeadzone = 0.0f;
		guiSettings.TabRounding = 0.0f;
	}

	guiSettings.AntiAliasedLines = cfg->u.antiAliasing;
    guiSettings.AntiAliasedLinesUseTex = cfg->u.antiAliasing;
    guiSettings.AntiAliasedFill = cfg->u.antiAliasing;
    guiSettings.WindowBorderSize = cfg->u.windowBorder ? 1.f : 0.f;
    guiSettings.PopupBorderSize = cfg->u.windowBorder ? 1.f : 0.f;
    guiSettings.FrameBorderSize = cfg->u.frameBorder ? 1.f : 0.f;
    guiSettings.WindowTitleAlign = cfg->u.centerTitle ? ImVec2{0.5f, 0.5f} : ImVec2{ 0.f, 0.5f };
}

void GUI::loadWindow() noexcept {
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::Begin(
		"Loading...",
		&isRunning,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse
	);
	static int timeLeft = 15;

	if (timeLeft < 1)
		isRunning = false;

	static DWORD time = GetTickCount();
	if (GetTickCount() - time >= 1000) {
		time = GetTickCount();
		timeLeft--;
	}

	ImGui::Text("Hello xs9 :)");
	ImGui::Text("Waiting For csgo.exe...");
	ImGui::Text("CS:GO: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IConsole);
	ImGui::Text("Client: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IClient);
	ImGui::Text("Engine: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IEngine);
	if (!IConsole) ImGui::Text("\nAuto-Closing in %d seconds!", timeLeft);
	else ImGui::Text("\nPreparing...!", timeLeft);
	ImGui::End();
}

void GUI::RenderDebugWindow() noexcept {
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_FirstUseEver);
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

	int chokedPackets = mem.Read<int>(IClientState.address + Offset::signatures::clientstate_choked_commands);

	ImGui::TextUnformatted("Build date: " __DATE__ " " __TIME__);

	ImGui::Text("Fps: %i", framePerSecond);
	ImGui::Text("Tick: %i", tickRate);
	ImGui::Text("Screen Size: %2.fx%2.f", screenSize.x, screenSize.y);
	ImGui::Text("Game Size: %2.fx%2.f", gameScreenSize.x, gameScreenSize.y);
	ImGui::Text("Game Pos: %2.fx%2.f", gameScreenPos.x, gameScreenPos.y);
	ImGui::Text("CS:GO: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IConsole);
	ImGui::Text("Client: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IClient.address);
	ImGui::Text("ClientState: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IClientState.address);
	ImGui::Text("Engine: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IEngine.address);
	ImGui::Text("PlayerResource: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IPlayerResource.address);
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
	ImGui::SeparatorText("Font Test");
	ImGui::Text("Latin: "); ImGui::SameLine();	ImGui::Text((const char*)u8"Hello. Test");
	ImGui::Text("Slavonic: "); ImGui::SameLine();	ImGui::Text((const char*)u8"Cześć. Test");
	ImGui::Text("Japanese: "); ImGui::SameLine();	ImGui::Text((const char*)u8"こんにちは。 テスト");
	ImGui::Text("Chinese: "); ImGui::SameLine();	ImGui::Text((const char*)u8"你好。 測試");
	ImGui::Text("Korean: "); ImGui::SameLine();	ImGui::Text((const char*)u8"안녕하세요. 테스트");
	ImGui::Text("Cyrillic: "); ImGui::SameLine();	ImGui::Text((const char*)u8"Привет. Тест");
	ImGui::Text("Greek: "); ImGui::SameLine();	ImGui::Text((const char*)u8"Γεια. Δοκιμή");
	ImGui::Separator();
	ImGui::Checkbox("Bool Debug 0", &cfg->debug.boolDebug0);

	//ImGui::PushID("Roll");
	//static float roll = 0.f;
	//static float tempRoll = 0.f;
	//ImGui::SetNextItemWidth(128);
	//ImGui::SliderFloat("", &roll, -45.f, 45.f, "Roll: %.1f");
	//if (roll != tempRoll) {
	//	tempRoll = roll;
	//	const auto& viewAngles = mem.Read<ImVec2>(IClientState.address + Offset::signatures::dwClientState_ViewAngles);
	//	mem.Write<Vector>(IClientState.address + Offset::signatures::dwClientState_ViewAngles, { viewAngles.x, viewAngles.y, roll });
	//}
	//ImGui::PopID();
	static std::string cmd = "";
	ImGui::InputText("convar", &cmd);
	ImGui::SameLine();
	if (ImGui::Button("Send"))
		usr0::SendConsoleCommand(cmd);
	ImGui::End();
}

const static std::array<PNGTexture, 19> ranksTextures{
	resource::skillgroup0,
	resource::skillgroup1,
	resource::skillgroup2,
	resource::skillgroup3,
	resource::skillgroup4,
	resource::skillgroup5,
	resource::skillgroup6,
	resource::skillgroup7,
	resource::skillgroup8,
	resource::skillgroup9,
	resource::skillgroup10,
	resource::skillgroup11,
	resource::skillgroup12,
	resource::skillgroup13,
	resource::skillgroup14,
	resource::skillgroup15,
	resource::skillgroup16,
	resource::skillgroup17,
	resource::skillgroup18
};

void GUI::RenderPlayerList() noexcept {

	if (!showMenu && !cfg->m.playerList.hotkey.isActive() && cfg->m.playerList.hotkey.isSet())
		return;

	int windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing;

	if (cfg->m.playerList.noTitleBar)
		windowFlags |= ImGuiWindowFlags_NoTitleBar;

	if (cfg->m.playerList.pos != ImVec2{}) {
		ImGui::SetNextWindowPos(cfg->m.playerList.pos);
		cfg->m.playerList.pos = {};
	}

	ImGui::Begin(
		"Player List",
		&cfg->m.playerList.enabled,
		windowFlags
	);
	ImGui::SetNextWindowSize({ -1, -1 });
	if (ImGui::BeginTable("Players List", 8))
	{
		ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Health", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Armor", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Money", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Weapon", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Rank", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Last Place", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();
		for (unsigned int row = 0; row < gameData.playerData.size(); row++)
		{
			auto teamColor = localPlayer.get() == (uintptr_t)gameData.playerData[row].entity ? ImVec4{ 1.0f, 0.25f, 1.0f, 1.f } : gameData.playerData[row].teamNumber == 2 ? ImVec4{ 0.92f, 0.82f, .54f, 1.f } : ImVec4{ 0.26f, 0.59f, 0.98f, 1.f };
			auto hpColor = gameData.playerData[row].health < 50 ? gameData.playerData[row].health < 25 ? ImVec4{ 1.f, .0f, .0f, 1.f } : ImVec4{ 1.f, 1.f, .0f, 1.f } : ImVec4{ 0.f, 1.f, .0f, 1.f };

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%i", gameData.playerData[row].idx);
			ImGui::TableNextColumn();
			ImGui::PushID(row);
			ImGui::TextColored(teamColor, "%s", gameData.playerData[row].name.c_str());
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("%i", gameData.playerData[row].steamID);
			ImGui::PopID();
			ImGui::TableNextColumn();
			ImGui::TextColored(hpColor, "%s", gameData.playerData[row].health < 1 ? "DEAD" : std::to_string(gameData.playerData[row].health).c_str());
			ImGui::TableNextColumn();
			ImGui::Text("%i%s", gameData.playerData[row].armor, gameData.playerData[row].hasHelmet ? "+H" : "");
			ImGui::TableNextColumn();
			ImGui::Text("$%i", gameData.playerData[row].money);
			ImGui::TableNextColumn();
			ImGui::Text("%s", Skin::getWeaponIDName(gameData.playerData[row].weaponID));
			ImGui::TableNextColumn();
			ImGui::Image(ranksTextures[gameData.playerData[row].rank].getTexture(), { 2.45f /* -> proportion 49x20px */ * ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() });
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::PushFont(nullptr);
				ImGui::TextUnformatted(std::to_string(gameData.playerData[row].wins).append(" wins").c_str());
				ImGui::PopFont();
				ImGui::EndTooltip();
			}
			ImGui::TableNextColumn();
			ImGui::Text("%s", gameData.playerData[row].placename.c_str());
		}
		ImGui::EndTable();
	}
	ImGui::End();
}

void GUI::RenderMainMenu() noexcept {
	ImGui::SetNextWindowPos({ screenSize.x / 2 - 320, screenSize.y / 2 - 240 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 640, 480 });
	ImGui::Begin(
		"Azurre External",
		&isRunning,
		ImGuiWindowFlags_AlwaysAutoResize
	);

	ImGui::Text("Hello xs9 :)");
	if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip)) {
		if (ImGui::BeginTabItem("Aimbot")) {
			ImGui::PushID("aimbot");
			ImGui::BeginDisabled(cfg->restrictions);
			ImGui::Checkbox("Enabled", &cfg->a.enabled);
			ImGui::EndDisabled();
			ImGui::SameLine();
			ImGui::hotkey("", cfg->a.hotkey);
			ImGui::PopID();
			ImGui::Checkbox("Auto Shot", &cfg->a.autoShot);
			ImGui::Checkbox("Auto Stop", &cfg->a.autoStop);
			ImGui::Checkbox("Friendly Fire", &cfg->a.friendlyFire);
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
			ImGui::Checkbox("Friendly Fire", &cfg->t.friendlyFire);
			ImGui::SetNextItemWidth(200.0f);
			ImGui::SliderInt("Between Shots Delay", &cfg->t.delay, 0, 1000);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Glow")) {
			ImGui::BeginDisabled(cfg->restrictions);
			ImGui::Checkbox("Enabled", &cfg->g.enabled);
			ImGui::SameLine();
			ImGui::PushID("warning-1");
			ImGui::TextDisabled("?");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Glow causes instant red trustfactor! Use at your own risk!");
			ImGui::PopID();
			ImGuiCustom::colorPicker("Enemies", cfg->g.enemy.color.data(), &cfg->g.enemy.color[3], nullptr, nullptr, &cfg->g.enemy.enabled);
			ImGuiCustom::colorPicker("Allies", cfg->g.ally.color.data(), &cfg->g.ally.color[3], nullptr, nullptr, &cfg->g.ally.enabled);
			ImGui::EndTabItem();
			ImGui::EndDisabled();
		}
		if (ImGui::BeginTabItem("Chams")) {
			ImGui::BeginDisabled(cfg->restrictions);
			ImGui::Checkbox("Enabled", &cfg->c.enabled);
			ImGuiCustom::colorPicker("Enemies", cfg->c.enemy);
			ImGuiCustom::colorPicker("Allies", cfg->c.ally);
			ImGui::SetNextItemWidth(200.0f);
			ImGui::SliderFloat("Brightness", &cfg->c.brightness, 0.1f, 1.f);
			ImGui::EndTabItem();
			ImGui::EndDisabled();
		}
		if (ImGui::BeginTabItem("ESP")) {
			static int list = 0;
			static int spotted = 0;
			constexpr std::array categories{ "Allies", "Enemies Occluded", "Enemies Visible" };
			ImGui::Checkbox("Enabled", &cfg->esp.enabled);
			ImGui::Combo("##player", &list, "Allies\0Enemies\0");
			if (list) ImGui::Combo("##players", &spotted, "Occluded\0Visible\0");
			else spotted = 0;
			ImGuiCustom::colorPicker("Name", cfg->esp.players[categories[list + spotted]].other.names.color.data(), nullptr, nullptr, nullptr, &cfg->esp.players[categories[list + spotted]].other.names.enabled);
			ImGuiCustom::colorPicker("Weapon", cfg->esp.players[categories[list + spotted]].other.weapons.color.data(), nullptr, nullptr, nullptr, &cfg->esp.players[categories[list + spotted]].other.weapons.enabled);
#if defined(_DEBUG)
			ImGui::Checkbox("Skeleton", &cfg->esp.players[categories[list + spotted]].skeleton); // DEBUG bones pos
#endif
			ImGui::Checkbox("Boxes", &cfg->esp.players[categories[list + spotted]].box.enabled);
			ImGui::PushID("boxes");
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				if (ImGui::Checkbox("Gradient Color", &cfg->esp.players[categories[list + spotted]].box.gradientColor)) {
					ImGuiCustom::colorPicker("Top Color", cfg->esp.players[categories[list + spotted]].box.grandientTop.color.data(), nullptr, nullptr, nullptr, nullptr);
					ImGuiCustom::colorPicker("Bottom Color", cfg->esp.players[categories[list + spotted]].box.grandientBottom.color.data(), nullptr, nullptr, nullptr, nullptr);
				}
				else
					ImGuiCustom::colorPicker("Solid Color", cfg->esp.players[categories[list + spotted]].box.solid.color.data(), nullptr, nullptr, nullptr, nullptr);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGui::Checkbox("Health Bar", &cfg->esp.players[categories[list + spotted]].healthBar.enabled);
			ImGui::PushID("healbar");
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGuiCustom::colorPicker("Solid Color", cfg->esp.players[categories[list + spotted]].healthBar.solidColor.color.data(), nullptr, nullptr, nullptr, &cfg->esp.players[categories[list + spotted]].healthBar.solidColor.enabled);
				ImGuiCustom::colorPicker("Health Number", cfg->esp.players[categories[list + spotted]].healthBar.showHealthNumber.color.data(), nullptr, nullptr, nullptr, &cfg->esp.players[categories[list + spotted]].healthBar.showHealthNumber.enabled);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGuiCustom::colorPicker("Lines", cfg->esp.players[categories[list + spotted]].other.lines.color.data(), nullptr, nullptr, nullptr, &cfg->esp.players[categories[list + spotted]].other.lines.enabled);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Misc")) {
			ImGui::PushItemWidth(96.f);
			ImGui::Combo("Restrictions", &cfg->restrictions, "None\0Read Only");
			ImGuiCustom::classicHotkey("Menu Key", cfg->m.menuKey);
			ImGui::Checkbox("Bunny Hop", &cfg->m.bhop);
			ImGui::BeginDisabled(cfg->restrictions);
			ImGui::Checkbox("Fix Tablet Signal", &cfg->m.fixTablet);
			ImGui::Checkbox("Engine Radar", &cfg->m.radarHack);
			ImGui::Checkbox("Fast Stop", &cfg->m.autoStop);
			ImGui::Checkbox("Grenade Trajectory", &cfg->m.grenadeTrajectory);
			ImGui::EndDisabled();
			ImGui::InputText("Hit Sound", &cfg->m.hitSound);
			ImGui::PushID("hitmarker");
			ImGui::Combo("Type", &cfg->m.hitMarker.type, "None\0Cross\0Cross Fading\0"); ImGui::SameLine(); ImGuiCustom::colorPicker("Hit Marker", cfg->m.hitMarker.color.color.data());
			ImGui::SameLine();
			ImGui::SliderFloat("##as", &cfg->m.hitMarker.time, 0.1f, 1.5f, "Time: %.2fs");
			ImGui::PopID();

			ImGui::PushID("RecoilCrosshair");
			ImGuiCustom::colorPicker("Recoil Crosshair", cfg->m.recoilCrosshair.color.color.data(), nullptr, &cfg->m.recoilCrosshair.color.rainbow, &cfg->m.recoilCrosshair.color.rainbowSpeed, &cfg->m.recoilCrosshair.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::PushItemWidth(120.f);
				ImGui::InputFloat("Length", &cfg->m.recoilCrosshair.length, 0.1f, 1.f);
				ImGui::InputFloat("Gap", &cfg->m.recoilCrosshair.gap, 0.1f, 1.f);
				ImGui::InputFloat("Thickness", &cfg->m.recoilCrosshair.thickness, 0.1f, 1.f);
				ImGui::PopItemWidth();
				ImGuiCustom::colorPicker("Outline", cfg->m.recoilCrosshair.outline, &cfg->m.recoilCrosshair.outline.enabled, &cfg->m.recoilCrosshair.outlineThickness);
				ImGui::Checkbox("Dot", &cfg->m.recoilCrosshair.dot);
				ImGui::SameLine();
				ImGui::Checkbox("TStyle", &cfg->m.recoilCrosshair.TStyle);
				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::PushID("SniperCrosshair");
			ImGuiCustom::colorPicker("Sniper Crosshair", cfg->m.sniperCrosshair.color.color.data(), nullptr, &cfg->m.sniperCrosshair.color.rainbow, &cfg->m.sniperCrosshair.color.rainbowSpeed, &cfg->m.sniperCrosshair.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::PushItemWidth(120.f);
				ImGui::InputFloat("Length", &cfg->m.sniperCrosshair.length, 0.1f, 1.f);
				ImGui::InputFloat("Gap", &cfg->m.sniperCrosshair.gap, 0.1f, 1.f);
				ImGui::InputFloat("Thickness", &cfg->m.sniperCrosshair.thickness, 0.1f, 1.f);
				ImGui::PopItemWidth();
				ImGuiCustom::colorPicker("Outline", cfg->m.sniperCrosshair.outline, &cfg->m.sniperCrosshair.outline.enabled, &cfg->m.sniperCrosshair.outlineThickness);
				ImGui::Checkbox("Dot", &cfg->m.sniperCrosshair.dot);
				ImGui::SameLine();
				ImGui::Checkbox("TStyle", &cfg->m.sniperCrosshair.TStyle);
				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::PushID("Minimap");
			ImGui::Checkbox("Minimap", &cfg->m.minimap.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::hotkey("On Key", cfg->m.minimap.hotkey);
				ImGui::Checkbox("No Title", &cfg->m.minimap.noTitleBar);
				ImGui::Checkbox("No Background", &cfg->m.minimap.noBackground);
				ImGui::Checkbox("Show Players", &cfg->m.minimap.showPlayers);
				ImGui::Checkbox("Show Dormant", &cfg->m.minimap.showDormant);
				ImGui::Checkbox("Show Weapons", &cfg->m.minimap.showWeapons);
				ImGui::Checkbox("Show Grenades", &cfg->m.minimap.showGrenades);
				ImGui::SliderFloat("##scale", &cfg->m.minimap.scale, 0.25f, 2.f, "Scale: %.2f");
				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::PushID("Keybinds");
			ImGui::Checkbox("Keybind List", &cfg->m.keybinds.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::Checkbox("No Title", &cfg->m.keybinds.noTitleBar);
				ImGui::Checkbox("No Background", &cfg->m.keybinds.noBackground);
				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::PushID("BombTimer");
			ImGui::Checkbox("Bomb Timer", &cfg->m.bombTimer.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::Checkbox("No Title", &cfg->m.bombTimer.noTitleBar);
				ImGuiCustom::colorPicker("Color", cfg->m.bombTimer.barColor.color.data(), nullptr, &cfg->m.bombTimer.barColor.rainbow, &cfg->m.bombTimer.barColor.rainbowSpeed);
				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::PushID("SpectatorList");
			ImGui::Checkbox("Spectator List", &cfg->m.spectatorList.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::Checkbox("No Title Bar", &cfg->m.spectatorList.noTitleBar);
				ImGui::Checkbox("No Background", &cfg->m.spectatorList.noBackground);
				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::PushID("PlayerList");
			ImGui::Checkbox("Player List", &cfg->m.playerList.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::Checkbox("No Title", &cfg->m.playerList.noTitleBar);
				ImGui::hotkey("On Key", cfg->m.playerList.hotkey);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGui::PushID("FakeLag");
			ImGui::BeginDisabled(cfg->restrictions);
			ImGui::Checkbox("Fake Lag", &cfg->m.fakeLag.enabled);
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::Combo("Mode", &cfg->m.fakeLag.type, "Static\0Adaptative\0Random\0");
				ImGui::SliderInt("##limit", &cfg->m.fakeLag.limit, 1, 16, "Limit: %d");
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGui::Combo("ClanTag", &cfg->clanTag.mode, "None\0Azurre\0Clock\0Reverse\0Velocity\0Position\0HP\0\\n Overflow\0Custom\0");
			if (cfg->clanTag.mode == 8) {
				ImGui::PushID("ClanTagCustom");
				ImGui::SameLine();
				if (ImGui::Button("..."))
					ImGui::OpenPopup("##custom");

				if (ImGui::BeginPopup("##custom")) {
					ImGui::PushItemWidth(120.f);

					ImGui::Combo("Type", &cfg->clanTag.custom.type, "Static\0Rotate\0Rotate Backwards\0Add\0Remove\0");
					if (ImGui::InputText("Name", &cfg->clanTag.custom.tag))
						Clan::update(false, true);
					if (ImGui::InputText("Team", &cfg->clanTag.custom.teamTag))
						Clan::update(false, true);
					if (ImGui::InputText("Prefix", &cfg->clanTag.custom.prefix))
						Clan::update(false, true);
					if (ImGui::InputText("Postfix", &cfg->clanTag.custom.postfix))
						Clan::update(false, true);
					ImGui::SliderFloat("##speed", &cfg->clanTag.custom.speed, 0.01f, 1.f, "Speed: %.2f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);
					ImGui::Checkbox("Hide Name", &cfg->clanTag.custom.hideName);
					ImGui::PopItemWidth();
					ImGui::EndPopup();
				}
				ImGui::PopID();
			}
			ImGui::EndDisabled();
			ImGui::PopItemWidth();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Visuals")) {
			ImGui::BeginDisabled(cfg->restrictions);
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
			ImGui::Checkbox("No Shadows", &cfg->v.noShadows);
			ImGui::Checkbox("No 3DSky", &cfg->v.no3DSky);
			ImGui::Checkbox("No Particles", &cfg->v.noParticles);
			ImGui::PushItemWidth(96.f);
			ImGui::Combo("Skybox", &cfg->v.skybox, skyboxList.data(), skyboxList.size());
			ImGui::SliderInt("##flash", &cfg->v.flashReduction, 0, 100, "Flash reduction: %d%%");
			ImGui::SliderInt("##fov", &cfg->v.fov, 30, 150, "Fov: %d");
			ImGui::PopItemWidth();
			ImGui::EndDisabled();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Changer")) {
			ImGui::BeginDisabled(cfg->restrictions);
			static int itemIndex = 0;
			static int itemIndexTemp = 0;
			ImGui::PushItemWidth(96.f);
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
			ImGui::EndDisabled();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Discord")) {
			ImGui::Checkbox("Enabled", &cfg->d.enabled);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("GUI")) {

			if (ImGui::Combo("Menu colors", &cfg->u.menuColors, "Azurre\0Genshi\0Emerald\0Bloddy Red\0Gold Mine\0Pandora\0Holy Light\0Deep Dark\0Visual Studio\0GoldSrc\0ImGui\0Neverlose\0Aimware\0Onetap\0Custom\0"))
				updateColors();

			ImGui::Checkbox("AntiAliasing",&cfg->u.antiAliasing);
			ImGui::Checkbox("Center Title",&cfg->u.centerTitle);
			ImGui::Checkbox("Frame Border",&cfg->u.frameBorder);
			ImGui::Checkbox("Round Border",&cfg->u.roundBorder);
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

void watermark() {
#if _DEBUG
	constexpr const char* builtDate = __DATE__;
	constexpr const char* builtTime = __TIME__;
	const std::string watermark = std::string("Azurre | Built: ").append(builtDate).append(" ").append(builtTime).append(" | Hello xs9 :)");
#else
	static auto frameRate = 1.0f;
	frameRate = 0.9f * frameRate + 0.1f * globalVars->absoluteFrameTime;
	const int framePerSecond = frameRate != 0.0f ? static_cast<int>(1 / frameRate) : 0;
	const std::string watermark = std::string("Azurre | FPS: ").append(std::to_string(framePerSecond)).append(" | Hello xs9 :)");
#endif
	for (unsigned int i = 0; i < watermark.size(); i++) {

		constexpr float pi = std::numbers::pi_v<float>;
		const float r = static_cast<float>(std::sin((i / 16) - 5.f * globalVars->realTime / 2.f) * 0.5f + 0.5f);
		const float g = static_cast<float>(std::sin((i / 16) - 5.f * globalVars->realTime / 2.f + 2 * pi / 3) * 0.5f + 0.5f);
		const float b = static_cast<float>(std::sin((i / 16) - 5.f * globalVars->realTime / 2.f + 4 * pi / 3) * 0.5f + 0.5f);

		const auto color = ImGui::GetColorU32(ImVec4{ r, g, b, 1.f });

		std::string sym(1, watermark[i]);
		ImGui::GetForegroundDrawList()->AddText({ i * 8.f, 2 * static_cast<float>(sin(static_cast<int>(globalVars->realTime * 10) % (i + 1))) }, color, sym.c_str());
		ImGui::GetForegroundDrawList()->AddText({ i * 8.f + 1.f , 2 * static_cast<float>(sin(static_cast<int>(globalVars->realTime * 10) % (i + 1))) + 1.f }, ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), sym.c_str());
	}
}

void GUI::overlay() noexcept {
	ESP::render();
	watermark();
#if defined(_DEBUG)
	//ImGui::GetBackgroundDrawList()->AddRect( //Draws Rectangle around csgo window
	//	gameScreenPos,
	//	gameScreenPosEnd,
	//	ImGui::GetColorU32({ 0.f, 0.4f, 1.f, 0.5f }),
	//	0, 0, 2.5f);
#endif
}