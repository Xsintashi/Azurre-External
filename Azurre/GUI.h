#pragma once
#include <d3d9.h>

namespace GUI {
	// when this changes, exit threads
	// and close menu :)
	inline bool isRunning = true;

	// winapi window vars
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };

	// window overlay
	inline RECT overlayRect;
	inline RECT marginRect;
	inline int resX;
	inline int resY;

	// points for window movement
	inline POINTS position = { };

	// direct x state vars
	inline PDIRECT3D9 d3d = 0;
	inline LPDIRECT3DDEVICE9 device = 0;
	inline D3DPRESENT_PARAMETERS presentParameters = { };

	// handle window creation & destruction
	void CreateHWindow(const char* windowName) noexcept;
	void CreateHWindow(const wchar_t* windowName) noexcept;
	void DestroyHWindow() noexcept;

	// handle device creation & destruction
	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	// handle ImGui creation & destruction
	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;

	void updateColors() noexcept;

	void update() noexcept;

	void loadWindow() noexcept;

	// Windows
	void RenderDebugWindow() noexcept;
	void RenderPlayerList() noexcept;
	void RenderMainMenu() noexcept;
	void overlay() noexcept;
}

struct vertex {
	float x, y, z, rhw;
	D3DCOLOR color;
};