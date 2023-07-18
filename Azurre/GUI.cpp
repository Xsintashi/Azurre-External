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

#include <algorithm>
#include <string>
#include <dwmapi.h>
#include <d3d9.h>
#include <strsafe.h>
#include <shlobj_core.h>
#include <KnownFolders.h>
#include "Hacks/Aimbot.h"

#pragma execution_character_set("utf-8")

constexpr const char* colorTeam[] = {
	"Gray",
	"Blank",
	"Yellow",
	"Purple",
	"Green",
	"Blue",
	"Orange"
};

constexpr const char* ranks[] = {
	"Hidden / Expired",
	"Silver 1",
	"Silver 2",
	"Silver 3",
	"Silver 4",
	"Silver elite",
	"Silver elite master",
	"Gold nova 1",
	"Gold nova 2",
	"Gold nova 3",
	"Gold nova master",
	"Master guardian 1",
	"Master guardian 2",
	"Master guardian elite",
	"Distinguished master guardian",
	"Legendary eagle",
	"Legendary eagle master",
	"Supreme master first class",
	"The global elite"
};

constexpr const char* medalNames[] = {
	"None",
	"5 years",
	"DreamHack SteelSeries 2013 CS:GO Champion",
	"DreamHack SteelSeries 2013 CS:GO Finalist",
	"DreamHack SteelSeries 2013 CS:GO Semifinalist",
	"DreamHack SteelSeries 2013 CS:GO Quarterfinalist",
	"EMS One Katowice 2014 CS:GO Champion",
	"EMS One Katowice 2014 CS:GO Finalist",
	"EMS One Katowice 2014 CS:GO Semifinalist",
	"EMS One Katowice 2014 CS:GO Quarterfinalist",
	"ESL One Cologne 2014 CS:GO Champion",
	"ESL One Cologne 2014 CS:GO Finalist",
	"ESL One Cologne 2014 CS:GO Semifinalist",
	"ESL One Cologne 2014 CS:GO Quarterfinalist",
	"ESL One Cologne 2014 Pick 'Em Challenge Bronze",
	"ESL One Cologne 2014 Pick 'Em Challenge Silver",
	"ESL One Cologne 2014 Pick 'Em Challenge Gold",
	"DreamHack Winter 2014 CS:GO Champion",
	"DreamHack Winter 2014 CS:GO Finalist",
	"DreamHack Winter 2014 CS:GO Semifinalist",
	"DreamHack Winter 2014 CS:GO Quarterfinalist",
	"DreamHack Winter 2014 Pick 'Em Challenge Bronze",
	"DreamHack Winter 2014 Pick 'Em Challenge Silver",
	"DreamHack Winter 2014 Pick 'Em Challenge Gold",
	"ESL One Katowice 2015 CS:GO Champion",
	"ESL One Katowice 2015 CS:GO Finalist",
	"ESL One Katowice 2015 CS:GO Semifinalist",
	"ESL One Katowice 2015 CS:GO Quarterfinalist",
	"ESL One Katowice 2015 Pick 'Em Challenge Bronze",
	"ESL One Katowice 2015 Pick 'Em Challenge Silver",
	"ESL One Katowice 2015 Pick 'Em Challenge Gold",
	"ESL One Cologne 2015 CS:GO Champion",
	"ESL One Cologne 2015 CS:GO Finalist",
	"ESL One Cologne 2015 CS:GO Semifinalist",
	"ESL One Cologne 2015 CS:GO Quarterfinalist",
	"ESL One Cologne 2015 Pick 'Em Challenge Bronze",
	"ESL One Cologne 2015 Pick 'Em Challenge Silver",
	"ESL One Cologne 2015 Pick 'Em Challenge Gold",
	"DreamHack Cluj-Napoca 2015 Pick 'Em Challenge Bronze",
	"DreamHack Cluj-Napoca 2015 Pick 'Em Challenge Silver",
	"DreamHack Cluj-Napoca 2015 Pick 'Em Challenge Gold",
	"DreamHack Cluj-Napoca 2015 Fantasy Team Bronze",
	"DreamHack Cluj-Napoca 2015 Fantasy Team Silver",
	"DreamHack Cluj-Napoca 2015 Fantasy Team Gold",
	"DreamHack Cluj-Napoca 2015 CS:GO Champion",
	"DreamHack Cluj-Napoca 2015 CS:GO Finalist",
	"DreamHack Cluj-Napoca 2015 CS:GO Semifinalist",
	"DreamHack Cluj-Napoca 2015 CS:GO Quarterfinalist",
	"MLG Columbus 2016 Pick 'Em Challenge Bronze",
	"MLG Columbus 2016 Pick 'Em Challenge Silver",
	"MLG Columbus 2016 Pick 'Em Challenge Gold",
	"MLG Columbus 2016 Fantasy Team Bronze",
	"MLG Columbus 2016 Fantasy Team Silver",
	"MLG Columbus 2016 Fantasy Team Gold",
	"MLG Columbus 2016 CS:GO Champion",
	"MLG Columbus 2016 CS:GO Finalist",
	"MLG Columbus 2016 CS:GO Semifinalist",
	"MLG Columbus 2016 CS:GO Quarterfinalist",
	"ESL One Cologne 2016 CS:GO Champion",
	"ESL One Cologne 2016 CS:GO Finalist",
	"ESL One Cologne 2016 CS:GO Semifinalist",
	"ESL One Cologne 2016 CS:GO Quarterfinalist",
	"Cologne 2016 Pick 'Em Challenge Bronze",
	"Cologne 2016 Pick 'Em Challenge Silver",
	"Cologne 2016 Pick 'Em Challenge Gold",
	"Cologne 2016 Fantasy Team Challenge Bronze",
	"Cologne 2016 Fantasy Team Challenge Silver",
	"Cologne 2016 Fantasy Team Challenge Gold",
	"ELEAGUE Atlanta 2017 CS:GO Champion",
	"ELEAGUE Atlanta 2017 CS:GO Finalist",
	"ELEAGUE Atlanta 2017 CS:GO Semifinalist",
	"ELEAGUE Atlanta 2017 CS:GO Quarterfinalist",
	"Atlanta 2017 Pick 'Em Challenge Bronze",
	"Atlanta 2017 Pick 'Em Challenge Silver",
	"Atlanta 2017 Pick 'Em Challenge Gold",
	"PGL Krakow 2017 CS:GO Champion",
	"PGL Krakow 2017 CS:GO Finalist",
	"PGL Krakow 2017 CS:GO Semifinalist",
	"PGL Krakow 2017 CS:GO Quarterfinalist",
	"Krakow 2017 Pick 'Em Challenge Bronze",
	"Krakow 2017 Pick 'Em Challenge Silver",
	"Krakow 2017 Pick 'Em Challenge Gold",
	"ELEAGUE Boston 2018 CS:GO Champion",
	"ELEAGUE Boston 2018 CS:GO Finalist",
	"ELEAGUE Boston 2018 CS:GO Semifinalist",
	"ELEAGUE Boston 2018 CS:GO Quarterfinalist",
	"Boston 2018 Pick 'Em Challenge Bronze",
	"Boston 2018 Pick 'Em Challenge Silver",
	"Boston 2018 Pick 'Em Challenge Gold",
	"FACEIT London 2018 CS:GO Champion",
	"FACEIT London 2018 CS:GO Finalist",
	"FACEIT London 2018 CS:GO Semifinalist",
	"FACEIT London 2018 CS:GO Quarterfinalist",
	"London 2018 Pick 'Em Challenge Bronze",
	"London 2018 Pick 'Em Challenge Silver",
	"London 2018 Pick 'Em Challenge Gold",
	"10 years",
	"Loyalty",
	"IEM Katowice 2019 CS:GO Champion",
	"IEM Katowice 2019 CS:GO Finalist",
	"IEM Katowice 2019 CS:GO Semifinalist",
	"IEM Katowice 2019 CS:GO Quarterfinalist",
	"StarLadder Berlin 2019 CS:GO Champion",
	"StarLadder Berlin 2019 CS:GO Finalist",
	"StarLadder Berlin 2019 CS:GO Semifinalist",
	"StarLadder Berlin 2019 CS:GO Quarterfinalist",
	"PGL Stockholm 2021 CS:GO Champion",
	"PGL Stockholm 2021 CS:GO Finalist",
	"PGL Stockholm 2021 CS:GO Semifinalist",
	"PGL Stockholm 2021 CS:GO Quarterfinalist",
	"PGL Antwerp 2022 CS:GO Champion",
	"PGL Antwerp 2022 CS:GO Finalist",
	"PGL Antwerp 2022 CS:GO Semifinalist",
	"PGL Antwerp 2022 CS:GO Quarterfinalist",
	"10 years birthday",
	"BLAST.tv Paris 2023 CS:GO Champion",
	"BLAST.tv Paris 2023 CS:GO Finalist",
	"BLAST.tv Paris 2023 CS:GO Semifinalist",
	"BLAST.tv Paris 2023 CS:GO Quarterfinalist",
	"Operation Payback #1",
	"Operation Payback #2",
	"Operation Payback #3",
	"Operation Bravo #1",
	"Operation Bravo #2",
	"Operation Bravo #3",
	"Operation Phoenix #1",
	"Operation Phoenix #2",
	"Operation Phoenix #3",
	"Operation Breakout #1",
	"Operation Breakout #2",
	"Operation Breakout #3",
	"Operation Vanguard #1",
	"Operation Vanguard #2",
	"Operation Vanguard #3",
	"Operation Bloodhound #1",
	"Operation Bloodhound #2",
	"Operation Bloodhound #3",
	"2015 Service #1",
	"2015 Service #2",
	"Operation Wildfire #1",
	"Operation Wildfire #2",
	"Operation Wildfire #3",
	"2016 Service #1",
	"2016 Service #2",
	"2016 Service #3",
	"2016 Service #4",
	"2016 Service #5",
	"2016 Service #6",
	"2017 Service #1",
	"2017 Service #2",
	"2017 Service #3",
	"2017 Service #4",
	"2017 Service #5",
	"2017 Service #6",
	"2017 Service #7",
	"2018 Service #1",
	"2018 Service #2",
	"2018 Service #3",
	"2018 Service #4",
	"2018 Service #5",
	"2018 Service #6",
	"2019 Service #1",
	"2019 Service #2",
	"2019 Service #3",
	"2019 Service #4",
	"2019 Service #5",
	"2019 Service #6",
	"Operation Hydra #1",
	"Operation Hydra #2",
	"Operation Hydra #3",
	"Operation Hydra #3",
	"Operation Shattered #1",
	"Operation Shattered #2",
	"Operation Shattered #3",
	"Operation Shattered #4",
	"2020 Service #1",
	"2020 Service #2",
	"2020 Service #3",
	"2020 Service #4",
	"2020 Service #5",
	"2020 Service #6",
	"alyx_04",
	"alyx_07",
	"alyx_09",
	"alyx_05",
	"alyx_12",
	"alyx_01",
	"alyx_02",
	"alyx_03",
	"alyx_06",
	"alyx_08",
	"alyx_11",
	"Operation Broken Fang #1",
	"Operation Broken Fang #2",
	"Operation Broken Fang #3",
	"Operation Broken Fang #4",
	"2021 Service #1",
	"2021 Service #2",
	"2021 Service #3",
	"2021 Service #4",
	"2021 Service #5",
	"2021 Service #6",
	"Operation Riptide #1",
	"Operation Riptide #2",
	"Operation Riptide #3",
	"Operation Riptide #4",
	"2022 Service #1",
	"2022 Service #2",
	"2022 Service #3",
	"2022 Service #4",
	"2022 Service #5",
	"2022 Service #6",
	"2023 Service #1",
	"2023 Service #2",
	"2023 Service #3",
	"2023 Service #4",
	"2023 Service #5",
	"2023 Service #6",
	"BLAST.tv Paris 2023 CS:GO Bronze",
	"BLAST.tv Paris 2023 CS:GO Silver",
	"BLAST.tv Paris 2023 CS:GO Gold",
	"BLAST.tv Paris 2023 CS:GO Crystal",
	"Guardian 2",
	"Bravo",
	"Baggage",
	"Phoenix",
	"Office",
	"Cobblestone",
	"Overpass",
	"Bloodhound",
	"Cache",
	"Valeria",
	"Chroma",
	"Guardian 3",
	"Canals",
	"Welcome to the Clutch",
	"Death Sentence",
	"Inferno",
	"Hydra",
	"Easy Peasy",
	"Aces High",
	"Hydra",
	"Howl",
	"Brigadier General",
	"Alyx",
	"Dust II",
	"Guardian Elite",
	"Mirage",
	"Inferno",
	"Italy",
	"Victory",
	"Militia",
	"Nuke",
	"Train",
	"Guardian"
};

constexpr int medalIDs[] = {
	0,
	874,
	875,
	876,
	877,
	878,
	879,
	880,
	881,
	882,
	883,
	884,
	885,
	886,
	887,
	888,
	889,
	890,
	891,
	892,
	893,
	894,
	895,
	896,
	897,
	898,
	899,
	900,
	901,
	902,
	903,
	904,
	905,
	906,
	907,
	908,
	909,
	910,
	911,
	912,
	913,
	914,
	915,
	916,
	917,
	918,
	919,
	920,
	921,
	922,
	923,
	924,
	925,
	926,
	927,
	928,
	929,
	930,
	931,
	932,
	933,
	934,
	935,
	936,
	937,
	938,
	939,
	940,
	941,
	942,
	943,
	944,
	945,
	946,
	947,
	948,
	949,
	950,
	951,
	952,
	953,
	954,
	955,
	956,
	957,
	958,
	959,
	960,
	961,
	962,
	963,
	964,
	965,
	966,
	967,
	968,
	969,
	970,
	971,
	972,
	973,
	974,
	975,
	976,
	977,
	978,
	979,
	980,
	981,
	982,
	983,
	984,
	985,
	986,
	987,
	992,
	993,
	994,
	995,
	1001,
	1002,
	1003,
	1013,
	1014,
	1015,
	1024,
	1025,
	1026,
	1028,
	1029,
	1030,
	1316,
	1317,
	1318,
	1327,
	1328,
	1329,
	1331,
	1332,
	1336,
	1337,
	1338,
	1339,
	1340,
	1341,
	1342,
	1343,
	1344,
	1357,
	1358,
	1359,
	1360,
	1361,
	1362,
	1363,
	1367,
	1368,
	1369,
	1370,
	1371,
	1372,
	1376,
	1377,
	1378,
	1379,
	1380,
	1381,
	4353,
	4354,
	4355,
	4356,
	4550,
	4551,
	4552,
	4553,
	4674,
	4675,
	4676,
	4677,
	4678,
	4679,
	4682,
	4683,
	4684,
	4685,
	4686,
	4687,
	4688,
	4689,
	4690,
	4691,
	4692,
	4700,
	4701,
	4702,
	4703,
	4737,
	4738,
	4739,
	4740,
	4741,
	4742,
	4759,
	4760,
	4761,
	4762,
	4819,
	4820,
	4821,
	4822,
	4823,
	4824,
	4873,
	4874,
	4875,
	4876,
	4877,
	4878,
	4884,
	4885,
	4886,
	4887,
	6012,
	6013,
	6014,
	6015,
	6016,
	6017,
	6018,
	6019,
	6020,
	6021,
	6022,
	6023,
	6024,
	6025,
	6026,
	6027,
	6028,
	6029,
	6030,
	6031,
	6032,
	6033,
	6034,
	6101,
	6102,
	6103,
	6104,
	6105,
	6106,
	6107,
	6108,
	6109,
	6110
};

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
		font = io.Fonts->AddFontFromMemoryTTF((void*)resource::ubuntuFont, sizeof(resource::ubuntuFont), 13.0f, &cfgFont, Helpers::getFontGlyphRanges());
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
	guiSettings.SeparatorTextAlign.x = 0.5f;
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
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize );
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
	ImGui::Text("MaxEntity: %.d", maxEntity);
	ImGui::Text("HighestEntityIndex: %d", highestEntityIndex);
	ImGui::Text("LocalPlayerIndex: %d", localPlayerIndex);
	ImGui::Text("CrosshairID: %d", localPlayer->crosshairID());
	ImGui::SeparatorText("GameRules Test");
	const auto& m_fRoundStartTime = mem.Read<float>(IGameRules.address + Offset::netvars::m_fRoundStartTime);
	const auto& m_fMatchStartTime = mem.Read<float>(IGameRules.address + Offset::netvars::m_fMatchStartTime);
	const float roundTime = globalVars->currentTime - m_fRoundStartTime;
	const float matchTime = globalVars->currentTime - m_fMatchStartTime;
	ImGui::Text("m_fRoundStartTime: %.2f", m_fRoundStartTime);
	ImGui::Text("m_fMatchStartTime: %.2f", m_fMatchStartTime);
	ImGui::Text("Round Time: %.2f", roundTime);
	ImGui::Text("Match Time: %.2f", matchTime);
	ImGui::PushID("Roll");
	static float roll = 0.f;
	static float tempRoll = 0.f;
	ImGui::SetNextItemWidth(128);
	ImGui::SliderFloat("", &roll, -45.f, 45.f, "Roll: %.1f");
	if (roll != tempRoll) {
		tempRoll = roll;
		const auto& viewAngles = mem.Read<ImVec2>(IClientState.address + Offset::signatures::dwClientState_ViewAngles);
		mem.Write<Vector>(IClientState.address + Offset::signatures::dwClientState_ViewAngles, { viewAngles.x, viewAngles.y, roll });
	}
	ImGui::PopID();
	ImGui::Checkbox("Bool Debug 0", &cfg->debug.boolDebug0);
	ImGui::hotkey("Key Debug 0", cfg->debug.keyDebug0);
	ImGui::InputFloat("FloatDebug 0", &cfg->debug.floatDebug0);
	ImGui::GetIO().FontGlobalScale = cfg->debug.floatDebug0;
	static std::string cmd = "";
	ImGui::InputText("convar", &cmd);
	ImGui::SameLine();
	if (ImGui::Button("Send"))
		clientCmd(cmd);
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
	constexpr int flags = ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchSame;
	constexpr int flagsColumn = ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize;
	if (ImGui::BeginTable("Players List", 8, flags))
	{
		ImGui::TableSetupColumn("ID", flagsColumn);
		ImGui::TableSetupColumn("Name", flagsColumn);
		ImGui::TableSetupColumn("Health", flagsColumn);
		ImGui::TableSetupColumn("Armor", flagsColumn);
		ImGui::TableSetupColumn("Money", flagsColumn);
		ImGui::TableSetupColumn("Weapon", flagsColumn);
		ImGui::TableSetupColumn("Rank", flagsColumn);
		ImGui::TableSetupColumn("More", flagsColumn);
		ImGui::TableHeadersRow();

		std::vector<std::reference_wrapper<const PlayerData>> playersOrdered{ gameData.playerData.begin(), gameData.playerData.end()};
		std::ranges::sort(playersOrdered, [](const PlayerData& a, const PlayerData& b) {
			switch (cfg->m.playerList.sort) {
				default:
				case 0: // By Index asc
					return a.idx < b.idx;
				case 1: // by Index desc
					return a.idx > b.idx;
				case 2: // by Team (Teammates First)
					return a.teamNumber < b.teamNumber;
				case 3: // by Team (Enemies First)
					return a.teamNumber > b.teamNumber;
				case 4: // by Rank asc
					return gameData.playerResource.competitiveRanking[a.idx] < gameData.playerResource.competitiveRanking[b.idx];
				case 5: // by Rank desc
					return gameData.playerResource.competitiveRanking[a.idx] > gameData.playerResource.competitiveRanking[b.idx];
			}
		});

		for (const PlayerData& player : playersOrdered)
		{
			const auto& lpColor = cfg->m.playerList.localPlayerColor.color;
			auto teamColor = localPlayer.get() == (uintptr_t)player.entity ? ImVec4{lpColor[0], lpColor[1], lpColor[2], lpColor[3]} : player.teamNumber == 2 ? ImVec4{ 0.92f, 0.82f, .54f, 1.f } : ImVec4{0.26f, 0.59f, 0.98f, 1.f};
			const auto hpColor = player.health < 50 ? player.health < 25 ? ImVec4{ 1.f, .0f, .0f, 1.f } : ImVec4{ 1.f, 1.f, .0f, 1.f } : ImVec4{ 0.f, 1.f, .0f, 1.f };

			if (cfg->m.playerList.hideLocalPlayer && (player.idx == localPlayerIndex))
				continue;

			if (cfg->m.playerList.hideDormant && player.dormant)
				continue;

			if (player.dormant)
				teamColor.w = 0.5f;

			if (player.isHLTV)
				continue;

			ImGui::TableNextRow();
			ImGui::PushID(ImGui::TableGetRowIndex());
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.idx);
			ImGui::TableNextColumn();
			ImGui::TextColored(teamColor, "%s", player.name.c_str());
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", player.steamID.c_str());
			
			ImGui::TableNextColumn();
			ImGui::TextColored(hpColor, "%s", player.health < 1 ? "DEAD" : std::to_string(player.health).c_str());
			ImGui::TableNextColumn();
			ImGui::Text("%i%s", player.armor, player.hasHelmet ? "+H" : "");
			ImGui::TableNextColumn();
			ImGui::Text("$%i", player.money);
			ImGui::TableNextColumn();
			ImGui::Text("%s", Skin::getWeaponIDName(player.weaponID));
			ImGui::TableNextColumn();
			ImGui::Image(ranksTextures[gameData.playerResource.competitiveRanking[player.idx]].getTexture(), { 2.45f /* -> proportion 49x20px */ * ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() });
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::PushFont(nullptr);
				ImGui::TextUnformatted(std::to_string(gameData.playerResource.competitiveWins[player.idx]).append(" wins").c_str());
				ImGui::PopFont(); 
				ImGui::EndTooltip();
			}
			if (ImGui::TableNextColumn()) {
				if (ImGui::Button("...", { 24.f, 16.f }))
					ImGui::OpenPopup("plMore");
			}
			if (ImGui::BeginPopup("plMore")) {
				ImGui::Text("Name: %s", player.name.c_str());
				ImGui::Text("Clantag: %s", gameData.playerResource.clanTag[player.idx].c_str());
				ImGui::Text("Ping: %d", gameData.playerResource.ping[player.idx]);
				ImGui::Text("Kills: %d", gameData.playerResource.kills[player.idx]);
				ImGui::Text("Assists: %d", gameData.playerResource.assists[player.idx]);
				ImGui::Text("Deaths: %d", gameData.playerResource.deaths[player.idx]);
				float KD = static_cast<float>(gameData.playerResource.kills[player.idx]);
				if (gameData.playerResource.deaths[player.idx] > 1)
					KD = static_cast<float>(gameData.playerResource.kills[player.idx]) / static_cast<float>(gameData.playerResource.deaths[player.idx]);
				ImGui::Text("K/D: %.2f", KD);

				ImGui::Text("MVP: %d", gameData.playerResource.MVPs[player.idx]);
				ImGui::Text("Score: %d", gameData.playerResource.score[player.idx]);
				ImGui::BeginDisabled(cfg->restrictions);
				if (ImGui::Button("Set Resources", { 100.f, 16.f }))
					ImGui::OpenPopup("setPlayerResources");
				ImGui::EndDisabled();

				if (ImGui::BeginPopup("setPlayerResources")) {
					static int wins = 0;
					static int rank = 0;
					static int kills = 0;
					static int assists = 0;
					static int deaths = 0;
					static int ping = 0;
					static int level = 0;
					static int coin = 0;
					static int color = 0;
					static int musicID = 0;
					static int MVPs = 0;
					static int score = 0;
					static std::string clantag = "azurre";
					ImGui::PushID(32);
					ImGui::InputInt("Wins", &wins, 1, 10);
					ImGui::Combo("Rank", &rank, ranks, ARRAYSIZE(ranks));
					ImGui::InputInt("Kills", &kills, 1, 10);
					ImGui::InputInt("Assists", &assists, 1, 10);
					ImGui::InputInt("Deaths", &deaths, 1, 10);
					ImGui::InputInt("Ping", &ping, 1, 10);
					ImGui::InputInt("Level", &level, 1, 10);
					ImGui::Combo("Medals", &coin, medalNames, ARRAYSIZE(medalNames));
					ImGui::Combo("Color", &color, colorTeam, ARRAYSIZE(colorTeam));
					ImGui::InputInt("Music", &musicID, 1, 10);
					ImGui::InputInt("MVP", &MVPs, 1, 10);
					ImGui::InputInt("Score", &score, 1, 10);
					ImGui::InputText("Clantag", &clantag);
					ImGui::PopID();

					if (ImGui::Button("Override")) {
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveWins + 0x4 + player.idx * 4, wins);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iCompetitiveRanking + 0x4 + player.idx * 4, rank);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iKills + 0x4 + player.idx * 4, kills);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iAssists + 0x4 + player.idx * 4, assists);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iDeaths + 0x4 + player.idx * 4, deaths);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iPing + 0x4 + player.idx * 4, ping);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_nPersonaDataPublicLevel + 0x4 + player.idx * 4, level);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_nActiveCoinRank + 0x4 + player.idx * 4, medalIDs[coin]);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iCompTeammateColor + 0x4 + player.idx * 4, color - 2);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_nMusicID + 0x4 + player.idx * 4, musicID);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iMVPs + 0x4 + player.idx * 4, MVPs);
						mem.Write<int>(IPlayerResource.address + Offset::netvars::m_iScore + 0x4 + player.idx * 4, score);
						WriteProcessMemory(mem.processHandle, reinterpret_cast<void*>(IPlayerResource.address + Offset::netvars::m_szClan + 0x10 + player.idx * 0x10), clantag.c_str(), 16, NULL);
					}
					ImGui::EndPopup();
				}
				
				ImGui::EndPopup();
			}
			ImGui::PopID();

		}
		ImGui::EndTable();
	}
	ImGui::End();
}

void childLabel(const char* text) {
	ImGui::BeginChild(std::string(text).append(" Label").c_str(), {232.f , 20.f}, false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::SeparatorText(text);
	ImGui::EndChild();
}

void renderAimbotWindow() noexcept {
	ImGui::PushID("Aimbot");
	ImGui::BeginChild("Aimbot", {232.f , 266.f}, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	{
		ImGui::PushID("key");
		ImGui::Checkbox("Enabled", &cfg->a.enabled);
		ImGui::SameLine();
		ImGui::hotkey("", cfg->a.hotkey);
		ImGui::PopID();
		ImGui::Checkbox("Auto Shot", &cfg->a.autoShot);
		ImGui::BeginDisabled(cfg->restrictions);
		ImGui::EndDisabled();
		ImGui::Checkbox("Auto Stop", &cfg->a.autoStop);
		ImGui::Checkbox("Disable While Spectated", &cfg->a.disableWhileBeingSpectated);
		ImGui::Checkbox("Visible Only", &cfg->a.visibleOnly);
		ImGui::Checkbox("Friendly Fire", &cfg->a.friendlyFire);
		ImGui::PushItemWidth(128.f);
		ImGui::Combo("Bone", &cfg->a.bone, "Head\0Neck\0Sternum\0Chest\0Stomach\0Pelvis\0");
		ImGui::SliderFloat("##fov", &cfg->a.fov, 0.001f, 255.000f, "Fov: %.2f");
		ImGui::SliderFloat("##smooth", &cfg->a.smooth, 1.00f, 100.00f, "Smooth: %.2f");
		ImGui::PopItemWidth();
		ImGui::BeginDisabled(cfg->restrictions);
		ImGui::Checkbox("RCS", &cfg->a.rcs);
		ImGui::EndDisabled();
		ImGuiCustom::colorPicker("Draw fov", cfg->a.drawFov);
	}
	ImGui::EndChild();
	ImGui::PopID();
}

void renderTriggerBotWindow() noexcept {
	ImGui::BeginChild("Trigger Bot", { 232.f , 105.f }, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	{
		ImGui::PushID("Key");
		ImGui::Checkbox("Enabled", &cfg->t.enabled);
		ImGui::SameLine();
		ImGui::hotkey("", cfg->t.hotkey);
		ImGui::PopID();
		ImGui::Checkbox("Friendly Fire", &cfg->t.friendlyFire);
		ImGui::SetNextItemWidth(200.0f);
		ImGui::PushItemWidth(128.f);
		ImGui::SliderInt("Delay", &cfg->t.delay, 0, 1000);
		ImGui::SliderInt("Burst", &cfg->t.burst, 0, 1000);
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
}

void renderGlowWindow() noexcept {
	ImGui::BeginChild("Glow", { 232.f, 105.f }, true, ImGuiWindowFlags_NoTitleBar);
	{
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
		ImGui::EndDisabled();
	}
	ImGui::EndChild();
}

void renderChamsWindow() noexcept {
	ImGui::BeginChild("Chams", { 232.f, 105.f }, true, ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::BeginDisabled(cfg->restrictions);
		ImGui::Checkbox("Enabled", &cfg->c.enabled);
		ImGuiCustom::colorPicker("Enemies", cfg->c.enemy);
		ImGuiCustom::colorPicker("Allies", cfg->c.ally);
		ImGui::SetNextItemWidth(200.0f);
		ImGui::SliderFloat("Brightness", &cfg->c.brightness, 0.1f, 1.f);
		ImGui::EndDisabled();
	}
	ImGui::EndChild();
}

void renderESPWindow() noexcept {
	ImGui::BeginChild("ESP", { 232.f,242.f }, true, ImGuiWindowFlags_NoTitleBar);
	{
		static int list = 0;
		static int spotted = 0;
		constexpr std::array categories{ "Allies", "Enemies Occluded", "Enemies Visible" };
		ImGui::Checkbox("Enabled", &cfg->esp.enabled);
		ImGui::Combo("##player", &list, "Allies\0Enemies\0Weapons\0Projectiles\0Others\0");
		switch (list) {
		default: case 0: case 1:
			if (list) ImGui::Combo("##players", &spotted, "Occluded\0Visible\0");
			else spotted = 0;
			ImGuiCustom::colorPicker("Name", cfg->esp.players[categories[list + spotted]].other.names.color.data(), nullptr, nullptr, nullptr, &cfg->esp.players[categories[list + spotted]].other.names.enabled);
			ImGuiCustom::colorPicker("Weapon", cfg->esp.players[categories[list + spotted]].weapons.color.data(), nullptr, nullptr, nullptr, &cfg->esp.players[categories[list + spotted]].weapons.enabled);
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
			ImGui::Checkbox("Head Boxes", &cfg->esp.players[categories[list + spotted]].headBox.enabled);
			ImGui::PushID("hboxes");
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				if (ImGui::Checkbox("Gradient Color", &cfg->esp.players[categories[list + spotted]].headBox.gradientColor)) {
					ImGuiCustom::colorPicker("Top Color", cfg->esp.players[categories[list + spotted]].headBox.grandientTop.color.data(), nullptr, nullptr, nullptr, nullptr);
					ImGuiCustom::colorPicker("Bottom Color", cfg->esp.players[categories[list + spotted]].headBox.grandientBottom.color.data(), nullptr, nullptr, nullptr, nullptr);
				}
				else
					ImGuiCustom::colorPicker("Solid Color", cfg->esp.players[categories[list + spotted]].headBox.solid.color.data(), nullptr, nullptr, nullptr, nullptr);
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
			break;

		case 2:
			ImGuiCustom::colorPicker("Name", cfg->esp.weapons["All"].other.names.color.data(), nullptr, nullptr, nullptr, &cfg->esp.weapons["All"].other.names.enabled);
			ImGui::Checkbox("Boxes", &cfg->esp.weapons["All"].box.enabled);
			ImGui::PushID("boxes weapons");
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				if (ImGui::Checkbox("Gradient Color", &cfg->esp.weapons["All"].box.gradientColor)) {
					ImGuiCustom::colorPicker("Top Color", cfg->esp.weapons["All"].box.grandientTop.color.data(), nullptr, nullptr, nullptr, nullptr);
					ImGuiCustom::colorPicker("Bottom Color", cfg->esp.weapons["All"].box.grandientBottom.color.data(), nullptr, nullptr, nullptr, nullptr);
				}
				else
					ImGuiCustom::colorPicker("Solid Color", cfg->esp.weapons["All"].box.solid.color.data(), nullptr, nullptr, nullptr, nullptr);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGuiCustom::colorPicker("Lines", cfg->esp.weapons["All"].other.lines.color.data(), nullptr, nullptr, nullptr, &cfg->esp.weapons["All"].other.lines.enabled);
			break;
		case 3:
			ImGuiCustom::colorPicker("Name", cfg->esp.projectiles["All"].names.color.data(), nullptr, nullptr, nullptr, &cfg->esp.projectiles["All"].names.enabled);
			ImGui::Checkbox("Boxes", &cfg->esp.projectiles["All"].box.enabled);
			ImGui::PushID("boxes projectiles");
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				if (ImGui::Checkbox("Gradient Color", &cfg->esp.projectiles["All"].box.gradientColor)) {
					ImGuiCustom::colorPicker("Top Color", cfg->esp.projectiles["All"].box.grandientTop.color.data(), nullptr, nullptr, nullptr, nullptr);
					ImGuiCustom::colorPicker("Bottom Color", cfg->esp.projectiles["All"].box.grandientBottom.color.data(), nullptr, nullptr, nullptr, nullptr);
				}
				else
					ImGuiCustom::colorPicker("Solid Color", cfg->esp.projectiles["All"].box.solid.color.data(), nullptr, nullptr, nullptr, nullptr);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			break;
		case 4:
			static int currentCategory = 0;
			constexpr std::array categories{ "C4", "Planted C4", "Defuse Kits"};
			ImGui::Combo("Others", &currentCategory, categories.data(), categories.size());

			ImGuiCustom::colorPicker("Name", cfg->esp.others[categories[currentCategory]].other.names.color.data(), nullptr, nullptr, nullptr, &cfg->esp.others[categories[currentCategory]].other.names.enabled);
			ImGui::SameLine();
			ImGui::Checkbox("Enable on CT site", &cfg->esp.onCT);
			ImGui::Checkbox("Boxes", &cfg->esp.others[categories[currentCategory]].box.enabled);
			ImGui::PushID("boxes weapons");
			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				if (ImGui::Checkbox("Gradient Color", &cfg->esp.others[categories[currentCategory]].box.gradientColor)) {
					ImGuiCustom::colorPicker("Top Color", cfg->esp.others[categories[currentCategory]].box.grandientTop.color.data(), nullptr, nullptr, nullptr, nullptr);
					ImGuiCustom::colorPicker("Bottom Color", cfg->esp.others[categories[currentCategory]].box.grandientBottom.color.data(), nullptr, nullptr, nullptr, nullptr);
				}
				else
					ImGuiCustom::colorPicker("Solid Color", cfg->esp.others[categories[currentCategory]].box.solid.color.data(), nullptr, nullptr, nullptr, nullptr);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGuiCustom::colorPicker("Lines", cfg->esp.others[categories[currentCategory]].other.lines.color.data(), nullptr, nullptr, nullptr, &cfg->esp.others[categories[currentCategory]].other.lines.enabled);
			break;

		}
	}
	ImGui::EndChild();
}

void renderFakeLagWindow() noexcept {
	ImGui::BeginChild("Fake Lag", { 232.f, 56.f }, true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	{
		ImGui::BeginDisabled(cfg->restrictions);
		ImGui::Combo("Mode", &cfg->m.fakeLag.type, "Static\0Adaptative\0Random\0");
		ImGui::SliderInt("##limit", &cfg->m.fakeLag.limit, 0, 16, "Limit: %d");
		ImGui::EndDisabled();
	}
	ImGui::EndChild();

}

void renderClanTagWindow() noexcept {
	ImGui::BeginChild("Clantag", { 232.f, 200.f }, true, ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::BeginDisabled(cfg->restrictions);
		ImGui::PushItemWidth(120.f);
		ImGui::Combo("ClanTag", &cfg->clanTag.mode, "None\0Azurre\0Clock\0Reverse\0Velocity\0Position\0HP\0\\n Overflow\0Custom\0");
		if (cfg->clanTag.mode == 8) {
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
		}
		ImGui::PopItemWidth();
		ImGui::EndDisabled();
	}
	ImGui::EndChild();

}

void renderMiscWindow() noexcept {
	ImGui::BeginChild("Miscellaneous", { 232.f, -1 }, true, ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::PushItemWidth(96.f);
		ImGuiCustom::classicHotkey("Menu Key", cfg->m.menuKey);
		ImGui::Checkbox("Bunny Hop", &cfg->m.bhop);
		ImGui::BeginDisabled(cfg->restrictions);
		ImGui::Checkbox("Fix Tablet Signal", &cfg->m.fixTablet);
		ImGui::Checkbox("Engine Radar", &cfg->m.radarHack);
		ImGui::Checkbox("Fast Stop", &cfg->m.autoStop);
		ImGui::Checkbox("Grenade Trajectory", &cfg->m.grenadeTrajectory);
		ImGui::EndDisabled();
		ImGuiCustom::colorPicker("Offscreen Enemies", cfg->m.offscreenEnemies.toggle.color.data(), nullptr, &cfg->m.offscreenEnemies.toggle.rainbow, &cfg->m.offscreenEnemies.toggle.rainbowSpeed, &cfg->m.offscreenEnemies.toggle.enabled);
		ImGui::SameLine();
		ImGui::PushID("Offscreen Enemies");
		if (ImGui::Button("..."))
			ImGui::OpenPopup("");

		if (ImGui::BeginPopup("")) {
			ImGui::SliderFloat("##size", &cfg->m.offscreenEnemies.size, 4.0f, 64.0f, "Size: %.1f");
			ImGui::SliderFloat("##radius", &cfg->m.offscreenEnemies.radius, 1.0f, 64.0f, "Radius: %.1f");
			ImGui::EndPopup();
		}
		ImGui::PopID();
		ImGui::PushID("hit");
		ImGui::InputText("Hit Sound", &cfg->m.hitSound);
		ImGui::Combo("Type", &cfg->m.hitMarker.type, "None\0Cross\0Cross Fading\0"); ImGui::SameLine(); ImGuiCustom::colorPicker("Hit Marker", cfg->m.hitMarker.color.color.data());
		ImGui::SameLine();
		ImGui::SliderFloat("##as", &cfg->m.hitMarker.time, 0.1f, 1.5f, "Time: %.2fs");
		ImGui::PopID();
		ImGui::PushID("kill");
		ImGui::InputText("Kill Sound", &cfg->m.killSound);
		ImGui::Combo("Type", &cfg->m.killMarker.type, "None\0Cross\0Cross Fading\0"); ImGui::SameLine(); ImGuiCustom::colorPicker("Kill Marker", cfg->m.killMarker.color.color.data());
		ImGui::SameLine();
		ImGui::SliderFloat("##as", &cfg->m.killMarker.time, 0.1f, 1.5f, "Time: %.2fs");
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
			ImGui::Combo("Color Schemes", &cfg->m.minimap.colorScheme, "CT/TT\0Engine Radar\0");
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
			ImGuiCustom::colorPicker("LocalPlayer Color", cfg->m.playerList.localPlayerColor.color.data(), &cfg->m.playerList.localPlayerColor.color[3]);
			ImGui::Checkbox("Hide Local Player", &cfg->m.playerList.hideLocalPlayer);
			ImGui::Checkbox("Hide Dormant Players", &cfg->m.playerList.hideDormant);
			ImGui::Combo("Sort", &cfg->m.playerList.sort, "By Index (Ascend)\0By Index (Descend)\0By Team (Teammates)\0By Team (Enemies)\0By Rank (Ascend)\0By Rank (Descend)\0");
			ImGui::EndPopup();
		}
		ImGui::PopID();
		ImGui::PushID("Indicators");
		ImGui::Checkbox("Indicators", &cfg->m.indicators.enabled);
		ImGui::SameLine();
		if (ImGui::Button("..."))
			ImGui::OpenPopup("");

		if (ImGui::BeginPopup("")) {
			ImGui::Checkbox("No Title Bar", &cfg->m.indicators.noTitleBar);
			ImGui::Checkbox("No Background", &cfg->m.indicators.noBackground);
			ImGui::SetNextItemWidth(80.0f);
			ImGuiCustom::multiCombo("Flags", cfg->m.indicators.bytewise, "Chocked Packets\0Height\0Velocity\0Slowdown\0Stamina\0");
			ImGui::EndPopup();
		}
		ImGui::PopID();
		ImGui::BeginDisabled(cfg->restrictions);
		if (ImGui::Button("Fake Prime")) {
			constexpr uint8_t patch[]{ 0x31, 0xC0, 0x40, 0xC3 };
			WriteProcessMemory(mem.processHandle, (LPVOID)(IClient.address + 0x62EDF0), patch, 4, 0);
		}
		ImGui::EndDisabled();
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
}

void renderVisualsWindow() noexcept {
	ImGui::BeginChild("Visuals", { 228.f, 250.f}, true, ImGuiWindowFlags_NoTitleBar);
	{
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
		ImGui::Checkbox("Disable Panorama Blur", &cfg->v.noPanoramaBlur);
		ImGui::PushItemWidth(96.f);
		ImGui::Combo("Skybox", &cfg->v.skybox, skyboxList.data(), skyboxList.size());
		ImGui::SliderInt("##flash", &cfg->v.flashReduction, 0, 100, "Flash reduction: %d%%");
		ImGui::SliderInt("##fov", &cfg->v.fov, 30, 150, "Fov: %d");
		ImGui::PopItemWidth();
		ImGui::EndDisabled();
	}
	ImGui::EndChild();
}

void renderChangerWindow() noexcept {
	ImGui::BeginChild("Skin Changer", { 232.f, 272.f }, true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	{
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
	}
	ImGui::EndChild();
}

void renderDiscordWindow() noexcept {
	ImGui::BeginChild("Discord", { 232.f,-1 }, true, ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::Checkbox("Enabled", &cfg->d.enabled);
		ImGui::Checkbox("Show Map", &cfg->d.showMap);
		ImGui::Checkbox("Show Match Time", &cfg->d.showMatchTime);
		ImGui::Combo("Details", &cfg->d.details, "Azurre!\0Nick\0Rank\0");
		ImGui::Combo("State", &cfg->d.state, "Only for chosen\0Stats\0Rank\0");
	}
	ImGui::EndChild();
}

void renderGUIWindow() noexcept {
	ImGui::BeginChild("GUI", { 232.f, 152.f }, true, ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::SetNextItemWidth(96.f);
		if (ImGui::Combo("Menu colors", &cfg->u.menuColors, "Azurre\0Genshi\0Emerald\0Bloddy Red\0Gold Mine\0Pandora\0Holy Light\0Deep Dark\0Visual Studio\0GoldSrc\0ImGui\0Neverlose\0Aimware\0Onetap\0Custom\0"))
			GUI::updateColors();
		
		ImGui::Checkbox("AntiAliasing", &cfg->u.antiAliasing);
		ImGui::Checkbox("Center Title", &cfg->u.centerTitle);
		ImGui::Checkbox("Frame Border", &cfg->u.frameBorder);
		ImGui::Checkbox("Round Border", &cfg->u.roundBorder);
		ImGui::Checkbox("Window Border", &cfg->u.windowBorder);
	}
	ImGui::EndChild();
}

void renderConfigWindow() noexcept {
	ImGui::BeginChild("Config", { 232.f, 232.f }, true, ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnOffset(1, 170.0f);

		static bool incrementalLoad = false;
		ImGui::Checkbox("Incremental Load", &incrementalLoad);

		ImGui::PushItemWidth(152.0f);

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

		if (ImGui::ListBox("##configs", &currentConfig, [](void* data, int idx, const char** out_text) {
			auto& vector = *static_cast<std::vector<std::string>*>(data);
			*out_text = vector[idx].c_str();
			return true;
			}, &configItems, configItems.size(), 8) && currentConfig != -1)
			buffer = configItems[currentConfig];

			ImGui::PushID(0);
			if (ImGui::InputTextWithHint("", "config name", &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
				if (currentConfig != -1)
					cfg->rename(currentConfig, buffer.c_str());
			}
			ImGui::PopID();
			ImGui::NextColumn();

			ImGui::PushItemWidth(100.0f);

			if (ImGui::Button("Folder", { 48.0f, 24.0f }))
				cfg->openConfigDir();

			if (ImGui::Button("Create", { 48.0f, 24.0f }))
				cfg->add(buffer.c_str());

			if (ImGui::Button("Reset", { 48.0f, 24.0f }))
				ImGui::OpenPopup("Config to reset");

			if (ImGui::BeginPopup("Config to reset")) {
				static constexpr const char* names[]{ "Whole", "Aimbot", "Chams", "Discord", "Glow", "Misc", "Changer", "TriggerBot", "Visuals" };
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
				if (ImGui::Button("Load", { 48.0f, 24.0f })) {
					cfg->load(currentConfig, incrementalLoad);
				}
				if (ImGui::Button("Save", { 48.0f, 24.0f }))
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
				if (ImGui::Button("Delete", { 48.0f, 24.0f }))
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
			ImGui::SetNextItemWidth(96.f);
			ImGui::Combo("Restrictions", &cfg->restrictions, "None\0Read Only");
	}
	ImGui::EndChild();
}

const static std::array<PNGTexture, 6> guiIcons{
	resource::aimbotIcon,
	resource::espIcon,
	resource::miscIcon,
	resource::visualsIcon,
	resource::skinsIcon,
	resource::configIcon
};

void GUI::RenderMainMenu() noexcept {

	static int category = -1;

	ImGui::SetNextWindowPos({ screenSize.x / 2 - 320, screenSize.y / 2 - 240 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 576.f, 512.f });
	ImGui::Begin(
		"Azurre External",
		&isRunning,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse
	);
	ImGui::Columns(2, "menu", false);
	ImGui::SetColumnOffset(1, 80.f);
	if (ImGui::ImageButton(guiIcons[0].getTexture(), {56.f, 56.f})) category = 0;
	if (ImGui::ImageButton(guiIcons[1].getTexture(), {56.f, 56.f})) category = 1;
	if (ImGui::ImageButton(guiIcons[2].getTexture(), {56.f, 56.f})) category = 2;
	if (ImGui::ImageButton(guiIcons[3].getTexture(), {56.f, 56.f})) category = 3;
	if (ImGui::ImageButton(guiIcons[4].getTexture(), {56.f, 56.f})) category = 4;
	if (ImGui::ImageButton(guiIcons[5].getTexture(), {56.f, 56.f})) category = 5;
	ImGui::NextColumn();
	switch (category) {
		default:
		case -1:
			break;
		case 0: // Aimbot | TriggerBot
			childLabel("Aimbot");
			ImGui::SameLine();
			childLabel("Trigger Bot");
			renderAimbotWindow();
			ImGui::SameLine();
			renderTriggerBotWindow();
			break;
		case 1: // ESP | Chams | Glow
			ImGui::BeginChild("ESPChild", { 232.f, -1.f }, false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
			childLabel("ESP");
			renderESPWindow();
			ImGui::EndChild();

			ImGui::SameLine();
			ImGui::BeginChild("ChamsGlow", { 232.f, -1.f }, false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
			childLabel("Chams");
			renderChamsWindow();
			childLabel("Glow");
			renderGlowWindow();
			ImGui::EndChild();
			break;
		case 2: // Misc | Discord | GUI
			ImGui::BeginChild("Misc", { 232.f, -1.f }, false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
			childLabel("Miscellaneous");
			renderMiscWindow();
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("MiscOther", { 232.f, -1.f }, false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
			childLabel("Fake Lag");
			renderFakeLagWindow();
			childLabel("ClanTag");
			renderClanTagWindow();
			childLabel("Discord");
			renderDiscordWindow();

			ImGui::EndChild();
			break;
		case 3: // Visuals
			childLabel("Visuals");
			renderVisualsWindow();
			break;
		case 4: // Skins
			childLabel("Changer");
			renderChangerWindow();
			break;
		case 5: // Config
			childLabel("Config");
			ImGui::SameLine();
			childLabel("GUI Style");
			renderConfigWindow();
			ImGui::SameLine();
			renderGUIWindow();
			break;
	}
	ImGui::Columns(1);
	ImGui::End();
}

void watermark() {
	const auto time = std::time(nullptr);
	const auto localTime = std::localtime(&time);
	char s[21];
	s[0] = '\0';
	snprintf(s, sizeof(s), " | %02d:%02d:%02d | ", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
#if _DEBUG
	constexpr const char* builtDate = __DATE__;
	constexpr const char* builtTime = __TIME__;
	const std::string watermark = std::string("Azurre | Built: ").append(builtDate).append(" ").append(builtTime).append(s).append("Hello xs9 :)");
#else
	static auto frameRate = 1.0f;
	frameRate = 0.9f * frameRate + 0.1f * globalVars->absoluteFrameTime;
	const int framePerSecond = frameRate != 0.0f ? static_cast<int>(1 / frameRate) : 0;
	const std::string watermark = std::string("Azurre | FPS: ").append(std::to_string(framePerSecond)).append(s).append("Hello xs9 :)");
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
	Aimbot::drawFov();
	Misc::hitMarkerSound();
	Misc::killMarkerSound();
	Misc::crosshairs();
	Misc::drawOffscreenEnemies();
	watermark();
#if defined(_DEBUG)
	//ImGui::GetBackgroundDrawList()->AddRect( //Draws Rectangle around csgo window
	//	gameScreenPos,
	//	gameScreenPosEnd,
	//	ImGui::GetColorU32({ 0.f, 0.4f, 1.f, 0.5f }),
	//	0, 0, 2.5f);
#endif
}