#pragma once

#include <array>
#include <numbers>
#include <string>
#include <vector>


#include "../lib/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../lib/imgui/imgui_internal.h"

using uint8_tColor3 = std::array<uint8_t, 3>;
using uint8_tColor4 = std::array<uint8_t, 4>;

struct Matrix4x4;
struct ImVec2;
struct Vector;
struct Color3;
struct Color4;

namespace Helpers {
	constexpr auto deg2rad(float degrees) noexcept { return degrees * (std::numbers::pi_v<float> / 180.0f); }
	constexpr auto rad2deg(float radians) noexcept { return radians * (180.0f / std::numbers::pi_v<float>); }
	std::array<uint8_t, 3> ConvertColors3ToUint8_t(std::array<float, 3> in);
	std::array<uint8_t, 4> ConvertColors4ToUint8_t(std::array<float, 4> in);
	Vector calculateRealAngles();
	Vector calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept;
	Vector lerp(float percent, Vector a, Vector b) noexcept;
	float lerp(float percent, float a, float b) noexcept;

	ImWchar* getFontGlyphRanges() noexcept;

	Vector world2Screen(const ImVec2& screenSize, const Vector& pos, Matrix4x4 matrix);

	bool decodeVFONT(std::vector<char>& buffer) noexcept;
	std::vector<char> loadBinaryFile(const std::string& path) noexcept;

	std::array<float, 3U> rgbToHsv(float r, float g, float b) noexcept;
	std::array<float, 3U> hsvToRgb(float h, float s, float v) noexcept;

	std::array<float, 3U> rainbowColor(float speed) noexcept;
	std::array<float, 4U> rainbowColor(float speed, float alpha) noexcept;

	unsigned int calculateColor(Color4 color) noexcept;
	unsigned int calculateColor(Color3 color) noexcept;
	unsigned int calculateColor(int r, int g, int b, int a) noexcept;

	static constexpr std::array RanksShort = {
	"Unrank",
	"S1",
	"S2",
	"S3",
	"S4",
	"S5",
	"S6",
	"G1",
	"G2",
	"G3",
	"G4",
	"MG2",
	"MG2",
	"MGE",
	"DMG",
	"LE",
	"LEM",
	"Supreme",
	"Global"
	};

	static constexpr std::array Ranks = {
		"Unranked",
		"Silver I",
		"Silver II",
		"Silver III",
		"Silver IV",
		"Silver Elite",
		"Silver Elite Master",
		"Gold Nova I",
		"Gold Nova II",
		"Gold Nova III",
		"Gold Nova Master",
		"Master Guardian I",
		"Master Guardian II",
		"Master Guardian Elite",
		"Distinguished Master Guardian",
		"Legendary Eagle",
		"Legendary Eagle Master",
		"Supreme Master First Class",
		"The Global Elite"
	};

	constexpr static const char* convertRankFromNumber(bool shorten, int rank) noexcept {
		if (rank > 18) rank = 18;
		if (rank < 0) rank = 0;
		if (shorten)
			return RanksShort[rank];
		else
			return Ranks[rank];
	}

	constexpr bool isFriendly(__int64 steamID, std::vector<__int64> list) {
		for (const auto& i : list) {
			if (steamID == i)
				return true;
		}
		return false;
	}

	constexpr std::uint8_t utf8SeqLen(char firstByte) noexcept
	{
		return (firstByte & 0x80) == 0x00 ? 1 :
			(firstByte & 0xE0) == 0xC0 ? 2 :
			(firstByte & 0xF0) == 0xE0 ? 3 :
			(firstByte & 0xF8) == 0xF0 ? 4 :
			0;
	}

	static bool getByteFromBytewise(int input, int byte) {
		return (input & 1 << byte) == 1 << byte;
	}
}