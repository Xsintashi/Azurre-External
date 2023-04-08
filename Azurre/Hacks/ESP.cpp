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

void drawBorderBox(ImVec2 pos, float width, float height, ImU32 color) {
	drawList->AddLine({pos.x - width, pos.y}, { pos.x + width, pos.y }, color); //TOP
	drawList->AddLine({pos.x - width, pos.y}, { pos.x - width, pos.y - height }, color); //LEFT
	drawList->AddLine({ pos.x + width, pos.y }, { pos.x + width, pos.y - height }, color); //TOP
	drawList->AddLine({ pos.x - width, pos.y - height }, { pos.x + width, pos.y - height }, color); //BOTTOM
}

void drawHealthBar(ImVec2 pos, float width, float height, int health, ImU32 color, ImU32 colorNumber) {
	const auto centerText = ImGui::CalcTextSize(std::to_string(health).c_str());
	drawList->AddLine({ pos.x + width + (width * .1f), pos.y - (100 - health) / 100.0f * height }, { pos.x + width + (width * .1f), pos.y - height }, color, 4.f); //LEFT
	drawList->AddText({ (pos.x + width + (width * .1f) - centerText.x / 2) + 1.f, (pos.y - (100 - health) / 100.0f * height) + 1.f }, color & IM_COL32_A_MASK, std::to_string(health).c_str());
	drawList->AddText({ pos.x + width + (width * .1f) - centerText.x / 2, pos.y - (100 - health) / 100.0f * height }, colorNumber, std::to_string(health).c_str());
}

void drawPlayerName(ImVec2 pos, float width, float height, std::string name, ImU32 color) {
	const auto centerText = ImGui::CalcTextSize(name.c_str());
	drawList->AddText({pos.x - centerText.x / 2, pos.y - 4.f + (height * .1f)}, color, name.c_str());
}

void renderPlayer(Entity* player, int index, Matrix4x4 m) {
	const ImU32 color = ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f });
	const ImU32 red = ImGui::GetColorU32({ 1.f, 0.f, 0.f, 1.f });
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

	float height = headScreen.y - posScreen.y;
	float width = height / 4;
#pragma endregion Box
#pragma region Health Bar
	int health = player->health();
	auto colorHealthBar = ImGui::GetColorU32({ cfg->esp.health.solidColor.color[0] * 255, cfg->esp.health.solidColor.color[1] * 255, cfg->esp.health.solidColor.color[2] * 255, 1.f });
	auto colorHealthBarNumber = ImGui::GetColorU32({ cfg->esp.health.showHealthNumber.color[0] * 255, cfg->esp.health.showHealthNumber.color[1] * 255, cfg->esp.health.showHealthNumber.color[2] * 255, 1.f });
#pragma endregion Health Bar
#pragma region Player Name
	const auto& userInfoTable = csgo.Read<uintptr_t>(IClientState.address + Offset::signatures::dwClientState_PlayerInfo);
	const auto& items = csgo.Read<uintptr_t>(csgo.Read<uintptr_t>(userInfoTable + 0x40) + 0xC);
	PlayerInfo pInfo = csgo.Read<PlayerInfo>(csgo.Read<uintptr_t>(items + 0x28 + (index * 0x34)));
	std::string name = pInfo.name;
#pragma endregion Player Name

	if (posScreen.z >= 0.01f) {
		if (cfg->esp.box) drawBorderBox({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, color);
		if (cfg->esp.health.enabled) drawHealthBar({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, health, colorHealthBar, colorHealthBarNumber);
		if (cfg->esp.playerNames) drawPlayerName({ gameScreenPos.x + headScreen.x, gameScreenPos.y + headScreen.y }, width, height, name, color);
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