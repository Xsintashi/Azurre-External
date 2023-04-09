#include "ESP.h"

#include "../Config.h"
#include "../Core.h"

#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Interfaces.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/Matrix.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../Lib/imgui/imgui.h"
#include "../../Lib/imgui/ImGuiCustom.h"
#include "../../Lib/imgui/imgui_stdlib.h"
#include "../../Lib/imgui/imgui_impl_dx9.h"
#include "../../Lib/imgui/imgui_impl_win32.h"
#include <string>
#include "../SDK/PlayerInfo.h"

#define drawList ImGui::GetBackgroundDrawList()

void drawBorderBox(ImVec2 pos, float width, float height, ImU32 color, ImU32 color_) {
	drawList->AddRectFilled({ pos.x - width, pos.y - .5f }, { pos.x + width, pos.y + .5f }, color); //TOP
	drawList->AddRectFilledMultiColor({ pos.x - width + .5f, pos.y }, { pos.x - width - .5f, pos.y - height }, color, color, color_, color_); //LEFT
	drawList->AddRectFilledMultiColor({ pos.x + width + .5f, pos.y }, { pos.x + width - .5f, pos.y - height }, color, color, color_, color_); //RIGHT
	drawList->AddRectFilled({ pos.x - width, pos.y - height - .5f }, { pos.x + width, pos.y - height + .5f }, color_); //BOTTOM
}

void drawHealthBar(ImVec2 pos, float width, float height, int health, ImU32 color, ImU32 colorNumber, ColorToggle3 showHealthNumber) {
	const auto centerText = ImGui::CalcTextSize(std::to_string(health).c_str());
	drawList->AddLine({ pos.x + width + (width * .1f), pos.y - (100 - health) / 100.0f * height }, { pos.x + width + (width * .1f), pos.y - height }, color, 4.f);
	if (showHealthNumber.enabled) { // text
		drawList->AddText({ (pos.x + width + (width * .1f) - centerText.x / 2) + 1.f, (pos.y - (100 - health) / 100.0f * height) + 1.f }, colorNumber & IM_COL32_A_MASK, std::to_string(health).c_str());
		drawList->AddText({ pos.x + width + (width * .1f) - centerText.x / 2, pos.y - (100 - health) / 100.0f * height }, colorNumber, std::to_string(health).c_str());
	}
}

void drawPlayerName(ImVec2 pos, float width, float height, std::string name, ImU32 color) {
	const auto centerText = ImGui::CalcTextSize(name.c_str());
	drawList->AddText({pos.x - centerText.x / 2, pos.y - 4.f + (height * .1f)}, color, name.c_str());
}

void renderPlayer(Entity* player, int index, Matrix4x4 m) {
	const ImU32 color = ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f });
	const ImU32 red = ImGui::GetColorU32({ 1.f, 0.f, 0.f, 1.f });

	int tab = 1;
	if (player->isSameTeam()) tab = 0;

	int spotted = csgo.Read<int>((uintptr_t)player + Offset::netvars::m_bSpotted);

	auto& config = cfg->esp.players[tab][spotted];

#pragma region Box
	Vector pos = player->origin();
	Vector head;
	pos.z -= 8.f;
	head.x = pos.x;
	head.y = pos.y;
	head.z = pos.z + 75.f;

	Vector posScreen = Helpers::world2Screen(gameScreenSize, pos, m);
	Vector headScreen = Helpers::world2Screen(gameScreenSize, head, m);

	ImVec2 pos2D = { posScreen.x, posScreen.y };
	ImVec2 head2D = { headScreen.x, headScreen.y };

	const float height = headScreen.y - posScreen.y;
	const float width = height / 4;

	const auto colorBox = config.box.gradientColor ? config.box.grandientTop.color : config.box.solid.color;
	const auto colorBox_ = config.box.gradientColor ? config.box.grandientBottom.color : config.box.solid.color;

	const auto colorFinal = ImGui::GetColorU32({ colorBox[0], colorBox[1], colorBox[2], 1.f });
	const auto colorFinal_ = ImGui::GetColorU32({ colorBox_[0], colorBox_[1], colorBox_[2], 1.f});

#pragma endregion Box
#pragma region Health Bar
	int health = player->health();

	const auto colorNumberHealth = ImGui::GetColorU32({ config.healthBar.showHealthNumber.color[0], config.healthBar.showHealthNumber.color[1], config.healthBar.showHealthNumber.color[2], 1.f });

	const auto colorHealthBar = ImGui::GetColorU32({ config.healthBar.solidColor.color[0], config.healthBar.solidColor.color[1], config.healthBar.solidColor.color[2], 1.f});
	const auto colorHealthBarMix = ImGui::GetColorU32({ (float)(1.f - health / 100.f), (health / 100.f), 0, 1.f });

	const auto colorHealthBarFinal = config.healthBar.solidColor.enabled ? colorHealthBar : colorHealthBarMix;
#pragma endregion Health Bar
#pragma region Player Name
	const auto& userInfoTable = csgo.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo);
	const auto& items = csgo.Read<uintptr_t>(csgo.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
	PlayerInfo pInfo = csgo.Read<PlayerInfo>(csgo.Read<uintptr_t>(items + 0x28 + (index * 0x34)));
	std::string name = pInfo.name;
#pragma endregion Player Name

#pragma region Skeleton
#pragma endregion Skeleton

	if (posScreen.z >= 0.01f) {
		if (config.box.enabled) drawBorderBox({gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y}, width, height, colorFinal, colorFinal_);
		if (config.healthBar.enabled) drawHealthBar({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, health, colorHealthBarFinal, colorNumberHealth, config.healthBar.showHealthNumber);
		if (config.name) drawPlayerName({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, name, color);
	}
}

void ESP::render() noexcept {
	if (!localPlayer) return;
	if (!cfg->esp.enabled) return;

	Matrix4x4 m = csgo.Read<Matrix4x4>(IClient.address + Offset::signatures::dwViewMatrix);
	for (int i = 1; i < 32; i++) {
		auto entity = getEntity(i);

		if (!entity)
			continue;

		if (entity->isDead())
			continue;

		if ((uintptr_t)entity == localPlayer.get())
			continue;

		renderPlayer(entity, i, m);
	}
}