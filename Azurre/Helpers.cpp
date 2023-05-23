#include "Helpers.h"
#include "Core.h"
#include "Offsets.h"

#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/LocalPlayer.h"
#include "SDK/Matrix.h"
#include "SDK/Vector.h"

#include "../../Config.h"

#include <fstream>

std::array<uint8_t, 3> Helpers::ConvertColors3ToUint8_t(std::array<float, 3> in) {
	std::array<uint8_t, 3> out;
	out[0] = static_cast<uint8_t>(in[0] * 255.f);
	out[1] = static_cast<uint8_t>(in[1] * 255.f);
	out[2] = static_cast<uint8_t>(in[2] * 255.f);

	return out;
}

std::array<uint8_t, 4> Helpers::ConvertColors4ToUint8_t(std::array<float, 4> in) {
	std::array<uint8_t, 4> out;
	out[0] = static_cast<uint8_t>(in[0] * 255.f);
	out[1] = static_cast<uint8_t>(in[1] * 255.f);
	out[2] = static_cast<uint8_t>(in[2] * 255.f);
	out[3] = static_cast<uint8_t>(in[3] * 255.f);

	return out;
}

Vector Helpers::calculateRealAngles() {
	Vector movementVector = localPlayer->velocity();
	float viewAngles = csgo.Read<float>(IClientState.address + Offset::signatures::dwClientState_ViewAngles + 4);
	Vector angle;
	viewAngles = deg2rad(viewAngles);
	angle.x = (cos(viewAngles) * movementVector.x) - (sin(viewAngles) * (-movementVector.y));
	angle.y = (sin(viewAngles) * movementVector.x) + (cos(viewAngles) * (-movementVector.y));
	return angle;
}

Vector Helpers::calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngless) noexcept {
	return ((destination - source).toAngle() - viewAngless).normalize();
}

Vector Helpers::world2Screen(const ImVec2& screenSize, const Vector& pos, Matrix4x4 matrix) {

	float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = screenSize.x * .5f;
	float y = screenSize.y * .5f;

	x += 0.5f * _x * screenSize.x + 0.5f;
	y -= 0.5f * _y * screenSize.y + 0.5f;

	return { x,y,w };
}

Vector Helpers::lerp(float percent, Vector a, Vector b) noexcept
{
	return a + (b - a) * percent;
}

float Helpers::lerp(float percent, float a, float b) noexcept
{
	return a + (b - a) * percent;
}

ImWchar* Helpers::getFontGlyphRanges() noexcept
{
	static ImVector<ImWchar> ranges;
	if (ranges.empty()) {
		ImFontGlyphRangesBuilder builder;
		constexpr ImWchar baseRanges[]{
			0x0100, 0x024F, // Latin Extended-A + Latin Extended-B
			0x0300, 0x03FF, // Combining Diacritical Marks + Greek/Coptic
			0x0600, 0x06FF, // Arabic
			0x0E00, 0x0E7F, // Thai
			0
		};
		builder.AddRanges(baseRanges);
		builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
		builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
		builder.AddText("\u9F8D\u738B\u2122");
		builder.BuildRanges(&ranges);
	}
	return ranges.Data;
}

bool Helpers::decodeVFONT(std::vector<char>& buffer) noexcept
{
	constexpr std::string_view tag = "VFONT1";
	unsigned char magic = 0xA7;

	if (buffer.size() <= tag.length())
		return false;

	const auto tagIndex = buffer.size() - tag.length();
	if (std::memcmp(tag.data(), &buffer[tagIndex], tag.length()))
		return false;

	unsigned char saltBytes = buffer[tagIndex - 1];
	const auto saltIndex = tagIndex - saltBytes;
	--saltBytes;

	for (std::size_t i = 0; i < saltBytes; ++i)
		magic ^= (buffer[saltIndex + i] + 0xA7) % 0x100;

	for (std::size_t i = 0; i < saltIndex; ++i) {
		unsigned char xored = buffer[i] ^ magic;
		magic = (buffer[i] + 0xA7) % 0x100;
		buffer[i] = xored;
	}

	buffer.resize(saltIndex);
	return true;
}

std::vector<char> Helpers::loadBinaryFile(const std::string& path) noexcept
{
	std::vector<char> result;
	std::ifstream in{ path, std::ios::binary };
	if (!in)
		return result;
	in.seekg(0, std::ios_base::end);
	result.resize(static_cast<std::size_t>(in.tellg()));
	in.seekg(0, std::ios_base::beg);
	in.read(result.data(), result.size());
	return result;
}