#include "Aimbot.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../GUI.h"
#include "../Offsets.h"
#include "../Helpers.h"
#include "../Config.h"

#include "../SDK/LocalPlayer.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"
#include "../SDK/WeaponInfo.h"
#include "../SDK/WeaponID.h"

#include <chrono>

void Aimbot::run() noexcept {
	while (THREAD_LOOP) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (!cfg->a.enabledAimbot) continue;

		if (!localPlayer || localPlayer->isDead() || localPlayer->nextAttack() > serverTime || localPlayer->nextPrimaryAttack() > serverTime || localPlayer->isDefusing() || localPlayer->waitForNoAttack())
			continue;

		const auto eyePosition = localPlayer->origin() + localPlayer->viewOffset();
		const auto& viewAngles = mem.Read<Vector>(IClientState.address + Offset::signatures::dwClientState_ViewAngles);

		if (!gameData.observerData.empty() && cfg->a.disableWhileBeingSpectated) {
			for (auto& i : gameData.observerData) {
				if(!Helpers::isFriendly(i.steamID, cfg->friendlySteamIDs))
					break;
			}
		}
		
		
		const auto& activeWeapon = localPlayer->getActiveWeapon();

		if (!activeWeapon->isValid())
			continue;

		if (activeWeapon->clip() < 1)
			continue;

		int weaponIndex = getWeaponIndex((WeaponID)localPlayer->getWeaponIDFromPlayer());
		if (!weaponIndex)
			continue;

		int weaponClass = getWeaponClass((WeaponID)localPlayer->getWeaponIDFromPlayer());

		if (!cfg->a.weapons[weaponIndex].enable)
			weaponIndex = weaponClass;

		if (!cfg->a.weapons[weaponIndex].enable)
			weaponIndex = 0;

		const auto& aimPunch = localPlayer->aimPunch() * 2.f;

		auto& config = cfg->a.weapons[weaponIndex];

		if (config.ignoreFlash && localPlayer->isFlashed())
			continue;

		float bestFov = config.fov;
		Vector bestAngle = {};

		std::vector<Aimbot::Enemies> enemies;
		for (int i = 1; i <= globalVars->maxClients; ++i) {
			const auto entity = getEntity(i);

			if (!entity->isValid())
				continue;

			if (entity->isDead() || entity->dormant() || entity->gunGameImmunity())
				continue;

			if (config.visibleOnly && !entity->isVisible())
				continue;

			if (!config.friendlyFire && entity->isSameTeam() && !isDangerZoneModePlayed)
				continue;

			Vector angle;
			float fov;

			for (auto bone : { 8, 4, 3, 7, 6, 5 }) {
				const auto bonePosition = 8 - config.bone;

				const auto bonePos = entity->bonePosition(bonePosition);

				angle = Helpers::calculateRelativeAngle(eyePosition, bonePos, { viewAngles.x + aimPunch.x, viewAngles.y + aimPunch.y, 0.f });

				fov = angle.length2D(); //fov

				if (fov < bestFov) {
					bestFov = fov;
					bestAngle = angle;
				}
			}

			const auto origin{ entity->origin() };
			const auto health{ entity->health() }; //health
			const auto distance{ localPlayer->origin().distTo(origin) }; //distance
			enemies.emplace_back(i, health, distance, fov);
		}

		if (enemies.empty())
			continue;

		switch (config.priority)
		{
		case 0:
			std::sort(enemies.begin(), enemies.end(), healthSort);
			break;
		case 1:
			std::sort(enemies.begin(), enemies.end(), distanceSort);
			break;
		case 2:
			std::sort(enemies.begin(), enemies.end(), fovSort);
			break;
		default:
			break;
		}

		//Get Sensitivity
		const uintptr_t sensPtr = IClient.address + Offset::signatures::dwSensitivityPtr;
		auto sensitivity = mem.Read<uintptr_t>(IClient.address + Offset::signatures::dwSensitivity);
		sensitivity ^= sensPtr;

		const float actualSensitivity = *reinterpret_cast<float*>(&sensitivity);

		const float multiplier = (50.f / actualSensitivity);
		DWORD xMove = static_cast<DWORD>(-bestAngle.y * multiplier / config.smooth);
		DWORD yMove = static_cast<DWORD>(bestAngle.x * multiplier / config.smooth);
		bool doOnce = true;
		for (const auto& target : enemies) {

			if (bestAngle.notNull() && (cfg->a.hotkey.isActive() && cfg->a.hotkey.isSet() || !cfg->a.hotkey.isSet())) {
				if (cfg->restrictions && !showMenu) {
					if (doOnce) {
						mouse_event(MOUSEEVENTF_MOVE, xMove, yMove, NULL, NULL);
						doOnce = false;
					}
				}
				else if (!cfg->restrictions) {
					Vector correctAngles{ viewAngles.x + bestAngle.x / config.smooth, viewAngles.y + bestAngle.y / config.smooth, 0.f };
					correctAngles.normalize();
					correctAngles.clamp();
					mem.Write<Vector>(IClientState.address + Offset::signatures::dwClientState_ViewAngles, correctAngles);
				}
				
				const short& weaponID = localPlayer->getWeaponIDFromPlayer();

				if (config.autoStop && !cfg->restrictions) {
					const float velocity = localPlayer->velocity().length2D();
					Vector finalVector = Helpers::calculateRealAngles();
					if (velocity >= (getWeaponMaxSpeed(weaponID) / 3) && (localPlayer->flags() & 1)) {
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

				if (!crosshair || crosshair > 64)
					continue;
				if (const auto ent = getEntity(crosshair - 1); ent->isSameTeam() && !config.friendlyFire)
					continue;

				if (config.autoShot) {
					if (localPlayer->velocity().length2D() > (getWeaponMaxSpeed(weaponID) / 3) && config.forceAccuracy) // Force accuracy shoots when spread doesnt affect player
						break;

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

void Aimbot::drawFov() noexcept {
	if (!cfg->a.enabledAimbot || !cfg->a.drawFov.enabled)
		return;

	if (!localPlayer || !localPlayer->isAlive())
		return;

	const auto& activeWeapon = localPlayer->getActiveWeapon();

	if (!activeWeapon->isValid())
		return;

	int weaponIndex = getWeaponIndex((WeaponID)localPlayer->getWeaponIDFromPlayer());
	if (!weaponIndex)
		return;

	int weaponClass = getWeaponClass((WeaponID)localPlayer->getWeaponIDFromPlayer());

	if (!cfg->a.weapons[weaponIndex].enable)
		weaponIndex = weaponClass;

	if (!cfg->a.weapons[weaponIndex].enable)
		weaponIndex = 0;

	ImVec2 mid = gameScreenSize / 2.f + gameScreenPos;
	Vector aimPunch = localPlayer->aimPunch() / 2.f;
	ImVec2 recoil = { mid.x - (screenSize.x / 90.f * aimPunch.y), mid.y + (screenSize.x / 90.f * aimPunch.x) };

	const auto radius = std::tan(Helpers::deg2rad(cfg->a.weapons[weaponIndex].fov) / (16.0f / 6.0f)) / std::tan(Helpers::deg2rad(localPlayer->isScoped() ? localPlayer->fov() : 90.0f) / 2.0f) * gameScreenSize.x;
	if (radius > gameScreenSize.x || radius > gameScreenSize.y || !std::isfinite(radius))
		return;

	const auto color = Helpers::calculateColor(cfg->a.drawFov);
	ImGui::GetBackgroundDrawList()->AddCircleFilled(localPlayer->shotsFired() > 1 ? recoil : mid, radius, color);
	if (cfg->a.drawFov.outline)
		ImGui::GetBackgroundDrawList()->AddCircle(localPlayer->shotsFired() > 1 ? recoil : mid, radius, color | IM_COL32_A_MASK, 360);
}