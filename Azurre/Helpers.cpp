#include "Helpers.h"
#include "Core.h"
#include "Offsets.h"

#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/LocalPlayer.h"
#include "SDK/Vector.h"
#include "../../Config.h"

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