#include "Misc.h"

#include "../Config.h"
#include "../GUI.h"

#include "../SDK/Convar.h"
#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Matrix.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/Vector.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "../SDK/UserInterface.h"
#include "../../Lib/imgui/ImGuiCustom.h"

void Misc::bunnyHop() noexcept {
	if (!cfg->m.bhop) return;

	if (!localPlayer) return;

	if (localPlayer->isDead()) return;

	const auto flags = localPlayer->flags();

	if (GetAsyncKeyState(VK_SPACE) && !cfg->restrictions){
		(flags & FlagsState::ONGROUND) ?
		mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceJump, 6) :
		mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceJump, 4);
		return;
	}

	if (cfg->restrictions && GetAsyncKeyState(VK_SPACE)) {
		(flags & FlagsState::ONGROUND) ?
			clientCmd("+jump") :
			clientCmd("-jump");
	}
}

void Misc::fakeLag() noexcept {
	if (!localPlayer) return;

	if (localPlayer->isDead()) return;

	if (cfg->restrictions) return; //RPM ONLY

	const auto& sendPacket = mem.Read<BYTE>(IEngine.address + Offset::signatures::dwbSendPackets);
	const auto& chokedPackets = mem.Read<int>(IClientState.address + Offset::signatures::clientstate_choked_commands);
	int choke = 0;

	if (cfg->m.fakeLag.limit < 1) return;

	const float speed = localPlayer->velocity().length2D() >= 15.0f ? localPlayer->velocity().length2D() : 0.0f;

	switch (cfg->m.fakeLag.type) {
	case 0: //Static
		choke = cfg->m.fakeLag.limit;
		break;
	case 1: //Adaptive
		choke = std::clamp(static_cast<int>(std::ceilf(64 / (speed * globalVars->intervalPerTick))), 1, cfg->m.fakeLag.limit);
		break;
	case 2: // Random
		srand(static_cast<unsigned int>(time(NULL)));
		choke = rand() % cfg->m.fakeLag.limit + 1;
		break;
	}

	choke = std::clamp(choke, 0, 16);
	mem.Write<byte>(IEngine.address + Offset::signatures::dwbSendPackets, chokedPackets >= choke);

}

void Misc::hitMarkerSound() noexcept
{
	if (!localPlayer || gameState != 6)
		return;

	static int currentHitsCounter = localPlayer->totalHits();
	static float lastHitTime = 0.0f;

	if (currentHitsCounter != localPlayer->totalHits() && localPlayer->totalHits() != 0) {

		std::string out = std::string(gameDir).append("\\").append(cfg->m.hitSound);

		std::ifstream isFileExist;
		isFileExist.open(out);

		currentHitsCounter = localPlayer->totalHits();
		lastHitTime = globalVars->realTime;

		if (isFileExist.is_open()) {
			PlaySound(out.c_str(), NULL, SND_ASYNC);
			isFileExist.close();
		}
	}
	if (!cfg->m.hitMarker.type)
		return;

	if (lastHitTime + cfg->m.hitMarker.time < globalVars->realTime)
		return;

	ImVec2 mid = gameScreenSize / 2.f + gameScreenPos;

	switch (cfg->m.hitMarker.type) {
		case 1: {
			auto color = IM_COL32(
				static_cast<int>(cfg->m.hitMarker.color.color[0] * 255.f),
				static_cast<int>(cfg->m.hitMarker.color.color[1] * 255.f),
				static_cast<int>(cfg->m.hitMarker.color.color[2] * 255.f),
				255);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x - 10, mid.y - 10 }, { mid.x - 4, mid.y - 4 }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x + 10.5f, mid.y - 10.5f }, { mid.x + 4.5f, mid.y - 4.5f }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x + 10.5f, mid.y + 10.5f }, { mid.x + 4.5f, mid.y + 4.5f }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x - 10, mid.y + 10 }, { mid.x - 4, mid.y + 4 }, color);
			break;
		}
		case 2: {
			auto color = IM_COL32(
				static_cast<int>(cfg->m.hitMarker.color.color[0] * 255.f),
				static_cast<int>(cfg->m.hitMarker.color.color[1] * 255.f),
				static_cast<int>(cfg->m.hitMarker.color.color[2] * 255.f),
				static_cast<int>(Helpers::lerp(fabsf(lastHitTime + cfg->m.hitMarker.time - globalVars->realTime) / cfg->m.hitMarker.time + FLT_EPSILON, 0.0f, 255.0f)));
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x - 10, mid.y - 10 }, { mid.x - 4, mid.y - 4 }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x + 10.5f, mid.y - 10.5f }, { mid.x + 4.5f, mid.y - 4.5f }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x + 10.5f, mid.y + 10.5f }, { mid.x + 4.5f, mid.y + 4.5f }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x - 10, mid.y + 10 }, { mid.x - 4, mid.y + 4 }, color);
			break;
		}
	}
	
}

void Misc::killMarkerSound() noexcept {
	if (!localPlayer || gameState != 6)
		return;
	const auto& kills = gameData.playerResource.kills[localPlayerIndex];
	static int currentKillsCounter = gameData.playerResource.kills[localPlayerIndex];
	static float lastKillTime = 0.0f;

	if (currentKillsCounter != kills && kills > 0) {

		std::string out = std::string(gameDir).append("\\").append(cfg->m.killSound);

		std::ifstream isFileExist;
		isFileExist.open(out);

		currentKillsCounter = kills;
		lastKillTime = globalVars->realTime;

		if (isFileExist.is_open()) {
			PlaySound(out.c_str(), NULL, SND_ASYNC);
			isFileExist.close();
		}
	}
	if (!cfg->m.killMarker.type)
		return;

	if (lastKillTime + cfg->m.killMarker.time < globalVars->realTime)
		return;

	ImVec2 mid = gameScreenSize / 2.f + gameScreenPos;

	switch (cfg->m.killMarker.type) {
		case 1: {
			auto color = IM_COL32(
				static_cast<int>(cfg->m.killMarker.color.color[0] * 255.f),
				static_cast<int>(cfg->m.killMarker.color.color[1] * 255.f),
				static_cast<int>(cfg->m.killMarker.color.color[2] * 255.f),
				255);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x - 10, mid.y - 10 }, { mid.x - 4, mid.y - 4 }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x + 10.5f, mid.y - 10.5f }, { mid.x + 4.5f, mid.y - 4.5f }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x + 10.5f, mid.y + 10.5f }, { mid.x + 4.5f, mid.y + 4.5f }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x - 10, mid.y + 10 }, { mid.x - 4, mid.y + 4 }, color);
			break;
		}
		case 2: {
			auto color = IM_COL32(
				static_cast<int>(cfg->m.killMarker.color.color[0] * 255.f),
				static_cast<int>(cfg->m.killMarker.color.color[1] * 255.f),
				static_cast<int>(cfg->m.killMarker.color.color[2] * 255.f),
				static_cast<int>(Helpers::lerp(fabsf(lastKillTime + cfg->m.killMarker.time - globalVars->realTime) / cfg->m.killMarker.time + FLT_EPSILON, 0.0f, 255.0f)));
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x - 10, mid.y - 10 }, { mid.x - 4, mid.y - 4 }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x + 10.5f, mid.y - 10.5f }, { mid.x + 4.5f, mid.y - 4.5f }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x + 10.5f, mid.y + 10.5f }, { mid.x + 4.5f, mid.y + 4.5f }, color);
			ImGui::GetBackgroundDrawList()->AddLine({ mid.x - 10, mid.y + 10 }, { mid.x - 4, mid.y + 4 }, color);
			break;
		}
	}

}

void Misc::indicators() noexcept {

	if (cfg->m.indicators.pos != ImVec2{}) {
		ImGui::SetNextWindowPos(cfg->m.indicators.pos);
		cfg->m.indicators.pos = {};
	}

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
	if (!GUI::isRunning)
		windowFlags |= ImGuiWindowFlags_NoInputs;

	if (cfg->m.indicators.noTitleBar)
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
	if (cfg->m.indicators.noBackground)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowSize({ 192.0f, 0.0f });
	ImGui::Begin("Indicators", &cfg->m.indicators.enabled, windowFlags);

	if (gameState != ConnectionState::FullyConnected || !localPlayer->isValid() || localPlayer->isDead()) {
		float sinuses[5];
		for (int i = 0; i < 5; i++) {
			sinuses[i] = sin((i + 1) * globalVars->realTime / 2.f);
		}

		if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 0)) {
			int chokedPackets = mem.Read<int>(IClientState.address + Offset::signatures::clientstate_choked_commands);
			ImGui::TextUnformatted("Choked packets");
			ImGui::progressBarFullWidth(sinuses[0], 5.f);
		}
		if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 1)) {
			ImGui::TextUnformatted("Height");
			ImGui::progressBarFullWidth(sinuses[1], 5.f);
		}
		if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 2)) {
			ImGui::TextUnformatted("Velocity");
			float velocity = localPlayer->velocity().length2D() / 300.f;
			ImGui::progressBarFullWidth(sinuses[2], 5.f);
		}
		if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 3)) {
			ImGui::TextUnformatted("Slowdown");
			ImGui::progressBarFullWidth(sinuses[3], 5.f);
		}
		if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 4)) {
			ImGui::TextUnformatted("Stamina");
			ImGui::progressBarFullWidth(sinuses[4], 5.f);
		}
		ImGui::End();
		return;
	}

	if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 0)){
		int chokedPackets = mem.Read<int>(IClientState.address + Offset::signatures::clientstate_choked_commands);
		ImGui::TextUnformatted("Choked packets");
		ImGui::progressBarFullWidth(static_cast<float>(chokedPackets / 16), 5.f);
	}
	if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 1)) {
		ImGui::TextUnformatted("Height");
		ImGui::progressBarFullWidth((localPlayer->bonePosition(8).z - localPlayer->origin().z - PLAYER_EYE_HEIGHT_CROUCH) / (PLAYER_EYE_HEIGHT - PLAYER_EYE_HEIGHT_CROUCH), 5.f);
	}
	if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 2)) {
		ImGui::TextUnformatted("Velocity");
		float velocity = localPlayer->velocity().length2D() / 300.f;
		ImGui::progressBarFullWidth(std::clamp(velocity, 0.f, 1.f), 5.f);
	}
	if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 3)) {
		ImGui::TextUnformatted("Slowdown");
		ImGui::progressBarFullWidth(1.f - (localPlayer->velocityModifier() / 100), 5.f);
	}
	if (Helpers::getByteFromBytewise(cfg->m.indicators.bytewise, 4)) {
		ImGui::TextUnformatted("Stamina");
		ImGui::progressBarFullWidth(1.f - (localPlayer->stamina() / 80), 5.f);
	}
	ImGui::End();
}

void Misc::changeWindowTitle(bool restore) noexcept {
	
	if (cfg->restrictions) return;
	
	if (restore) {
		SetWindowTextA(IConsole, "Counter-Strike: Global Offensive - Direct3D 9");
		return;
	}

	std::stringstream title;
	title << "Counter-Strike: Global Offensive - Azurre External 0.1";
#if defined(_DEBUG)
	title << " | ";
	title << " Client: 0x" << std::hex << IClient.address;
	title << " Engine: 0x" << std::hex << IEngine.address;
	title << " LocalPlayer: 0x" << std::hex << localPlayer.get();
#endif
	std::string titleConverted = title.str();
	SetWindowTextA(IConsole, titleConverted.c_str());
}

void Misc::forceReload(bool onKey) noexcept {

	if (!onKey || GetAsyncKeyState(VK_END) && !cfg->restrictions) {
		for (int i = 0; i < 8; i++) {
			equipment[i] = 0;
		}
		mem.Write<std::int32_t>(IClientState.address + 0x174, -1);
		mem.Write<byte>(IEngine.address + Offset::signatures::dwbSendPackets, true);
		changeWindowTitle();
	}
	if (GetAsyncKeyState(VK_HOME))
		mem.Write<byte>(IEngine.address + Offset::signatures::dwbSendPackets, true);
}

void Misc::showKeybinds() noexcept {

	bool anyActive = (cfg->t.enabled && cfg->t.hotkey.canShowKeybind()) || (cfg->v.thirdPerson && cfg->v.thirdPersonKey.canShowKeybind()) || (cfg->a.enabled && cfg->a.hotkey.canShowKeybind()) || (cfg->m.playerList.enabled && cfg->m.playerList.hotkey.canShowKeybind()) || (cfg->m.minimap.enabled && cfg->m.minimap.hotkey.canShowKeybind());

	if (!anyActive && !showMenu)
		return;

	if (cfg->m.keybinds.pos != ImVec2{}) {
		ImGui::SetNextWindowPos(cfg->m.keybinds.pos);
		cfg->m.keybinds.pos = {};
	}

	ImGui::SetNextWindowSize({ 250.f, 0.f }, ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints({ 250.f, 0.f }, { 250.f, FLT_MAX });

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoFocusOnAppearing;
	if (!showMenu)
		windowFlags |= ImGuiWindowFlags_NoInputs;

	if (cfg->m.keybinds.noTitleBar)
		windowFlags |= ImGuiWindowFlags_NoTitleBar;

	if (cfg->m.keybinds.noBackground)
		windowFlags |= ImGuiWindowFlags_NoBackground;
	ImGui::Begin("Keybind list", nullptr, windowFlags);
	cfg->t.hotkey.showKeybind();
	cfg->a.hotkey.showKeybind();
	cfg->v.thirdPersonKey.showKeybind();
	cfg->m.playerList.hotkey.showKeybind();
	cfg->m.minimap.hotkey.showKeybind();
	ImGui::End();
}

void Misc::modifyConVars(bool reset) noexcept {

	if (!localPlayer) return;

	if (gameState != 6) return;

	if (cfg->restrictions) return; //RPM ONLY

	ConVar sky{ "r_3dsky"};
	ConVar shadow{ "cl_csm_enabled"};
	ConVar grenade{ "cl_grenadepreview"};
	ConVar panoramaBlur{ "@panorama_disable_blur"};
	ConVar skyname{ "sv_skyname"};
	ConVar particles{ IClient.address + Offset::signatures::convar_r_drawparticles };
	const static int skynameFlags = skyname.getFlags();

	if (reset) {
		sky.setValue(1);
		shadow.setValue(1);
		grenade.setValue(0);
		particles.setValue(1);
		return;
	}

	static bool doOnce = true;
	if (doOnce) {
		int flags = skyname.getFlags();
		flags &= ~(CVarFlags::REPLICATED);
		flags &= ~(CVarFlags::CHEAT);
		skyname.flags(flags);
		doOnce = false;
	}

	sky.setValue(!cfg->v.no3DSky);
	shadow.setValue(!cfg->v.noShadows);
	grenade.setValue(cfg->m.grenadeTrajectory);
	particles.setValue(!cfg->v.noParticles);
	panoramaBlur.setValue(cfg->v.noPanoramaBlur);

	static int tempSkybox = 0;

	if (cfg->v.skybox && tempSkybox != cfg->v.skybox) {
		static int flags = skynameFlags;
		flags &= ~(CVarFlags::REPLICATED);
		flags &= ~(CVarFlags::CHEAT);
		skyname.flags(flags);
		std::string cmd = "sv_skyname " + std::string(skyboxList[cfg->v.skybox]);
		clientCmd(cmd.c_str());
		skyname.flags(skynameFlags);
		tempSkybox = cfg->v.skybox;
	}

}

void Misc::drawOffscreenEnemies() noexcept
{
	if (!cfg->m.offscreenEnemies.toggle.enabled)
		return;

	const auto &angles = mem.Read<Vector>(IClientState.address + Offset::signatures::dwClientState_ViewAngles);
	const auto yaw = Helpers::deg2rad(angles.y);

	for (auto& player : gameData.playerData) {
		if (player.dormant || !player.entity->isAlive() || player.entity->isSameTeam())
			continue;

		const auto positionDiff = localPlayer->origin() - player.entity->origin();

		auto x = std::cos(yaw) * positionDiff.y - std::sin(yaw) * positionDiff.x;
		auto y = std::cos(yaw) * positionDiff.x + std::sin(yaw) * positionDiff.y;
		if (const auto len = std::sqrt(x * x + y * y); len != 0.0f) {
			x /= len;
			y /= len;
		}

		ImVec2 mid = gameScreenSize / 2.f + gameScreenPos;

		const auto pos = mid + ImVec2{ x, y } * (cfg->m.offscreenEnemies.radius * 10);
		const auto trianglePos = pos + ImVec2{ x, y } * cfg->m.offscreenEnemies.radius;

		const auto white = Helpers::calculateColor(255, 255, 255, 255);
		const auto background = Helpers::calculateColor(0, 0, 0, 80);
		const auto color = Helpers::calculateColor(cfg->m.offscreenEnemies.toggle);

		const ImVec2 trianglePoints[]{
			trianglePos + ImVec2{  0.4f * y, -0.4f * x } * cfg->m.offscreenEnemies.size,
			trianglePos + ImVec2{  1.0f * x,  1.0f * y } * cfg->m.offscreenEnemies.size,
			trianglePos + ImVec2{ -0.4f * y,  0.4f * x } * cfg->m.offscreenEnemies.size
		};

		ImGui::GetBackgroundDrawList()->AddConvexPolyFilled(trianglePoints, 3, color);
	}
}

void Misc::spectatorList() noexcept {


	ImGui::SetNextWindowSize({ 200.0f, 0.f });

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing;
	if (!showMenu)
		windowFlags |= ImGuiWindowFlags_NoInputs;
	if (cfg->m.spectatorList.noTitleBar)
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
	if (cfg->m.spectatorList.noBackground)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	if (cfg->m.spectatorList.pos != ImVec2{}) {
		ImGui::SetNextWindowPos(cfg->m.spectatorList.pos);
		cfg->m.spectatorList.pos = {};
	}

	if (showMenu) {
		ImGui::Begin("Spectator List", nullptr, windowFlags);
		ImGui::TextWrapped("VALVE | 3rd");
		ImGui::TextWrapped("Azurre | 1rd");
		ImGui::TextWrapped("GOTV | Freecam");
		ImGui::End();
		return;
	}

	if (gameState != ConnectionState::FullyConnected) return;

	if (!localPlayer) return;

	if (gameData.observerData.empty()) return;

	ImGui::Begin("Spectator List", nullptr, windowFlags);

	for (auto& player : gameData.observerData) {
		
		const char* obsMode;

		switch (player.obsMode) {
			case ObsMode::Deathcam:
				obsMode = "Deathcam";
				break;
			case ObsMode::Freezecam:
				obsMode = "Freezecam";
				break;
			case ObsMode::Fixed:
				obsMode = "Fixed";
				break;
			case ObsMode::InEye:
				obsMode = "1st";
				break;
			case ObsMode::Chase:
				obsMode = "3rd";
				break;
			case ObsMode::Roaming:
				obsMode = "Freecam";
				break;
			default:
				obsMode = "";
		}
		ImGui::TextWrapped("%s | %s", player.name.c_str(), obsMode);
	}
	ImGui::End();
}

void Misc::bombTimer() noexcept {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoFocusOnAppearing;
	if (!showMenu)
		windowFlags |= ImGuiWindowFlags_NoInputs;

	if (cfg->m.bombTimer.noTitleBar)
		windowFlags |= ImGuiWindowFlags_NoTitleBar;

	if (cfg->m.bombTimer.pos != ImVec2{}) {
		ImGui::SetNextWindowPos(cfg->m.bombTimer.pos);
		cfg->m.bombTimer.pos = {};
	}
	
	const auto& plantedC4 = gameData.plantedC4;

	if (gameData.plantedC4->bombDefused())
		gameData.plantedC4 = nullptr;

	if (gameData.plantedC4 != nullptr || showMenu){
		ImGui::Begin("Bomb Timer", nullptr, windowFlags);
		std::ostringstream ss; ss << "Bomb on " << (!plantedC4->bombSite() ? 'A' : 'B') << " : " << std::fixed << std::showpoint << std::setprecision(3) << (std::max)(plantedC4->C4Blow() - globalVars->currentTime, 0.0f) << " s";
		ImGui::textUnformattedCentered(ss.str().c_str());

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Helpers::calculateColor(cfg->m.bombTimer.barColor));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::progressBarFullWidth((plantedC4->C4Blow() - globalVars->currentTime) / plantedC4->timerLength(), 5.0f);

		int bombDefuseris = plantedC4->bombDefuser();

		if (bombDefuseris != -1) {
			const bool canDefuse = plantedC4->C4Blow() >= plantedC4->defuseCountDown();

			if (localPlayer->isDefusing()) {
				if (canDefuse) {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
					ImGui::textUnformattedCentered("You can defuse!");
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
					ImGui::textUnformattedCentered("You can not defuse!");
				}
				ImGui::PopStyleColor();
			}
			else {
				std::ostringstream ss; ss << (gameData.defusingPlayerName.empty() ? "Player" : gameData.defusingPlayerName) << " is defusing: " << std::fixed << std::showpoint << std::setprecision(3) << (std::max)(plantedC4->defuseCountDown() - globalVars->currentTime, 0.0f) << " s";
				ImGui::textUnformattedCentered(ss.str().c_str());

				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, canDefuse ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
				ImGui::progressBarFullWidth((plantedC4->defuseCountDown() - globalVars->currentTime) / plantedC4->defuseLength(), 5.0f);
				ImGui::PopStyleColor();
			}
		}

		ImGui::PopStyleColor(2);
		ImGui::End();
	}
}

void drawCrosshair(ImVec2 pos, float length, float gap, float thickness, float outlineTickness, bool dot, bool outline, bool TStyle, unsigned int color, unsigned int colorOutline) {

	if (outline) {
		if (dot)
			ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - outlineTickness / 2.f, pos.y - outlineTickness / 2.f }, { pos.x + outlineTickness / 2.f , pos.y + outlineTickness / 2.f }, colorOutline);

		if (!TStyle)
			ImGui::GetBackgroundDrawList()->AddLine({ pos.x , pos.y - length - outlineTickness }, { pos.x, pos.y - gap + outlineTickness }, colorOutline, outlineTickness + thickness); //TOP

		ImGui::GetBackgroundDrawList()->AddLine({ pos.x , pos.y + length + outlineTickness }, { pos.x, pos.y + gap - outlineTickness }, colorOutline, outlineTickness + thickness); //BOTTOM
		ImGui::GetBackgroundDrawList()->AddLine({ pos.x - length - outlineTickness , pos.y }, { pos.x - gap + outlineTickness, pos.y }, colorOutline, outlineTickness + thickness); //LEFT
		ImGui::GetBackgroundDrawList()->AddLine({ pos.x + length + outlineTickness, pos.y }, { pos.x + gap - outlineTickness, pos.y}, colorOutline, outlineTickness + thickness); //RIGHT
	}

	if (dot)
		ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - thickness / 2.f, pos.y - thickness / 2.f }, { pos.x + thickness / 2.f , pos.y + thickness / 2.f }, color);

	if(!TStyle)
		ImGui::GetBackgroundDrawList()->AddLine({ pos.x , pos.y - length }, { pos.x, pos.y - gap }, color, thickness); //TOP

	ImGui::GetBackgroundDrawList()->AddLine({ pos.x , pos.y + length }, { pos.x, pos.y + gap }, color, thickness); //BOTTOM
	ImGui::GetBackgroundDrawList()->AddLine({ pos.x - length , pos.y }, { pos.x - gap, pos.y }, color, thickness); //LEFT
	ImGui::GetBackgroundDrawList()->AddLine({ pos.x + length, pos.y }, { pos.x + gap, pos.y }, color, thickness); //RIGHT
}

void Misc::crosshairs() noexcept {

	if (!localPlayer)
		return;

	if (gameState != ConnectionState::FullyConnected)
		return;

	if (localPlayer->isDead())
		return;

	ImVec2 mid = gameScreenSize / 2.f + gameScreenPos;

	if ((cfg->m.sniperCrosshair.enabled && showMenu) || (cfg->m.sniperCrosshair.enabled && !localPlayer->isScoped() && localPlayer->getActiveWeapon()->isWeaponRifleSniper())) {
		auto& settings = cfg->m.sniperCrosshair;
		drawCrosshair(mid, settings.length, settings.gap, settings.thickness, settings.outlineThickness, settings.dot, settings.outline.enabled, settings.TStyle, Helpers::calculateColor(settings.color), Helpers::calculateColor(settings.outline));
	}

	if ((cfg->m.recoilCrosshair.enabled && showMenu) || (cfg->m.recoilCrosshair.enabled && localPlayer->shotsFired() && !localPlayer->isScoped() && !localPlayer->getActiveWeapon()->isWeaponRifleSniper())) {
		auto& settings = cfg->m.recoilCrosshair;
		Vector aimPunch = localPlayer->aimPunch() / 2.f;
		drawCrosshair({ mid.x - (screenSize.x / 90.f * aimPunch.y), mid.y + (screenSize.x / 90.f * aimPunch.x) }, settings.length, settings.gap, settings.thickness, settings.outlineThickness, settings.dot, settings.outline.enabled, settings.TStyle, Helpers::calculateColor(settings.color), Helpers::calculateColor(settings.outline));
	}

}

void Misc::fastStop() noexcept	{

	if (!cfg->m.autoStop) return;

	if (!localPlayer) return;

	if (localPlayer->isDead()) return;

	if (cfg->restrictions) return;

	auto wKey = GetAsyncKeyState(0x57);
	auto aKey = GetAsyncKeyState(0x41);
	auto sKey = GetAsyncKeyState(0x53);
	auto dKey = GetAsyncKeyState(0x44);
	const float velocity = localPlayer->velocity().length2D();
	Vector finalVector = Helpers::calculateRealAngles();
	if (!wKey && !aKey && !sKey && !dKey && velocity >= 30.f && (localPlayer->flags() & 1)) {
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

