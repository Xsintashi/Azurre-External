#include "Aimbot.h"

#include "../GUI.h"
#include "../Offsets.h"
#include "../Helpers.h"
#include "../Config.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"
#include "../../imgui/imgui.h"
#include <chrono>

void Aimbot::run() noexcept {
	while (GUI::isRunning) {
		if (!cfg->a.enabled) continue;

		if (!localPlayer || localPlayer->isDead() || localPlayer->isDefusing() || localPlayer->waitForNoAttack()) continue;

		const auto eyePosition = localPlayer->origin() + localPlayer->viewOffset();
		const auto& viewAngles = csgo.Read<ImVec2>(IClientState + Offset::signatures::dwClientState_ViewAngles);

		const auto& aimPunch = localPlayer->aimPunch() * 2.f;

		float bestFov = cfg->a.fov;
		Vector bestAngle = {};

		int weaponIndex = csgo.Read<int>(localPlayer.get() + Offset::netvars::m_hActiveWeapon) & 0xFFF;

		if (!weaponIndex) continue;

		const auto& activeWeapon = getEntity(weaponIndex - 1);

		if (!activeWeapon || activeWeapon->clip() < 1)
			continue;

		for (auto i = 1; i <= 32; ++i)
		{
			const auto entity = getEntity(i);

			if (!entity)
				continue;

			if (entity->isDead() || entity->dormant() || !entity->spottedByMask())
				continue;

			if (entity->teamNumber() == localPlayer->teamNumber())
				continue;

			for (auto bone : { 8, 4, 3, 7, 6, 5 }) {
				const auto bonePosition = 8 - cfg->a.bone;

				const auto bonePos = Vector{
					csgo.Read<float>(entity->boneMatrix() + 0x30 * bonePosition + 0x0C),
					csgo.Read<float>(entity->boneMatrix() + 0x30 * bonePosition + 0x1C),
					csgo.Read<float>(entity->boneMatrix() + 0x30 * bonePosition + 0x2C)
				};

				const auto angle = Helpers::calculateRelativeAngle(eyePosition, bonePos, { viewAngles.x + aimPunch.x, viewAngles.y + aimPunch.y, 0.f });

				const auto fov = std::hypot(angle.x, angle.y);

				if (fov < bestFov) {
					bestFov = fov;
					bestAngle = angle;
				}
			}
			if (bestAngle.notNull()) {
				csgo.Write<Vector>(IClientState + Offset::signatures::dwClientState_ViewAngles, Vector{ viewAngles.x + bestAngle.x / cfg->a.smooth, viewAngles.y + bestAngle.y / cfg->a.smooth, 0.f } );
				if (cfg->a.autoShot) {
					csgo.Write<std::uintptr_t>(IClient + Offset::signatures::dwForceAttack, 6);
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					csgo.Write<std::uintptr_t>(IClient + Offset::signatures::dwForceAttack, 4);
				}
			}
		}
	}
}

void Aimbot::recoilSystem() noexcept {

	if (!cfg->a.rcs) return;

	if (!localPlayer) return;

	if (localPlayer->isDead()) return;

	static ImVec2 oldAimPunch = {};

	const auto& viewAngles = csgo.Read<ImVec2>(IClientState + Offset::signatures::dwClientState_ViewAngles);

	if (localPlayer->shotsFired()) {


		auto newAngles = ImVec2{
			viewAngles.x + oldAimPunch.x - localPlayer->aimPunch().x * 2.f,
			viewAngles.y + oldAimPunch.y - localPlayer->aimPunch().y * 2.f,
		};

		if (newAngles.x > 89.f)
			newAngles.x = 89.f;

		if (newAngles.y < -89.f)
			newAngles.y = -89.f;

		while (newAngles.y > 180.f)
			newAngles.y -= 360.f;

		while (newAngles.y < -180.f)
			newAngles.y += 360.f;

		csgo.Write<ImVec2>(IClientState + Offset::signatures::dwClientState_ViewAngles, newAngles);

		oldAimPunch.x = localPlayer->aimPunch().x * 2.f;
		oldAimPunch.y = localPlayer->aimPunch().y * 2.f;

	} else oldAimPunch = {};

}