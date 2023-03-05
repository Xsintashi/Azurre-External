#include "GUI.h"

#include "SDK/Interfaces.h"
#include "SDK/LocalPlayer.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include <string>
#include "SDK/Entity.h"
#include "Config.h"
#include "Hacks/SkinChanger.h"
#include "SDK/GlobalVars.h"

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

			if (GUI::position.x >= 0 &&
				GUI::position.x <= GUI::WIDTH &&
				GUI::position.y >= 0 && GUI::position.y <= 19)
				SetWindowPos(
					GUI::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
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
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void GUI::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool GUI::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
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
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

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

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

static int skinID = 0;
static float wear = 0.1f;
static int seed = 10;
static int statTrak = 10;
static int quality = 0;
static int itemIndex = 0;
std::string nametag = "";

void GUI::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"Azurre External 0.1",
		&isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::Text("Hello xs9 :)");
	if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip)) {
		if (ImGui::BeginTabItem("Aimbot")) {
			ImGui::Text("Soon!");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("TriggerBot")) {
			ImGui::Checkbox("Enabled", &cfg->t.enabled);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Glow")) {
			ImGui::Text("Soon!");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("ESP")) {
			ImGui::Text("Soon!");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Misc")) {
			ImGui::Checkbox("Bunny-Hop", &cfg->m.bhop);
			ImGui::Checkbox("Fix Tablet Signal", &cfg->m.fixTablet);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Visuals")) {
			ImGui::Checkbox("Thirdperson", &cfg->v.thirdPerson);
			//ImGui::Checkbox("No 3DSky", &cfg->v.no3DSky);
			ImGui::Checkbox("No Smoke", &cfg->v.noSmoke);
			ImGui::SetNextItemWidth(200.0f);
			ImGui::SliderInt("##flash", &cfg->v.flashReduction, 0, 100, "Flash reduction: %d%%");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Skin Changer")) {
			ImGui::PushItemWidth(200.0f);
			ImGui::Combo("##1", &itemIndex, [](void* data, int idx, const char** out_text) {
				*out_text = Skin::weapon_names[idx].name;
			return true;
				}, nullptr, Skin::weapon_names.size(), 5);

			ImGui::InputInt("Skin ID", &skinID);
			ImGui::InputFloat("Wear", &wear);
			ImGui::InputInt("Seed", &seed);
			ImGui::InputInt("StatTrak", &statTrak);
			ImGui::Combo("Quality", &quality, "Normal\0Genuine\0Vintage\0?\0Unique\0Community\0Valve\0Protoype\0Customized\0StatTrak\0Completed\0Souvenir\0");
			//ImGui::InputText("NameTag", &nametag);
			ImGui::PopItemWidth();

			if (ImGui::Button("Update"))
				Skin::add(Skin::weapon_names[itemIndex].definition_index, skinID, wear, seed, statTrak,quality , nametag);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Players")) {
			if (ImGui::BeginTable("Players List", 5))
			{
				ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Player", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Health", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Armor", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Money", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableHeadersRow();
				for (unsigned int row = 0; row < entityData.size(); row++)
				{
					auto teamColor = entityData[row].teamNumber == 2 ? ImVec4{ 0.92f, 0.82f, .54f, 1.f } : ImVec4{ 0.26f, 0.59f, 0.98f, 1.f };
					auto hpColor = entityData[row].health < 50 ? entityData[row].health < 25 ? ImVec4{ 1.f, .0f, .0f, 1.f } : ImVec4{ 1.f, 1.f, .0f, 1.f } : ImVec4{ 0.f, 1.f, .0f, 1.f } ;
					
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%i", entityData[row].idx);
					ImGui::TableNextColumn();
					ImGui::TextColored(teamColor, "%s", entityData[row].name.c_str());
					ImGui::TableNextColumn();
					ImGui::TextColored(hpColor, "%s", entityData[row].health < 1 ? "DEAD" : std::to_string(entityData[row].health).c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%i", entityData[row].armor);
					ImGui::TableNextColumn();
					ImGui::Text("$%i", entityData[row].money);
				}
				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Debug")) {

			static auto frameRate = 1.0f;
			frameRate = 0.9f * frameRate + 0.1f * globalVars->absoluteFrameTime;
			const int framePerSecond = frameRate != 0.0f ? static_cast<int>(1 / frameRate) : 0;
			const int tickRate = static_cast<int>(1 / globalVars->intervalPerTick); //tps

			ImGui::Text("Fps: %i", framePerSecond);
			ImGui::Text("Tick: %i", tickRate);
			ImGui::Text("Client: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IClient);
			ImGui::Text("ClientState: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IClientState);
			ImGui::Text("Engine: "); ImGui::SameLine(); ImGui::TextColored({ 0.0f, 0.38f, 1.0f, 1.0f }, "0x%p", IEngine);
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
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
		
	ImGui::End();
}
