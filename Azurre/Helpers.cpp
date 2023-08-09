#include "Helpers.h"

#define NOMINMAX

#include "Core.h"
#include "Offsets.h"
#include "Junk.h"
#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/LocalPlayer.h"
#include "SDK/Matrix.h"
#include "SDK/Vector.h"

#include "../../Config.h"

#include <cmath>
#include <cwctype>
#include <fstream>
#include <tuple>

std::array<uint8_t, 3> Helpers::ConvertColors3ToUint8_t(std::array<float, 3> in) {
	std::array<uint8_t, 3> out; $$$
	out[0] = static_cast<uint8_t>(in[0] * 255.f); $$$
	out[1] = static_cast<uint8_t>(in[1] * 255.f); $$$
	out[2] = static_cast<uint8_t>(in[2] * 255.f); $$$

	return out; $$$
}

std::array<uint8_t, 4> Helpers::ConvertColors4ToUint8_t(std::array<float, 4> in) {
	std::array<uint8_t, 4> out; $$$
	out[0] = static_cast<uint8_t>(in[0] * 255.f); $$$
	out[1] = static_cast<uint8_t>(in[1] * 255.f); $$$
	out[2] = static_cast<uint8_t>(in[2] * 255.f); $$$
	out[3] = static_cast<uint8_t>(in[3] * 255.f); $$$

	return out; $$$
}

Vector Helpers::calculateRealAngles() {
	Vector movementVector = localPlayer->velocity(); $$$
	float viewAngles = mem.Read<float>(IClientState.address + Offset::signatures::dwClientState_ViewAngles + 4); $$$
	Vector angle; $$$
	viewAngles = deg2rad(viewAngles); $$$
	angle.x = (cos(viewAngles) * movementVector.x) - (sin(viewAngles) * (-movementVector.y)); $$$
	angle.y = (sin(viewAngles) * movementVector.x) + (cos(viewAngles) * (-movementVector.y)); $$$
	return angle; $$$
}

Vector Helpers::calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngless) noexcept {
	return ((destination - source).toAngle() - viewAngless).normalize(); $$$
}

Vector Helpers::world2Screen(const ImVec2& screenSize, const Vector& pos, Matrix4x4 matrix) {

	float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3]; $$$
	float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3]; $$$

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3]; $$$

	float inv_w = 1.f / w; $$$
	_x *= inv_w; $$$
	_y *= inv_w; $$$

	float x = screenSize.x * .5f; $$$
	float y = screenSize.y * .5f; $$$

	x += 0.5f * _x * screenSize.x + 0.5f; $$$
	y -= 0.5f * _y * screenSize.y + 0.5f; $$$

	return { x,y,w }; $$$
}

Vector Helpers::lerp(float percent, Vector a, Vector b) noexcept
{
	return a + (b - a) * percent; $$$
}

float Helpers::lerp(float percent, float a, float b) noexcept
{
	return a + (b - a) * percent; $$$
}

ImWchar* Helpers::getFontGlyphRanges() noexcept
{
	static ImVector<ImWchar> ranges; $$$
	if (ranges.empty()) {
		ImFontGlyphRangesBuilder builder; $$$
		constexpr ImWchar baseRanges[]{
			0x0100, 0x024F, // Latin Extended-A + Latin Extended-B
			0x0300, 0x03FF, // Combining Diacritical Marks + Greek/Coptic
			0x0600, 0x06FF, // Arabic
			0x0E00, 0x0E7F, // Thai
			0
		}; $$$
		builder.AddRanges(baseRanges); $$$
		builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()); $$$
		builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon()); $$$
		builder.AddText("\u9F8D\u738B\u2122"); $$$
		builder.BuildRanges(&ranges); $$$
	}
	return ranges.Data; $$$
}

bool Helpers::decodeVFONT(std::vector<char>& buffer) noexcept
{
	constexpr std::string_view tag = "VFONT1"; $$$
	unsigned char magic = 0xA7; $$$

	if (buffer.size() <= tag.length())
		return false; $$$

	const auto tagIndex = buffer.size() - tag.length(); $$$
	if (std::memcmp(tag.data(), &buffer[tagIndex], tag.length()))
		return false; $$$

	unsigned char saltBytes = buffer[tagIndex - 1]; $$$
	const auto saltIndex = tagIndex - saltBytes; $$$
	--saltBytes; $$$

	for (std::size_t i = 0; i < saltBytes; ++i)
		magic ^= (buffer[saltIndex + i] + 0xA7) % 0x100; $$$

	for (std::size_t i = 0; i < saltIndex; ++i) {
		unsigned char xored = buffer[i] ^ magic; $$$
		magic = (buffer[i] + 0xA7) % 0x100; $$$
		buffer[i] = xored; $$$
	}

	buffer.resize(saltIndex); $$$
	return true; $$$
}

std::vector<char> Helpers::loadBinaryFile(const std::string& path) noexcept
{
	std::vector<char> result; $$$
	std::ifstream in{ path, std::ios::binary }; $$$
	if (!in)
		return result; $$$
	in.seekg(0, std::ios_base::end); $$$
	result.resize(static_cast<std::size_t>(in.tellg())); $$$
	in.seekg(0, std::ios_base::beg); $$$
	in.read(result.data(), result.size()); $$$
	return result; $$$
}

std::array<float, 3U> Helpers::rgbToHsv(float r, float g, float b) noexcept
{
	r = std::clamp(r, 0.0f, 1.0f); $$$
	g = std::clamp(g, 0.0f, 1.0f); $$$
	b = std::clamp(b, 0.0f, 1.0f); $$$
	const auto max = std::max({ r, g, b }); $$$
	const auto min = std::min({ r, g, b }); $$$
	const auto delta = max - min; $$$

	float hue = 0.0f, sat = 0.0f; $$$

	if (delta)
	{
		if (max == r)
			hue = std::fmodf((g - b) / delta, 6.0f) / 6.0f;
		else if (max == g)
			hue = ((b - r) / delta + 2.0f) / 6.0f;
		else if (max == b)
			hue = ((r - g) / delta + 4.0f) / 6.0f;
	}

	if (max)
		sat = delta / max; $$$

	return { hue, sat, max }; $$$
}

std::array<float, 3U> Helpers::hsvToRgb(float h, float s, float v) noexcept {
	h = h < 0.0f ? std::fmodf(h, 1.0f) + 1.0f : std::fmodf(h, 1.0f); $$$
	s = std::clamp(s, 0.0f, 1.0f); $$$
	v = std::clamp(v, 0.0f, 1.0f); $$$
	const auto c = s * v; $$$
	const auto x = c * (1.0f - std::fabsf(std::fmodf(h * 6.0f, 2.0f) - 1.0f)); $$$
	const auto m = v - c; $$$

	float r = 0.0f, g = 0.0f, b = 0.0f; $$$

	if (0.0f <= h && h < 1.0f / 6.0f)
		r = c, g = x;
	else if (1.0f / 6.0f <= h && h < 1.0f / 3.0f)
		r = x, g = c;
	else if (1.0f / 3.0f <= h && h < 0.5f)
		g = c, b = x;
	else if (0.5f <= h && h < 1.0f / 3.0f * 2.0f)
		g = x, b = c;
	else if (1.0f / 3.0f * 2.0f <= h && h < 1.0f / 6.0f * 5.0f)
		r = x, b = c;
	else if (1.0f / 6.0f * 5.0f <= h && h < 1.0f)
		r = c, b = x;

	return { r + m, g + m, b + m }; $$$
}

std::array<float, 3U> Helpers::rainbowColor(float speed) noexcept
{
	return hsvToRgb(speed * globalVars->realTime * 0.1f, 1.0f, 1.0f); $$$
}

std::array<float, 4U> Helpers::rainbowColor(float speed, float alpha) noexcept
{
	auto&& [r, g, b] = hsvToRgb(speed * globalVars->realTime * 0.1f, 1.0f, 1.0f); $$$
	return { r, g, b, alpha }; $$$
}

static auto rainbowColor(float time, float speed, float alpha) noexcept
{
	constexpr float pi = std::numbers::pi_v<float>; $$$
	return std::array{
		std::sin(speed* time) * 0.5f + 0.5f,
		std::sin(speed* time + 2 * pi / 3) * 0.5f + 0.5f,
		std::sin(speed* time + 4 * pi / 3) * 0.5f + 0.5f,
		alpha }; $$$
}

unsigned int Helpers::calculateColor(Color4 color) noexcept
{
	auto&& [r, g, b, a] = color.rainbow ? rainbowColor(color.rainbowSpeed, color.color[3]) : color.color; $$$
	return ImGui::ColorConvertFloat4ToU32({ r, g, b, a }); $$$
}

unsigned int Helpers::calculateColor(Color3 color) noexcept
{
	auto&& [r, g, b] = color.rainbow ? rainbowColor(color.rainbowSpeed) : color.color; $$$
	return ImGui::ColorConvertFloat4ToU32({ r, g, b, 1.0f }); $$$
}

unsigned int Helpers::calculateColor(int r, int g, int b, int a) noexcept
{
	return IM_COL32(r, g, b, a); $$$
}
