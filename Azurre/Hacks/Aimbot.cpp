#include "Aimbot.h"

#include "../GUI.h"
#include "../Offsets.h"
#include "../Helpers.h"
#include "../Config.h"

#include "../SDK/LocalPlayer.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../Lib/imgui/imgui.h"

#include <chrono>

void Aimbot::run() noexcept {
	while (GUI::isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		if (!cfg->a.enabled) continue;

		if (!localPlayer || localPlayer->isDead() || localPlayer->isDefusing() || localPlayer->waitForNoAttack()) continue;

		const auto eyePosition = localPlayer->origin() + localPlayer->viewOffset();
		const auto& viewAngles = mem.Read<ImVec2>(IClientState.address + Offset::signatures::dwClientState_ViewAngles);

		if (!gameData.observerData.empty() && cfg->a.disableWhileBeingSpectated)
			continue;

		const auto& aimPunch = localPlayer->aimPunch() * 2.f;

		float bestFov = cfg->a.fov;
		Vector bestAngle = {};

		int weaponIndex = mem.Read<int>(localPlayer.get() + Offset::netvars::m_hActiveWeapon) & ENT_ENTRY_MASK;

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

			if (!cfg->a.friendlyFire && entity->isSameTeam())
				continue;

			for (auto bone : { 8, 4, 3, 7, 6, 5 }) {
				const auto bonePosition = 8 - cfg->a.bone;

				const auto bonePos = Vector{
					mem.Read<float>(entity->boneMatrix() + 0x30 * bonePosition + 0x0C),
					mem.Read<float>(entity->boneMatrix() + 0x30 * bonePosition + 0x1C),
					mem.Read<float>(entity->boneMatrix() + 0x30 * bonePosition + 0x2C)
				};

				const auto angle = Helpers::calculateRelativeAngle(eyePosition, bonePos, { viewAngles.x + aimPunch.x, viewAngles.y + aimPunch.y, 0.f });

				const auto fov = std::hypot(angle.x, angle.y);

				if (fov < bestFov) {
					bestFov = fov;
					bestAngle = angle;
				}
			}

			float x = (-bestAngle.y * 2 / (cfg->a.smooth * bestFov / 2));
			float y = (bestAngle.x * 2 / (cfg->a.smooth * bestFov / 2));

			if (bestAngle.notNull() && cfg->a.hotkey.isActive()) {
				if (cfg->restrictions && !showMenu && bestFov > 0.2f)
					mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(10 * x), static_cast<DWORD>(10 * y), NULL, NULL);
				else if (!cfg->restrictions)
					mem.Write<Vector>(IClientState.address + Offset::signatures::dwClientState_ViewAngles, Vector{ viewAngles.x + bestAngle.x / cfg->a.smooth, viewAngles.y + bestAngle.y / cfg->a.smooth, 0.f });
				
				if (cfg->a.autoStop && !cfg->restrictions) {
					const float velocity = localPlayer->velocity().length2D();
					Vector finalVector = Helpers::calculateRealAngles();
					if (velocity >= 30.f && (localPlayer->flags() & 1)) {
						if (finalVector.x >= 20) // FRONT, SO GO BACKWARDS
							mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceBackward, 6);
						if (finalVector.x <= -20) // BACK, SO GO FRONT
							mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceForward, 6);
						if (finalVector.y >= 20) // RIGHT, SO GO LEFT
							mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceLeft, 6);
						if (finalVector.y <= -20) // LEFT, SO GO RIGHT
							mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceRight, 6);;
					}
				}

				const auto& crosshair = localPlayer->crosshairID();
				if (!crosshair || crosshair > 64) continue;
				if (cfg->a.autoShot || (cfg->a.autoShot && cfg->a.autoStop && localPlayer->velocity().length2D() < 15.f)) {
					if (cfg->restrictions) {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						std::this_thread::sleep_for(std::chrono::milliseconds(2));
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
					else
						mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceAttack, 6);
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

	const auto& viewAngles = mem.Read<ImVec2>(IClientState.address + Offset::signatures::dwClientState_ViewAngles);

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

		mem.Write<ImVec2>(IClientState.address + Offset::signatures::dwClientState_ViewAngles, newAngles);

		oldAimPunch.x = localPlayer->aimPunch().x * 2.f;
		oldAimPunch.y = localPlayer->aimPunch().y * 2.f;

	} else oldAimPunch = {};

}