#include "ESP.h"
#include "SkinChanger.h"

#include "../Config.h"
#include "../Core.h"
#include "../Helpers.h"

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
	drawList->AddRectFilled({ pos.x - size, pos.y - .5f - size }, { pos.x + size, pos.y + .5f - size }, color); //TOP
	drawList->AddRectFilledMultiColor({ pos.x - size + .5f, pos.y - size }, { pos.x - size - .5f, pos.y + size }, color, color, color_, color_); //LEFT
	drawList->AddRectFilledMultiColor({ pos.x + size + .5f, pos.y - size }, { pos.x + size - .5f, pos.y + size }, color, color, color_, color_); //RIGHT
	drawList->AddRectFilled({ pos.x - size, pos.y + size - .5f}, { pos.x + size, pos.y + size + .5f}, color_); //BOTTOM
}

void drawBorderTwoBox(ImVec2 pos, float width, float height, ImU32 color, ImU32 color_) {
	drawList->AddRectFilled({ pos.x - width, pos.y - .5f }, { pos.x + width, pos.y + .5f }, color); //TOP
	drawList->AddRectFilledMultiColor({ pos.x - width + .5f, pos.y }, { pos.x - width - .5f, pos.y - height }, color, color, color_, color_); //LEFT
	drawList->AddRectFilledMultiColor({ pos.x + width + .5f, pos.y }, { pos.x + width - .5f, pos.y - height }, color, color, color_, color_); //RIGHT
	drawList->AddRectFilled({ pos.x - width, pos.y - height - .5f}, { pos.x + width, pos.y - height + .5f}, color_); //BOTTOM
}

void drawLines(ImVec2 pos, ImVec2 pos_, ImU32 color) {
	drawList->AddLine(pos, pos_, color); //TOP
}

void drawSkeleton(uintptr_t entBones) {

	for (int skel = 0; skel < 128; skel++) {
		const auto bonePos = Vector{
			mem.Read<float>(entBones + 0x30 * skel + 0x0C),
			mem.Read<float>(entBones + 0x30 * skel + 0x1C),
			mem.Read<float>(entBones + 0x30 * skel + 0x2C)
		};

		Vector vStart = Helpers::world2Screen(gameScreenSize, bonePos, viewMatix);
		const auto centerText = ImGui::CalcTextSize(std::to_string(skel).c_str());
		drawList->AddCircleFilled({ vStart.x, vStart.y }, 8.f, IM_COL32(255, 255, 255, 255));
		drawList->AddText({ vStart.x - centerText.x / 2.f, vStart.y - centerText.x / 2.f },IM_COL32(255, 0, 0, 255), std::to_string(skel).c_str());
	}	
}

void drawHealthBar(ImVec2 pos, float width, float height , int health, ImU32 color, ImU32 colorNumber, ColorToggle3 showHealthNumber) {
	const auto centerText = ImGui::CalcTextSize(std::to_string(health).c_str());

	if (health < 1)
		return;

	drawList->AddLine({ pos.x + width + (width * .1f), pos.y - (100 - health) / 100.0f * height }, { pos.x + width + (width * .1f), pos.y - height }, color, 4.f);
	if (showHealthNumber.enabled) { // text
		drawList->AddText({ (pos.x + width + (width * .1f) - centerText.x / 2) + 1.f, (pos.y - (100 - health) / 100.0f * height) + 1.f }, colorNumber & IM_COL32_A_MASK, std::to_string(health).c_str());
		drawList->AddText({ pos.x + width + (width * .1f) - centerText.x / 2, pos.y - (100 - health) / 100.0f * height}, colorNumber, std::to_string(health).c_str());
	}
}

void drawPlayerName(ImVec2 pos, float width, float height, std::string name, ImU32 color) {
	const auto centerText = ImGui::CalcTextSize(name.c_str());
	drawList->AddText({pos.x - centerText.x / 2, pos.y - 4.f + (height * .1f) }, color, name.c_str());
}

constexpr std::array categories{ "Allies", "Enemies Occluded", "Enemies Visible", "Weapons" };

void renderWeapon(Entity* entity) {
	auto& config = cfg->esp.weapons["All"];

	Vector pos = entity->origin();
	Vector head;
	pos.z -= 8.f;
	head.x = pos.x;
	head.y = pos.y;
	head.z = pos.z + 16.f;

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, viewMatix);
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, viewMatix);

	float height = headScreen.y - posScreen.y;
	const float width = height;

	std::string name = Skin::getWeaponIDName(entity->getWeaponID());

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop : config.box.solid;
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom : config.box.solid;

	if (posScreen.z >= 0.01f) {
		if (config.box.enabled) drawBorderTwoBox({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, Helpers::calculateColor(colorBox), Helpers::calculateColor(colorBox_));
		if (config.other.names.enabled)	drawPlayerName({ gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, 0.f, 0.f, name, Helpers::calculateColor(config.other.names));
		if (config.other.lines.enabled) drawLines({ gameScreenPos.x + (gameScreenSize.x / 2), gameScreenSize.y + gameScreenPos.y }, { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, Helpers::calculateColor(config.other.lines));
	}
}

void renderProjectile(ProjectileData* projectile) {
	auto& config = cfg->esp.projectiles["All"];

	Vector pos = projectile->entity->origin();
	Vector head;
	pos.z -= 8.f;
	head.x = pos.x;
	head.y = pos.y;
	head.z = pos.z + 8.f;

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, viewMatix);
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, viewMatix);

	float height = headScreen.y - posScreen.y;
	const float width = height / 2.f;

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop : config.box.solid;
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom : config.box.solid;

	if (posScreen.z >= 0.01f) {
		if (config.box.enabled) drawBorderTwoBox({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, Helpers::calculateColor(colorBox), Helpers::calculateColor(colorBox_));
		if (config.names.enabled)	drawPlayerName({ gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, 0.f, 0.f, projectile->name, Helpers::calculateColor(config.names));
	}
}

void renderPlayer(Entity* entity, int index) {
	int tab = 1;
	unsigned int spotted = 0;
	spotted = entity->isVisible();

	if (entity->isSameTeam()){
		tab = 0;
		spotted = 0;
	}

	if (spotted >= 2)
		spotted = 1;

	auto& config = cfg->esp.players[categories[tab + spotted]];

#pragma region Box
	Vector pos = entity->origin();
	Vector head = entity->bonePosition(8);
	pos.z -= 8.f;
	head.z += 8.f;

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, viewMatix);
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, viewMatix);

	Vector headPos = entity->bonePosition(8);
	Vector headBoxScreen = Helpers::world2Screen(gameScreenSize, headPos, viewMatix);

	float height = headScreen.y - posScreen.y;
	const float width = height / 4;

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop : config.box.solid;
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom : config.box.solid;

	const auto colorHeadBox = config.headBox.gradientColor ? config.headBox.grandientTop : config.headBox.solid;
	const auto colorHeadBox_ = config.headBox.gradientColor ? config.headBox.grandientBottom : config.headBox.solid;

#pragma endregion Box
#pragma region Health Bar
	int health = entity->health();

	const auto colorNumberHealth = ImGui::GetColorU32({ config.healthBar.showHealthNumber.color[0], config.healthBar.showHealthNumber.color[1], config.healthBar.showHealthNumber.color[2], 1.f });

	const auto colorHealthBar = ImGui::GetColorU32({ config.healthBar.solidColor.color[0], config.healthBar.solidColor.color[1], config.healthBar.solidColor.color[2], 1.f});
	const auto colorHealthBarMix = ImGui::GetColorU32({ (float)(1.f - health / 100.f), (health / 100.f), 0, 1.f });

	const auto colorHealthBarFinal = config.healthBar.solidColor.enabled ? colorHealthBar : colorHealthBarMix;
#pragma endregion Health Bar
#pragma region Player Name
	const auto& userInfoTable = mem.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo);
	const auto& items = mem.Read<uintptr_t>(mem.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
	PlayerInfo pInfo = mem.Read<PlayerInfo>(mem.Read<uintptr_t>(items + 0x28 + (index * 0x34)));
	std::string name = pInfo.name;
	std::string weapon = Skin::getWeaponIDName(entity->getWeaponIDFromPlayer());

#pragma endregion Player Name
	if (posScreen.z >= 0.01f && !entity->dormant()) {
		if (config.skeleton) drawSkeleton(entity->boneMatrix());
		if (config.headBox.enabled) drawBorderBox( { gameScreenPos.x + headBoxScreen.x, gameScreenPos.y + headBoxScreen.y }, width / 3, Helpers::calculateColor(colorHeadBox), Helpers::calculateColor(colorHeadBox_));
		if (config.box.enabled) drawBorderTwoBox( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, Helpers::calculateColor(colorBox), Helpers::calculateColor(colorBox_));
		if (config.healthBar.enabled) drawHealthBar( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, health, colorHealthBarFinal, colorNumberHealth, config.healthBar.showHealthNumber);
		if (config.other.names.enabled) drawPlayerName( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, name, Helpers::calculateColor(config.other.names));
		if (config.weapons.enabled) drawPlayerName( { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y}, width, height, weapon, Helpers::calculateColor(config.weapons));
		if (config.other.lines.enabled) drawLines( { gameScreenPos.x + (gameScreenSize.x / 2), gameScreenSize.y + gameScreenPos.y }, { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, Helpers::calculateColor(config.other.lines));
	}
}

void ESP::render() noexcept {
	if (!localPlayer) return;
	if (gameData.playerData.empty() || gameData.weaponData.empty()) return;
	if (!cfg->esp.enabled) return;

	for (auto& player : gameData.playerData) {
		if (player.entity->isDead() || (uintptr_t)player.entity == localPlayer.get())
			continue;
		renderPlayer(player.entity, player.idx);
	}

	for (auto& weapon : gameData.weaponData) {
		if (weapon->origin() == Vector{ 0.f, 0.f, 0.f })
			continue;
		renderWeapon(weapon);
	}

	for (auto& projectile : gameData.projectileData) {
		if (projectile.entity->origin() == Vector{ 0.f, 0.f, 0.f })
			continue;
		renderProjectile(&projectile);
	}
}