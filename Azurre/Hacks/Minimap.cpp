#include "Minimap.h"

#include <string>
#include <fstream>
#include <windows.h>
#include <psapi.h>

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../../Lib/imgui/imgui.h"
#include "../../Lib/imgui/imgui_internal.h"
#include "../../Lib/imgui/ImGuiCustom.h"
#include "../../Lib/imgui/imgui_stdlib.h"
#include "../../Lib/imgui/imgui_impl_dx9.h"
#include "../../Lib/imgui/imgui_impl_win32.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../PNGTexture.h"
#include "../TextureManager.h"
#include "../resources.h"

#include "../SDK/Vector.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/WeaponID.h"

const PNGTexture c4Texture{ resource::C4 };
const PNGTexture c4PlantedTexture{ resource::plantedC4 };
const PNGTexture defuserTexture{ resource::defuser };
const PNGTexture he{ resource::he };
const PNGTexture decoy{ resource::decoy };
const PNGTexture flash{ resource::flash };
const PNGTexture molotov{ resource::molotov };
const PNGTexture smoke{ resource::smoke };

const PNGTexture ak47Texture{ resource::ak47 };
const PNGTexture augTexture{ resource::aug };
const PNGTexture awpTexture{ resource::awp };
const PNGTexture bizonTexture{ resource::bizon };
const PNGTexture cz75aTexture{ resource::cz75a };
const PNGTexture deagleTexture{ resource::deagle };
const PNGTexture eliteTexture{ resource::elite };
const PNGTexture famasTexture{ resource::famas };
const PNGTexture fivesevenTexture{ resource::fiveseven };
const PNGTexture g3sg1Texture{ resource::g3sg1 };
const PNGTexture galilarTexture{ resource::galilar };
const PNGTexture glockTexture{ resource::glock };
const PNGTexture m249Texture{ resource::m249 };
const PNGTexture m4a1Texture{ resource::m4a1 };
const PNGTexture m4a4Texture{ resource::m4a4 };
const PNGTexture mac10Texture{ resource::mac10 };
const PNGTexture mag7Texture{ resource::mag7 };
const PNGTexture mp5sdTexture{ resource::mp5sd };
const PNGTexture mp7Texture{ resource::mp7 };
const PNGTexture mp9Texture{ resource::mp9 };
const PNGTexture negevTexture{ resource::negev };
const PNGTexture novaTexture{ resource::nova };
const PNGTexture p2000Texture{ resource::p2000 };
const PNGTexture p250Texture{ resource::p250 };
const PNGTexture p90Texture{ resource::p90 };
const PNGTexture revolverTexture{ resource::revolver };
const PNGTexture sawedoffTexture{ resource::sawedoff };
const PNGTexture scar20Texture{ resource::scar20 };
const PNGTexture sg556Texture{ resource::sg556 };
const PNGTexture ssg08Texture{ resource::ssg08 };
const PNGTexture tec9Texture{ resource::tec9 };
const PNGTexture ump45Texture{ resource::ump45 };
const PNGTexture usp_silencerTexture{ resource::usp_silencer };
const PNGTexture xm1014Texture{ resource::xm1014 };
const PNGTexture ctDeathTexture{ resource::ctDeath };
const PNGTexture tDeathTexture{ resource::tDeath };


std::string parseString(const std::string& szBefore, const std::string& szSource) noexcept
{
	if (!szBefore.empty() && !szSource.empty() && (szSource.find(szBefore) != std::string::npos))
	{
		std::string t = strstr(szSource.c_str(), szBefore.c_str());
		t.erase(0, szBefore.length());
		size_t firstLoc = t.find('\"', 0);
		size_t secondLoc = t.find('\"', firstLoc + 1);
		t = t.substr(firstLoc + 1, secondLoc - 3);
		return t;
	}
	else
		return "";
}

Vector mapOriginStart;
Vector mapOriginEnd;
ImVec2 windowOffset;
float mapScale;
std::string mapRadar;
#define config cfg->m.minimap
constexpr float iconSize = 8.f;

struct TextureInfo {
	int width;
	int height;
	PDIRECT3DTEXTURE9 data;
};

TextureInfo mapTexture;

void Minimap::_() {
	
	mapRadar = std::string(gameDir).append("\\resource\\overviews\\").append(mapName).append("_radar.dds");

	std::ifstream isMapExist;
	isMapExist.open(mapRadar);

	if (!isMapExist.is_open())
		return;

	isMapExist.close();
	TextureManager::LoadTextureFromFile(mapRadar.c_str(), &mapTexture.data, &mapTexture.width, &mapTexture.height);

	std::string mapNameFata;
	mapNameFata = std::string(gameDir).append("\\resource\\overviews\\").append(mapName).append(".txt");
	std::ifstream ifs(mapNameFata.c_str());

	if (ifs.bad())
		return;

	std::string szInfo((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	if (szInfo.empty())
		return;

	mapOriginStart.x = std::stof(parseString(("\"pos_x\""), szInfo));
	mapOriginStart.y = std::stof(parseString(("\"pos_y\""), szInfo));
	mapScale = std::stof(parseString(("\"scale\""), szInfo));

	mapOriginEnd.x = 1024 * mapScale + mapOriginStart.x;
	mapOriginEnd.y = 1024 * mapScale + mapOriginStart.y;

	windowOffset = { 8.f, 8.f };
	if (!config.noTitleBar)
		windowOffset.y += 19.f;
}

void drawAngles(ImDrawList* drawList, const ImVec2& center, const ImVec2& pos, unsigned color, bool outline) noexcept
{
	const auto l = std::sqrtf(ImLengthSqr(pos));
	if (!l) return;
	const auto posNormalized = pos / l;

	constexpr int bigMulti = 15;

	const ImVec2 trianglePoints[] = {
		center + ImVec2{-0.25f * posNormalized.y * config.scale, 0.25f * posNormalized.x * config.scale} * bigMulti * config.scale,
		center + ImVec2{0.25f * posNormalized.y * config.scale, -0.25f * posNormalized.x * config.scale} * bigMulti * config.scale,
		center + ImVec2{0.5f * posNormalized.x * config.scale, 0.5f * posNormalized.y * config.scale} * bigMulti * config.scale
	};

	drawList->AddConvexPolyFilled(trianglePoints, 3, color);
	if (outline)
		drawList->AddPolyline(trianglePoints, 3, color | IM_COL32_A_MASK, ImDrawFlags_Closed, 1.5f);
}

void renderBomb(Entity* C4, ImVec2 windowPos, unsigned int color) {
	const float originX = C4->origin().x / 2.f / mapScale * config.scale;
	const float originY = C4->origin().y / 2.f / mapScale * config.scale;

	float xOnTheMap = (originX + windowPos.x + windowOffset.x + fabs(mapOriginStart.x) / mapScale / 2 * config.scale);
	float yOnTheMap = (-originY + windowPos.y + windowOffset.y + fabs(mapOriginStart.y) / mapScale / 2 * config.scale);

	std::ostringstream ss;

	if (GetClassId(C4) == ClassID::PlantedC4)
		ss << std::fixed << std::setprecision(2) << (C4->C4Blow() - globalVars->currentTime) << "sec";

	if (GetClassId(C4) == ClassID::PlantedC4 && (C4->C4Blow() - globalVars->currentTime) < 0.f || C4->bombDefused()) return;

	ImGui::GetForegroundDrawList()->AddImage(GetClassId(C4) == ClassID::PlantedC4 ? c4PlantedTexture.getTexture() : c4Texture.getTexture(), { xOnTheMap - iconSize * config.scale,  yOnTheMap - iconSize * config.scale }, { xOnTheMap + iconSize * config.scale,  yOnTheMap + iconSize * config.scale }, { 0, 0 }, { 1, 1 }, color); // 2 times smaller

	const auto centerText = ImGui::CalcTextSize(ss.str().c_str());
	ImGui::GetForegroundDrawList()->AddText({ xOnTheMap - centerText.x / 2, yOnTheMap }, IM_COL32(255, 255, 255, 255), ss.str().c_str());
}

void renderItem(Entity* item, ImVec2 windowPos, ImTextureID texture, float scaleForWeapons = 1.f) {
	const float originX = item->origin().x / 2.f / mapScale * config.scale;
	const float originY = item->origin().y / 2.f / mapScale * config.scale;

	float xOnTheMap = (originX + windowPos.x + windowOffset.x + fabs(mapOriginStart.x) / mapScale / 2 * config.scale);
	float yOnTheMap = (-originY + windowPos.y + windowOffset.y + fabs(mapOriginStart.y) / mapScale / 2 * config.scale);

	ImGui::GetForegroundDrawList()->AddImage(texture, { xOnTheMap - iconSize * config.scale * scaleForWeapons,  yOnTheMap - iconSize * config.scale * scaleForWeapons }, { xOnTheMap + iconSize * config.scale * scaleForWeapons,  yOnTheMap + iconSize * config.scale * scaleForWeapons });

}

void renderPlayer(Entity* entity, ImVec2 windowPos, unsigned int color, int index) {

	if (!config.showPlayers)
		return;

	if (entity->dormant() && config.showDormant)
		color -= IM_COL32(0, 0, 0, 196);
	else if (entity->dormant())
		return;

	const float originX = entity->origin().x / 2.f / mapScale * config.scale;
	const float originY = entity->origin().y / 2.f / mapScale * config.scale;

	float xOnTheMap = (originX + windowPos.x + windowOffset.x + fabs(mapOriginStart.x) / mapScale / 2 * config.scale);
	float yOnTheMap = (-originY + windowPos.y + windowOffset.y + fabs(mapOriginStart.y) / mapScale / 2 * config.scale);

	if (entity->lifeState() == LifeState::LIFE_DYING) {
		ImTextureID texture;
		switch (entity->teamNumber()) {
		default:
		case Team::TT:
			texture = tDeathTexture.getTexture();
			break;
		case Team::CT:
			texture = ctDeathTexture.getTexture();
			break;
		}
		ImGui::GetForegroundDrawList()->AddImage(texture, { xOnTheMap - iconSize * config.scale * 1.5f ,  yOnTheMap - iconSize * config.scale * 1.5f }, { xOnTheMap + iconSize * config.scale * 1.5f,  yOnTheMap + iconSize * config.scale * 1.5f });
	}

	if (entity->isDead())
		return;

	ImGui::GetForegroundDrawList()->AddCircleFilled({ xOnTheMap,  yOnTheMap }, 4.f * config.scale, color, 0);

	const auto& bombCarrier = csgo.Read<int>(IPlayerResource.address + Offset::netvars::m_iPlayerC4);
	if(index + 1 == bombCarrier) //Bro is carring bomb //Ghetto way
		ImGui::GetForegroundDrawList()->AddImage(c4Texture.getTexture(), { xOnTheMap - iconSize * config.scale,  yOnTheMap - iconSize * config.scale }, { xOnTheMap + iconSize * config.scale,  yOnTheMap + iconSize * config.scale }, { 0, 0 }, { 1, 1 }); // 2 times smaller

	const auto eyeAngles = entity->eyeAngleY();
	const auto yaw = Helpers::deg2rad(-eyeAngles - 45.f);
	auto x = std::cos(yaw) - std::sin(yaw);
	auto y = std::cos(yaw) + std::sin(yaw);
	if (const auto len = std::sqrt(x * x + y * y); len != 0.0f) {
		x /= len;
		y /= len;
	}
	drawAngles(ImGui::GetForegroundDrawList(), { xOnTheMap, yOnTheMap }, { x, y }, color, false);
}

void renderNades(Entity* entity, ImVec2 windowPos, unsigned int color) {

	const float originX = entity->origin().x / 2.f / mapScale * config.scale;
	const float originY = entity->origin().y / 2.f / mapScale * config.scale;

	const float xOnTheMap = (originX + windowPos.x + windowOffset.x + fabs(mapOriginStart.x) / mapScale / 2 * config.scale);
	const float yOnTheMap = (-originY + windowPos.y + windowOffset.y + fabs(mapOriginStart.y) / mapScale / 2 * config.scale);
		  
	if(entity->velocity().length2D() < 1.f)
		ImGui::GetForegroundDrawList()->AddCircleFilled({ xOnTheMap,  yOnTheMap }, 16.f, color, 0);

	ImTextureID texture;
	switch (GetClassId(entity)) {
		default:
		case ClassID::Flashbang:
			texture = flash.getTexture();
		break;
		case ClassID::DecoyProjectile:
			texture = decoy.getTexture();
			break;
		case ClassID::MolotovProjectile:
		case ClassID::Inferno:
			texture = molotov.getTexture();
			break;
		case ClassID::BaseCSGrenadeProjectile:
			texture = he.getTexture();
			break;
		case ClassID::SmokeGrenadeProjectile:
			texture = smoke.getTexture();
			break;
	}
	ImGui::GetForegroundDrawList()->AddImage(texture, { xOnTheMap - iconSize * config.scale,  yOnTheMap - iconSize * config.scale }, { xOnTheMap + iconSize * config.scale,  yOnTheMap + iconSize * config.scale });
}

void Minimap::Render() { //Render Thread
	
	if (!showMenu && !cfg->m.minimap.hotkey.isActive() && cfg->m.minimap.hotkey.isSet())
		return;

	constexpr auto ctColor = IM_COL32(100, 200, 255, 255);
	constexpr auto ttColor = IM_COL32(255, 200, 0, 255);
	constexpr auto lpColor = IM_COL32(203, 223, 223, 255);

	int windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing;

	if (!showMenu)
		windowFlags |= ImGuiWindowFlags_NoInputs;
	if (config.noTitleBar)
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
	if (config.noBackground)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	if (cfg->m.minimap.pos != ImVec2{}) {
		ImGui::SetNextWindowPos(cfg->m.minimap.pos);
		cfg->m.minimap.pos = {};
	}
	ImGui::Begin("Minimap", nullptr, windowFlags);
	
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImGui::Image(mapTexture.data, {512.f * config.scale, 512.f * config.scale }); // 2 times smaller

	for (int unsigned idx = 1; idx <= 1024; idx++) {
		const auto& entity = getEntity(idx);
		if (!entity) continue;

		if (entity->origin() == Vector{ 0.f, 0.f, 0.f }) continue;

		const short& weaponID = entity->getWeaponID();

		if (entity->isWeapon() && config.showWeapons) {

			switch (weaponID) {
			default: break;
			case WeaponID::Glock:
				renderItem(entity, windowPos, glockTexture.getTexture(), 1.5f); break;
			case WeaponID::Usp_s:
				renderItem(entity, windowPos, usp_silencerTexture.getTexture(), 1.5f); break;
			case WeaponID::Hkp2000:
				renderItem(entity, windowPos, p2000Texture.getTexture(), 1.5f); break;
			case WeaponID::Elite:
				renderItem(entity, windowPos, eliteTexture.getTexture(), 1.5f); break;
			case WeaponID::P250:
				renderItem(entity, windowPos, p250Texture.getTexture(), 1.5f); break;
			case WeaponID::Tec9:
				renderItem(entity, windowPos, tec9Texture.getTexture(), 1.5f); break;
			case WeaponID::Cz75a:
				renderItem(entity, windowPos, cz75aTexture.getTexture(), 1.5f); break;
			case WeaponID::Fiveseven:
				renderItem(entity, windowPos, fivesevenTexture.getTexture(), 1.5f); break;
			case WeaponID::Deagle:
				renderItem(entity, windowPos, deagleTexture.getTexture(), 1.5f); break;
			case WeaponID::Revolver:
				renderItem(entity, windowPos, revolverTexture.getTexture(), 1.5f); break;
			case WeaponID::Mac10:
				renderItem(entity, windowPos, mac10Texture.getTexture(), 1.5f); break;
			case WeaponID::Mp9:
				renderItem(entity, windowPos, mp9Texture.getTexture(), 1.5f); break;
			case WeaponID::Mp7:
				renderItem(entity, windowPos, mp7Texture.getTexture(), 1.5f); break;
			case WeaponID::Mp5sd:
				renderItem(entity, windowPos, mp5sdTexture.getTexture(), 1.5f); break;
			case WeaponID::Ump45:
				renderItem(entity, windowPos, ump45Texture.getTexture(), 1.5f); break;
			case WeaponID::P90:
				renderItem(entity, windowPos, p90Texture.getTexture(), 1.5f); break;
			case WeaponID::Bizon:
				renderItem(entity, windowPos, bizonTexture.getTexture(), 1.5f); break;
			case WeaponID::Famas:
				renderItem(entity, windowPos, famasTexture.getTexture(), 1.5f); break;
			case WeaponID::GalilAr:
				renderItem(entity, windowPos, galilarTexture.getTexture(), 1.5f); break;
			case WeaponID::Ak47:
				renderItem(entity, windowPos, ak47Texture.getTexture(), 1.5f); break;
			case WeaponID::M4A1:
				renderItem(entity, windowPos, m4a1Texture.getTexture(), 1.5f); break;
			case WeaponID::M4a1_s:
				renderItem(entity, windowPos, m4a4Texture.getTexture(), 1.5f); break;
			case WeaponID::Ssg08:
				renderItem(entity, windowPos, ssg08Texture.getTexture(), 1.5f); break;
			case WeaponID::Sg553:
				renderItem(entity, windowPos, sg556Texture.getTexture(), 1.5f); break;
			case WeaponID::Aug:
				renderItem(entity, windowPos, augTexture.getTexture(), 1.5f); break;
			case WeaponID::Awp:
				renderItem(entity, windowPos, awpTexture.getTexture(), 1.5f); break;
			case WeaponID::Scar20:
				renderItem(entity, windowPos, scar20Texture.getTexture(), 1.5f); break;
			case WeaponID::G3SG1:
				renderItem(entity, windowPos, g3sg1Texture.getTexture(), 1.5f); break;
			case WeaponID::Nova:
				renderItem(entity, windowPos, novaTexture.getTexture(), 1.5f); break;
			case WeaponID::Xm1014:
				renderItem(entity, windowPos, xm1014Texture.getTexture(), 1.5f); break;
			case WeaponID::Sawedoff:
				renderItem(entity, windowPos, sawedoffTexture.getTexture(), 1.5f); break;
			case WeaponID::Mag7:
				renderItem(entity, windowPos, mag7Texture.getTexture(), 1.5f); break;
			case WeaponID::M249:
				renderItem(entity, windowPos, m249Texture.getTexture(), 1.5f); break;
			case WeaponID::Negev:
				renderItem(entity, windowPos, negevTexture.getTexture(), 1.5f); break;
			}
		}
		switch(GetClassId(entity)) {
			case ClassID::CSPlayer: {
				renderPlayer(entity, windowPos, entity->teamNumber() == Team::TT ? ttColor : ctColor, idx);
				break;
			}
			case ClassID::PlantedC4:
			case ClassID::C4: {
				renderBomb(entity, windowPos, IM_COL32(255, 255, 255, 255));
				break;
			}
			case ClassID::EconEntity: { //Defuse Kit
				renderItem(entity, windowPos, defuserTexture.getTexture());
				break;
			}
			case ClassID::DecoyProjectile:
			case ClassID::MolotovProjectile:
			case ClassID::BaseCSGrenadeProjectile: {
				renderNades(entity, windowPos, 0);
				break;
			}
			case ClassID::SmokeGrenadeProjectile: {
				renderNades(entity, windowPos, IM_COL32(127, 127, 127, 195));
				break;
			}
			case ClassID::Inferno: {
				renderNades(entity, windowPos, IM_COL32(255, 127, 39, 195));
				break;
			}
		}
	}
	renderPlayer((Entity*)localPlayer.get(), windowPos, lpColor, 0);
	ImGui::End();
}