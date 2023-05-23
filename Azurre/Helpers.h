#pragma once

#include <numbers>
#include <array>
#include <vector>
#include <string>

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

	constexpr std::uint8_t utf8SeqLen(char firstByte) noexcept
	{
		return (firstByte & 0x80) == 0x00 ? 1 :
			(firstByte & 0xE0) == 0xC0 ? 2 :
			(firstByte & 0xF0) == 0xE0 ? 3 :
			(firstByte & 0xF8) == 0xF0 ? 4 :
			0;
	}
}