#pragma once

#include <numbers>

struct Vector;

struct Color3 {
	uint8_t r{}, g{}, b{};
};

struct Color4 {
	uint8_t r{}, g{}, b{}, a{};
};

namespace Helpers {
	constexpr auto deg2rad(float degrees) noexcept { return degrees * (std::numbers::pi_v<float> / 180.0f); }
	constexpr auto rad2deg(float radians) noexcept { return radians * (180.0f / std::numbers::pi_v<float>); }
	Color3 ConvertColors3(float in[3]);
	Color4 ConvertColors4(float in[4]);
	Vector calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept;
}