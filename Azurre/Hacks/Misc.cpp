#include "Misc.h"

#include "../Config.h"
#include "../GUI.h"

#include "../SDK/Convar.h"
#include "../SDK/Entity.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/Vector.h"

#include <chrono>
#include <fstream>
#include <algorithm>
#include "../SDK/UserInterface.h"

void Misc::bunnyHop() noexcept {
	if (!cfg->m.bhop) return;

	if (!localPlayer) return;

	if (localPlayer->isDead()) return;

	const auto flags = localPlayer->flags();

	if (GetAsyncKeyState(VK_SPACE) && !cfg->restrictions){
		(flags & (1 << 0)) ?
		csgo.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceJump, 6) :
		csgo.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceJump, 4);
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

	const auto& sendPacket = csgo.Read<BYTE>(IEngine.address + Offset::signatures::dwbSendPackets);
	const auto& chokedPackets = csgo.Read<int>(IClientState.address + Offset::signatures::clientstate_choked_commands);
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
	csgo.Write<byte>(IEngine.address + Offset::signatures::dwbSendPackets, chokedPackets >= choke);

}

void Misc::HitNKillSound() noexcept
{
	if (!localPlayer || gameState != 6)
		return;

	static int currentHitsCounter = localPlayer->totalHits();

	if (currentHitsCounter != localPlayer->totalHits() && localPlayer->totalHits() != 0){

		std::string out = std::string(gameDir).append("\\").append(cfg->m.hitSound);

		std::ifstream isFileExist;
		isFileExist.open(out);

		currentHitsCounter = localPlayer->totalHits();

		if (!isFileExist.is_open())
			return;

		isFileExist.close();
		
		PlaySound(out.c_str(), NULL, SND_ASYNC);
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
		csgo.Write<std::int32_t>(IClientState.address + 0x174, -1);
		changeWindowTitle();
	}
}

void Misc::entityLoop() noexcept {

	if (!localPlayer) return;

	for (unsigned int i = 1; i <= 32; i++) {
		const auto& entity = getEntity(i);
		if (!entity) continue;

		if (entity->isSameTeam() || entity == (Entity*)localPlayer.get()) continue;

		if(cfg->m.radarHack && !cfg->restrictions)
			csgo.Write<bool>((uintptr_t)entity + Offset::netvars::m_bSpotted, true);
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

void Misc::modifyClasses() noexcept {

	if (!localPlayer) return;

	if (cfg->restrictions) return;

	for (int i = 0; i < 512; i++)
	{
		int entity = csgo.Read<int>(IClient.address + Offset::signatures::dwEntityList + i * 0x10);
		if (!entity) continue;
		if (cfg->m.fixTablet && GetClassId(entity) == ClassID::Tablet)
			csgo.Write<bool>(entity + Offset::netvars::m_bTabletReceptionIsBlocked, false);
		if (cfg->v.customPostProcessing.enabled && GetClassId((int)entity) == ClassID::ToneMapController) {
			csgo.Write<bool>(entity + Offset::netvars::m_bUseCustomBloomScale, cfg->v.customPostProcessing.enabled);
			csgo.Write<bool>(entity + Offset::netvars::m_bUseCustomAutoExposureMax, cfg->v.customPostProcessing.enabled);
			csgo.Write<bool>(entity + Offset::netvars::m_bUseCustomAutoExposureMin, cfg->v.customPostProcessing.enabled);

			float bloomScale = cfg->v.customPostProcessing.bloomScale * 0.01f;
			float worldExposure = cfg->v.customPostProcessing.worldExposure * 0.001f;

			csgo.Write<float>(entity + Offset::netvars::m_flCustomBloomScale, bloomScale);
			csgo.Write<float>(entity + Offset::netvars::m_flCustomAutoExposureMax, worldExposure);
			csgo.Write<float>(entity + Offset::netvars::m_flCustomAutoExposureMin, worldExposure);
		}
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
			csgo.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceBackward, 6);
		if (finalVector.x <= -20) // BACK, SO GO FRONT
			csgo.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceForward, 6);
		if (finalVector.y >= 20) // RIGHT, SO GO LEFT
			csgo.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceLeft, 6);
		if (finalVector.y <= -20) // LEFT, SO GO RIGHT
			csgo.Write<std::uintptr_t>(IClient.address + Offset::signatures::dwForceRight, 6);;
	}
}