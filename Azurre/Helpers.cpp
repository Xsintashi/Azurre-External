#include "Helpers.h"
#include "Core.h"
#include "Offsets.h"

#include "SDK/Entity.h"
#include "SDK/GlobalVars.h"
#include "SDK/LocalPlayer.h"
#include "SDK/Matrix.h"
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
