#pragma once

#include <array>

inline constexpr std::array skyboxList{ "Default", "cs_baggage_skybox_", "cs_tibet", "embassy", "italy", "jungle", "nukeblank", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night_flat", "sky_csgo_night02", "sky_day02_05_hdr", "sky_day02_05", "sky_dust", "sky_l4d_rural02_ldr", "sky_venice", "vertigo_hdr", "vertigo", "vertigoblue_hdr", "vietnam", "sky_lunacy", "sky_hr_aztec" };

namespace Misc {
	void bunnyHop() noexcept;
	void fakeLag() noexcept;
	void hitMarkerSound() noexcept;
	void changeWindowTitle(bool restore = false) noexcept;
	void forceReload(bool onKey = false) noexcept;
	void showKeybinds() noexcept;
	void modifyConVars(bool reset = false) noexcept;
	void bombTimer() noexcept;
	void fastStop() noexcept;
}