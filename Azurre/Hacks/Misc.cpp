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
		(flags & (1 << 0)) ?
		mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceJump, 6) :
		mem.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceJump, 4);
		return;
	}

	if (cfg->restrictions && GetAsyncKeyState(VK_SPACE)) {
		(flags & (1 << 0)) ?
			usr0::SendConsoleCommand("+jump") :
			usr0::SendConsoleCommand("-jump");
	}
}

void Misc::fakeLag() noexcept {
	if (!localPlayer) return;

	if (localPlayer->isDead()) return;

	if (cfg->restrictions) return; //RPM ONLY

	const auto& sendPacket = mem.Read<BYTE>(IEngine.address + Offset::signatures::dwbSendPackets);
	const auto& chokedPackets = mem.Read<int>(IClientState.address + Offset::signatures::clientstate_choked_commands);
	int choke = 0;

	if (!cfg->m.fakeLag.enabled)return;

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
		mem.Write<std::int32_t>(IClientState.address + 0x174, -1);
		changeWindowTitle();
	}
}

void Misc::showKeybinds() noexcept
{
	if (!cfg->m.keybinds.enabled)
		return;

	bool anyActive = (cfg->t.enabled && cfg->t.hotkey.canShowKeybind()) || (cfg->v.thirdPerson && cfg->v.thirdPersonKey.canShowKeybind()) || (cfg->a.enabled && cfg->a.hotkey.canShowKeybind());

	if (!anyActive)
		return;

	if (cfg->m.keybinds.pos != ImVec2{}) {
		ImGui::SetNextWindowPos(cfg->m.keybinds.pos);
		cfg->m.keybinds.pos = {};
	}

	ImGui::SetNextWindowSize({ 250.f, 0.f }, ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints({ 250.f, 0.f }, { 250.f, FLT_MAX });

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
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
	ImGui::End();
}

void Misc::modifyConVars(bool reset) noexcept { //dont really work F

	if (!localPlayer) return;

	if (gameState != 6) return;

	if (cfg->restrictions) return; //RPM ONLY

	ConVar sky{ IClient.address + Offset::cvars::r_3dsky };
	ConVar shadow{ IClient.address + Offset::cvars::cl_csm_enabled };
	ConVar grenade{ IClient.address + Offset::cvars::cl_grenadepreview};
	ConVar skyname{ IClient.address + Offset::cvars::sv_skyname };
	ConVar particles{ IClient.address + Offset::cvars::r_drawparticles };
	const static int skynameFlags = skyname.getFlags();

	if (reset) {
		sky.setValue(1);
		shadow.setValue(1);
		grenade.setValue(0);
		particles.setValue(1);
		return;
	}

	static bool doOnce = true; // for tests
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

	static int tempSkybox = 0;

	if (cfg->v.skybox && tempSkybox != cfg->v.skybox) {
		static int flags = skynameFlags;
		flags &= ~(CVarFlags::REPLICATED);
		flags &= ~(CVarFlags::CHEAT);
		skyname.flags(flags);
		std::string cmd = "sv_skyname " + std::string(skyboxList[cfg->v.skybox]);
		usr0::SendConsoleCommand(cmd.c_str());
		skyname.flags(skynameFlags);
		tempSkybox = cfg->v.skybox;
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

	if (!showMenu) {
		ImGui::SetNextWindowBgAlpha(0.3f);
	}

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

