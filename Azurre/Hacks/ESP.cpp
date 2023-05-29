#include "ESP.h"
#include "SkinChanger.h"

#include "../Config.h"
#include "../Core.h"

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

Vector getBonePos(std::array<Matrix3x4, 128> bonematrix, int idx) {
	const auto bone = bonematrix.at(idx);
	return Vector(bone[0][3], bone[1][3], bone[2][3]);
}

void drawBorderBox(ImVec2 pos, float width, float height, float duckHeight, ImU32 color, ImU32 color_) {
	drawList->AddRectFilled({ pos.x - width, pos.y - .5f + duckHeight }, { pos.x + width, pos.y + .5f + duckHeight }, color); //TOP
	drawList->AddRectFilledMultiColor({ pos.x - width + .5f, pos.y + duckHeight }, { pos.x - width - .5f, pos.y - height }, color, color, color_, color_); //LEFT
	drawList->AddRectFilledMultiColor({ pos.x + width + .5f, pos.y + duckHeight }, { pos.x + width - .5f, pos.y - height }, color, color, color_, color_); //RIGHT
	drawList->AddRectFilled({ pos.x - width, pos.y - height - .5f}, { pos.x + width, pos.y - height + .5f}, color_); //BOTTOM
}

void drawLines(ImVec2 pos, ImVec2 pos_, ImU32 color) {
	drawList->AddLine(pos, pos_, color); //TOP
}

void drawSkeleton(uintptr_t entBones, Matrix4x4 m) {

	for (int skel = 0; skel < 128; skel++) {
		const auto bonePos = Vector{
			mem.Read<float>(entBones + 0x30 * skel + 0x0C),
			mem.Read<float>(entBones + 0x30 * skel + 0x1C),
			mem.Read<float>(entBones + 0x30 * skel + 0x2C)
		};

		Vector vStart = Helpers::world2Screen(gameScreenSize, bonePos, m);
		const auto centerText = ImGui::CalcTextSize(std::to_string(skel).c_str());
		drawList->AddCircleFilled({ vStart.x, vStart.y }, 8.f, IM_COL32(255, 255, 255, 255));
		drawList->AddText({ vStart.x - centerText.x / 2.f, vStart.y - centerText.x / 2.f },IM_COL32(255, 0, 0, 255), std::to_string(skel).c_str());
	}	
}

void drawHealthBar(ImVec2 pos, float width, float height, float duckHeight , int health, ImU32 color, ImU32 colorNumber, ColorToggle3 showHealthNumber) {
	const auto centerText = ImGui::CalcTextSize(std::to_string(health).c_str());
	drawList->AddLine({ pos.x + width + (width * .1f), pos.y - (100 - health) / 100.0f * height + (duckHeight / (100 / health)) }, { pos.x + width + (width * .1f), pos.y - height }, color, 4.f);
	if (showHealthNumber.enabled) { // text
		drawList->AddText({ (pos.x + width + (width * .1f) - centerText.x / 2) + 1.f, (pos.y - (100 - health) / 100.0f * height + (duckHeight / (100 / health))) + 1.f }, colorNumber & IM_COL32_A_MASK, std::to_string(health).c_str());
		drawList->AddText({ pos.x + width + (width * .1f) - centerText.x / 2, pos.y - (100 - health) / 100.0f * height + (duckHeight / (100 / health)) }, colorNumber, std::to_string(health).c_str());
	}
}

void drawPlayerName(ImVec2 pos, float width, float height, float duckHeight, std::string name, ImU32 color) {
	const auto centerText = ImGui::CalcTextSize(name.c_str());
	drawList->AddText({pos.x - centerText.x / 2, pos.y - 4.f + (height * .1f) + duckHeight }, color, name.c_str());
}

void renderPlayer(Entity* entity, int index, Matrix4x4 m) {
	constexpr std::array categories{ "Allies", "Enemies Occluded", "Enemies Visible" };
	int tab = 1;
	int spotted = 0;
	spotted = mem.Read<int>(entity + Offset::netvars::m_bSpotted);

	if (entity->isSameTeam()){
		tab = 0;
		spotted = 0;
	}

	auto& config = cfg->esp.players[categories[tab + spotted]];

#pragma region Box
	Vector pos = entity->origin();
	Vector head;
	pos.z -= 8.f;
	head.x = pos.x;
	head.y = pos.y;
	head.z = pos.z + 75.f;

	float duckHeight = 0.f;
	if (entity->duckAmount() > .25f)
		duckHeight += (16.f * entity->duckAmount());

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, m);
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, m);

	ImVec2 pos2D = { posScreen.x, posScreen.y };
	ImVec2 head2D = { headScreen.x, headScreen.y };

	float height = headScreen.y - posScreen.y;
	const float width = height / 4;

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop.color : config.box.solid.color;
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom.color : config.box.solid.color;

	const auto colorFinal = ImGui::GetColorU32({ colorBox[0], colorBox[1], colorBox[2], 1.f });
	const auto colorFinal_ = ImGui::GetColorU32({ colorBox_[0], colorBox_[1], colorBox_[2], 1.f});

#pragma endregion Box
#pragma region Health Bar
	int health = entity->health();

	const auto colorNumberHealth = ImGui::GetColorU32({ config.healthBar.showHealthNumber.color[0], config.healthBar.showHealthNumber.color[1], config.healthBar.showHealthNumber.color[2], 1.f });

	const auto colorHealthBar = ImGui::GetColorU32({ config.healthBar.solidColor.color[0], config.healthBar.solidColor.color[1], config.healthBar.solidColor.color[2], 1.f});
	const auto colorHealthBarMix = ImGui::GetColorU32({ (float)(1.f - health / 100.f), (health / 100.f), 0, 1.f });

	const auto colorHealthBarFinal = config.healthBar.solidColor.enabled ? colorHealthBar : colorHealthBarMix;
#pragma endregion Health Bar
#pragma region Player Name
	const auto colorNames = ImGui::GetColorU32({ config.other.names.color[0], config.other.names.color[1], config.other.names.color[2], 1.f });
	const auto colorWeapon = ImGui::GetColorU32({ config.other.weapons.color[0], config.other.weapons.color[1], config.other.weapons.color[2], 1.f });

	const auto& userInfoTable = mem.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo);
	const auto& items = mem.Read<uintptr_t>(mem.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
	PlayerInfo pInfo = mem.Read<PlayerInfo>(mem.Read<uintptr_t>(items + 0x28 + (index * 0x34)));
	std::string name = pInfo.name;
	std::string weapon = Skin::getWeaponIDName(entity->getWeaponIDFromPlayer());

#pragma endregion Player Name
	const auto colorLines = ImGui::GetColorU32({ config.other.lines.color[0], config.other.lines.color[1], config.other.lines.color[2], 1.f });

	if (posScreen.z >= 0.01f && !entity->dormant()) {
		if (config.skeleton) drawSkeleton(entity->boneMatrix(), m);
		if (config.box.enabled) drawBorderBox( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, duckHeight, colorFinal, colorFinal_);
		if (config.healthBar.enabled) drawHealthBar( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, duckHeight, health, colorHealthBarFinal, colorNumberHealth, config.healthBar.showHealthNumber);
		if (config.other.names.enabled) drawPlayerName( { gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, duckHeight, name, colorNames);
		if (config.other.weapons.enabled) drawPlayerName( { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y}, width, height, duckHeight, weapon, colorWeapon);
		if (config.other.lines.enabled) drawLines( { gameScreenPos.x + (gameScreenSize.x / 2), gameScreenSize.y + gameScreenPos.y }, { gameScreenPos.x + posScreen.x, gameScreenPos.y + posScreen.y }, colorLines);
	}
}

void ESP::render() noexcept {
	if (!localPlayer) return;
	if (!cfg->esp.enabled) return;

	for (int i = 1; i < 32; i++) {
		auto entity = getEntity(i);

		if (!entity)
			continue;

		if (entity->isDead())
			continue;

		if ((uintptr_t)entity == localPlayer.get())
			continue;

		renderPlayer(entity, i, viewMatix);
	}
}