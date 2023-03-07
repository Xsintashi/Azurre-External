#include "Helpers.h"
#include "SDK/Vector.h"

Color3 Helpers::ConvertColors3(float in[3]) {
	Color3 out;
	out.r = static_cast<uint8_t>(in[0] * 255.f);
	out.g = static_cast<uint8_t>(in[1] * 255.f);
	out.b = static_cast<uint8_t>(in[2] * 255.f);

	return out;
}

Color4 Helpers::ConvertColors4(float in[4]) {
	Color4 out;
	out.r = static_cast<uint8_t>(in[0] * 255.f);
	out.g = static_cast<uint8_t>(in[1] * 255.f);
	out.b = static_cast<uint8_t>(in[2] * 255.f);
	out.a = static_cast<uint8_t>(in[3] * 255.f);

	return out;
}

Vector Helpers::calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept
{
	return ((destination - source).toAngle() - viewAngles).normalize();
}