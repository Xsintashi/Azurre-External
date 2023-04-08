#pragma once

#include <numbers>
#include <array>

using uint8_tColor3 = std::array<uint8_t, 3>;
using uint8_tColor4 = std::array<uint8_t, 4>;

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

	constexpr std::uint8_t utf8SeqLen(char firstByte) noexcept
	{
		return (firstByte & 0x80) == 0x00 ? 1 :
			(firstByte & 0xE0) == 0xC0 ? 2 :
			(firstByte & 0xF0) == 0xE0 ? 3 :
			(firstByte & 0xF8) == 0xF0 ? 4 :
			0;
	}
}