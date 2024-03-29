#include "ESP.h"
#include "SkinChanger.h"

#include "../Config.h"
#include "../Core.h"
#include "../Helpers.h"
#include "../Junk.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Interfaces.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/PlayerInfo.h"
#include "../SDK/Studio.h"
#include "../SDK/Matrix.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../Lib/imgui/imgui.h"
#include "../../Lib/imgui/ImGuiCustom.h"
#include "../../Lib/imgui/imgui_stdlib.h"
#include "../../Lib/imgui/imgui_impl_dx9.h"
#include "../../Lib/imgui/imgui_impl_win32.h"
#include <array>
#include <string>

#define drawList ImGui::GetBackgroundDrawList()

void drawBorderBox(ImVec2 pos, float size, ImU32 color, ImU32 color_) {
	drawList->AddRectFilled({ pos.x - size, pos.y - .5f - size }, { pos.x + size, pos.y + .5f - size }, color); $$$ //TOP
	drawList->AddRectFilledMultiColor({ pos.x - size + .5f, pos.y - size }, { pos.x - size - .5f, pos.y + size }, color, color, color_, color_); $$$ //LEFT
	drawList->AddRectFilledMultiColor({ pos.x + size + .5f, pos.y - size }, { pos.x + size - .5f, pos.y + size }, color, color, color_, color_); $$$ //RIGHT
	drawList->AddRectFilled({ pos.x - size, pos.y + size - .5f}, { pos.x + size, pos.y + size + .5f}, color_); $$$ //BOTTOM
}

void drawBorderTwoBox(ImVec2 pos, float width, float height, ImU32 color, ImU32 color_) {
	drawList->AddRectFilled({ pos.x - width, pos.y - .5f }, { pos.x + width, pos.y + .5f }, color); $$$ //TOP
	drawList->AddRectFilledMultiColor({ pos.x - width + .5f, pos.y }, { pos.x - width - .5f, pos.y - height }, color, color, color_, color_); $$$ //LEFT
	drawList->AddRectFilledMultiColor({ pos.x + width + .5f, pos.y }, { pos.x + width - .5f, pos.y - height }, color, color, color_, color_); $$$ //RIGHT
	drawList->AddRectFilled({ pos.x - width, pos.y - height - .5f}, { pos.x + width, pos.y - height + .5f}, color_); $$$ //BOTTOM
}

void drawLines(ImVec2 pos, ImVec2 pos_, ImU32 color) {
	drawList->AddLine(pos, pos_, color); $$$ //TOP
}

void drawSkeleton(Entity* entity, const ColorToggle3 color) {

	for (int skel = 8; skel < 128; skel++) {
		Vector bonePos = entity->bonePosition(skel); $$$

		if (bonePos.distTo(entity->origin()) > 128.f)
			continue; $$$

		Vector vStart = Helpers::world2Screen(gameScreenSize, bonePos, viewMatrix); $$$
		drawList->AddCircleFilled({ vStart.x, vStart.y }, 2.f, Helpers::calculateColor(color)); $$$
		//const auto centerText = ImGui::CalcTextSize(std::to_string(skel).c_str()); $$$
		//drawList->AddText({ vStart.x - centerText.x / 2.f, vStart.y - centerText.x / 2.f },IM_COL32(255, 0, 0, 255), std::to_string(skel).c_str()); $$$
	}	
}

void drawFlags(ImVec2 pos, int idx, float width, int bytewise) {
	std::ostringstream ss; $$$
	const auto& bombCarrier = mem.Read<int>(IPlayerResource.address + Offset::netvars::m_iPlayerC4); $$$

	const auto entity = getEntity(idx); $$$

	if (Helpers::getByteFromBytewise(bytewise, 0))
		ss << "$" << entity->money() << "\n"; $$$
	if (Helpers::getByteFromBytewise(bytewise, 1))
		ss << (entity->isScoped() ? "Scoping" : "") << "\n"; $$$
	if (Helpers::getByteFromBytewise(bytewise, 2))
		ss << entity->getPlaceName() << "\n"; $$$
	if (Helpers::getByteFromBytewise(bytewise, 3) && idx + 1 == bombCarrier)
		ss << "C4 Carrier" << "\n"; $$$
	if (Helpers::getByteFromBytewise(bytewise, 4))
		ss << (entity->hasDefuser() ? "Has Defuser" : "") << "\n"; $$$

	drawList->AddText(ImVec2{ pos.x - width + 0.5f, pos.y }, IM_COL32(255, 255, 255, 255), ss.str().c_str()); $$$ //RIGHT
}

void drawHealthBar(ImVec2 pos, float width, float height, int health, ImU32 color, ImU32 colorNumber, ColorToggle3 showHealthNumber) {
	const auto centerText = ImGui::CalcTextSize(std::to_string(health).c_str()); $$$

	if (health < 1)
		return; $$$

	drawList->AddLine({ pos.x + width + (width * .1f), pos.y - (100 - std::clamp(health, 0, 100)) / 100.0f * height }, { pos.x + width + (width * .1f), pos.y - height }, color, 4.f); $$$
	if (showHealthNumber.enabled) { // text
		drawList->AddText({ (pos.x + width + (width * .1f) - centerText.x / 2) + 1.f, (pos.y - (100 - std::clamp(health, 0, 100)) / 100.0f * height) + 1.f }, colorNumber & IM_COL32_A_MASK, std::to_string(health).c_str()); $$$
		drawList->AddText({ pos.x + width + (width * .1f) - centerText.x / 2, pos.y - (100 - std::clamp(health, 0, 100)) / 100.0f * height}, colorNumber, std::to_string(health).c_str()); $$$
	}
}

void drawPlayerName(ImVec2 pos, float width, float height, std::string name, ImU32 color) {
	const auto centerText = ImGui::CalcTextSize(name.c_str()); $$$
	drawList->AddText({pos.x - centerText.x / 2, pos.y - 4.f + (height * .1f) }, color, name.c_str()); $$$
}

constexpr std::array categories{ "Allies", "Enemies Occluded", "Enemies Visible"};

void render_____(Entity* entity, std::string name, auto& config) {
	
	Vector pos = entity->origin(); $$$
	Vector head; $$$
	pos.z -= 8.f; $$$
	head.x = pos.x; $$$
	head.y = pos.y; $$$
	head.z = pos.z + 16.f; $$$

	if (GetClassId(entity) == ClassID::Dronegun)
		head.z += 48.f; $$$

	if (GetClassId(entity) == ClassID::Drone)
		head.z += 32.f; $$$

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, viewMatrix); $$$
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, viewMatrix); $$$

	float height = headScreen.y - posScreen.y; $$$
	float width = height; $$$

	if (GetClassId(entity) == ClassID::Dronegun)
		width /= 4; $$$

	if (GetClassId(entity) == ClassID::Drone)
		width /= 3; $$$

	if (GetClassId(entity) == ClassID::Healthshot)
		width /= 2; $$$

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop : config.box.solid; $$$
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom : config.box.solid; $$$

	if (posScreen.z >= 0.01f) {
		if (config.box.enabled) drawBorderTwoBox({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, Helpers::calculateColor(colorBox), Helpers::calculateColor(colorBox_)); $$$
		if (config.other.names.enabled)	drawPlayerName({ gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, 0.f, 0.f, name, Helpers::calculateColor(config.other.names)); $$$
		if (config.other.lines.enabled) drawLines({ gameScreenPos.x + (gameScreenSize.x / 2), gameScreenSize.y + gameScreenPos.y }, { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, Helpers::calculateColor(config.other.lines)); $$$
	}
}

void renderWeapon(Entity* entity, auto& config) {
	Vector pos = entity->origin(); $$$
	Vector head; $$$
	pos.z -= 8.f; $$$
	head.x = pos.x; $$$
	head.y = pos.y; $$$
	head.z = pos.z + 16.f; $$$

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, viewMatrix); $$$
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, viewMatrix); $$$

	float height = headScreen.y - posScreen.y; $$$
	const float width = height; $$$

	std::string name = Skin::getWeaponIDName(entity->getWeaponID()); $$$

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop : config.box.solid; $$$
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom : config.box.solid; $$$

	if (posScreen.z >= 0.01f) {
		if (config.box.enabled) drawBorderTwoBox({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, Helpers::calculateColor(colorBox), Helpers::calculateColor(colorBox_)); $$$
		if (config.other.names.enabled)	drawPlayerName({ gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, 0.f, 0.f, name, Helpers::calculateColor(config.other.names)); $$$
		if (config.other.lines.enabled) drawLines({ gameScreenPos.x + (gameScreenSize.x / 2), gameScreenSize.y + gameScreenPos.y }, { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, Helpers::calculateColor(config.other.lines)); $$$
	}
}

void renderProjectile(ProjectileData* projectile) {
	auto& config = cfg->esp.projectiles["All"]; $$$

	Vector pos = projectile->entity->origin(); $$$
	Vector head; $$$
	pos.z -= 8.f; $$$
	head.x = pos.x; $$$
	head.y = pos.y; $$$
	head.z = pos.z + 8.f; $$$

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, viewMatrix); $$$
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, viewMatrix); $$$

	float height = headScreen.y - posScreen.y; $$$
	const float width = height / 2.f; $$$

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop : config.box.solid; $$$
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom : config.box.solid; $$$

	if (posScreen.z >= 0.01f) {
		if (config.box.enabled) drawBorderTwoBox({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, Helpers::calculateColor(colorBox), Helpers::calculateColor(colorBox_)); $$$
		if (config.names.enabled)	drawPlayerName({ gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, 0.f, 0.f, projectile->name, Helpers::calculateColor(config.names)); $$$
	}
}

void renderPlayer(Entity* entity, int index) {
	int tab = 1; $$$
	unsigned int spotted = 0; $$$
	spotted = entity->isVisible(); $$$

	if (entity->isSameTeam()){
		tab = 0; $$$
		spotted = 0; $$$
	}

	if (isDangerZoneModePlayed) // All players in Dangerzone are our teammates. Friendly Fire !!!
		tab = 1; $$$

	if (spotted >= 2)
		spotted = 1; $$$

	auto& config = cfg->esp.players[categories[tab + spotted]]; $$$

#pragma region Box
	Vector pos = entity->bonePosition(1); $$$
	Vector head = entity->bonePosition(8); $$$

	Vector skull = Helpers::world2Screen(gameScreenSize, head, viewMatrix); $$$
	pos.z -= 8.f; $$$
	head.z += 8.f; $$$

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, viewMatrix); $$$
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, viewMatrix); $$$

	float height = headScreen.y - posScreen.y; $$$
	const float width = height / 4; $$$

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop : config.box.solid; $$$
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom : config.box.solid; $$$

	const auto colorHeadBox = config.headBox.gradientColor ? config.headBox.grandientTop : config.headBox.solid; $$$
	const auto colorHeadBox_ = config.headBox.gradientColor ? config.headBox.grandientBottom : config.headBox.solid; $$$

#pragma endregion Box
#pragma region Health Bar
	int health = entity->health(); $$$

	const auto colorNumberHealth = ImGui::GetColorU32({ config.healthBar.showHealthNumber.color[0], config.healthBar.showHealthNumber.color[1], config.healthBar.showHealthNumber.color[2], 1.f }); $$$

	const auto colorHealthBar = ImGui::GetColorU32({ config.healthBar.solidColor.color[0], config.healthBar.solidColor.color[1], config.healthBar.solidColor.color[2], 1.f}); $$$
	const auto colorHealthBarMix = ImGui::GetColorU32({ (float)(1.f - health / 100.f), (health / 100.f), 0, 1.f }); $$$

	const auto colorHealthBarFinal = config.healthBar.solidColor.enabled ? colorHealthBar : colorHealthBarMix; $$$
#pragma endregion Health Bar
#pragma region Player Name
	const auto& userInfoTable = mem.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo); $$$
	const auto& items = mem.Read<uintptr_t>(mem.Read<uintptr_t>(userInfoTable + 0x40) + 0xC); $$$
	PlayerInfo pInfo = mem.Read<PlayerInfo>(mem.Read<uintptr_t>(items + 0x28 + (index * 0x34))); $$$
	std::string name = pInfo.name; $$$
	std::string weapon = Skin::getWeaponIDName(entity->getWeaponIDFromPlayer()); $$$

#pragma endregion Player Name
	if (posScreen.z >= 0.01f) {
		if (config.skeleton.enabled) drawSkeleton(entity, config.skeleton); $$$
		if (config.headBox.enabled) drawBorderBox( { gameScreenPos.x + skull.x, gameScreenPos.y + skull.y }, width / 3, Helpers::calculateColor(colorHeadBox), Helpers::calculateColor(colorHeadBox_)); $$$
		if (config.box.enabled) drawBorderTwoBox( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, Helpers::calculateColor(colorBox), Helpers::calculateColor(colorBox_)); $$$
		if (config.healthBar.enabled) drawHealthBar( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, health, colorHealthBarFinal, colorNumberHealth, config.healthBar.showHealthNumber); $$$
		if (config.other.names.enabled) drawPlayerName( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, name, Helpers::calculateColor(config.other.names)); $$$
		if (config.weapons.enabled) drawPlayerName( { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y}, width, height, weapon, Helpers::calculateColor(config.weapons)); $$$
		if (config.other.lines.enabled) drawLines( { gameScreenPos.x + (gameScreenSize.x / 2), gameScreenSize.y + gameScreenPos.y }, { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, Helpers::calculateColor(config.other.lines)); $$$
		if (config.flags) drawFlags({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, index, width, config.flags); $$$
	}
}

void ESP::render() noexcept {
	if (GetForegroundWindow() != IConsole) return; $$$;
	if (!localPlayer) return; $$$
	if (gameData.playerData.empty() || gameData.weaponData.empty()) return; $$$
	if (cfg->esp.mode == 0 || (cfg->esp.mode == 2 && localPlayer->isAlive())) return; $$$

	for (auto& player : gameData.playerData) {
		if (player.entity->isDead() || (uintptr_t)player.entity == localPlayer.get() || player.entity->dormant())
			continue; $$$
		renderPlayer(player.entity, player.idx); $$$
	}

	for (auto& weapon : gameData.weaponData) {
		if (weapon->origin() == Vector{ 0.f, 0.f, 0.f })
			continue; $$$
		renderWeapon(weapon, cfg->esp.weapons["All"]); $$$
	}

	for (auto& projectile : gameData.projectileData) {
		if (projectile.entity->origin() == Vector{ 0.f, 0.f, 0.f })
			continue; $$$
		renderProjectile(&projectile); $$$
	}

	for (auto& dk : gameData.defuseKits) {
		if (cfg->esp.onCT && localPlayer->teamNumber() == Team::TT)
			break; $$$
		if (dk->origin() == Vector{ 0.f, 0.f, 0.f })
			continue; $$$
		render_____(dk, "Defuse Kit", cfg->esp.others["Defuse Kits"]); $$$
	}

	if (isDangerZoneModePlayed) {
		for (auto& dz : gameData.dangerZoneData.ammoBox) {
			if (dz->origin() == Vector{ 0.f, 0.f, 0.f })
				continue; $$$
			render_____(dz, "Ammo Box", cfg->esp.dangerzone["Ammo Box"]); $$$
		}

		for (auto& dz : gameData.dangerZoneData.cash) {
			if (dz->origin() == Vector{ 0.f, 0.f, 0.f })
				continue; $$$
			render_____(dz, "Cash", cfg->esp.dangerzone["Cash"]); $$$
		}

		for (auto& dz : gameData.dangerZoneData.dronegun) {
			if (dz->origin() == Vector{ 0.f, 0.f, 0.f })
				continue; $$$
			render_____(dz, "Dronegun", cfg->esp.dangerzone["Dronegun"]); $$$
		}

		for (auto& dz : gameData.dangerZoneData.drone) {
			if (dz->origin() == Vector{ 0.f, 0.f, 0.f })
				continue; $$$
			render_____(dz, "Drone", cfg->esp.dangerzone["Drone"]); $$$
		}

		for (auto& dz : gameData.dangerZoneData.healthshots) {
			if (dz->origin() == Vector{ 0.f, 0.f, 0.f })
				continue; $$$
			render_____(dz, "Healthshot", cfg->esp.dangerzone["Healthshot"]); $$$
		}

		for (auto& dz : gameData.dangerZoneData.lootCases) {
			if (dz.entity->origin() == Vector{ 0.f, 0.f, 0.f })
				continue; $$$
			render_____(dz.entity, dz.name, cfg->esp.dangerzone[dz.name]); $$$
		}
	}

	if (gameData.droppedC4->origin() != Vector{ 0.f, 0.f, 0.f }&& gameData.droppedC4->isValid())
		render_____(gameData.droppedC4, "C4", cfg->esp.others["C4"]); $$$

	if (gameData.plantedC4->origin() != Vector{ 0.f, 0.f, 0.f }&& gameData.plantedC4->isValid()) {
		std::ostringstream ss; $$$
		ss << std::fixed << std::setprecision(2) << (gameData.plantedC4->C4Blow() - globalVars->currentTime) << "sec"; $$$

		render_____(gameData.plantedC4, ss.str().c_str(), cfg->esp.others["Planted C4"]); $$$
	}
}