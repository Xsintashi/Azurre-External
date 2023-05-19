#include "Minimap.h"
#include "SkinChanger.h"
#include <string>
#include <fstream>
#include <windows.h>
#include <psapi.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../GUI.h"
#include "../resources.h"
#include "../Texture.h"
#include "../../Lib/imgui/imgui.h"
#include "../../Lib/imgui/imgui_internal.h"
#include "../../Lib/imgui/ImGuiCustom.h"
#include "../../Lib/imgui/imgui_stdlib.h"
#include "../../Lib/imgui/imgui_impl_dx9.h"
#include "../../Lib/imgui/imgui_impl_win32.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../Lib/stb/stb_image.h"
#include "../SDK/Vector.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Entity.h"
#include "../SDK/LocalPlayer.h"

#include <d3dx9tex.h>
#include <d3dx9.h>

#pragma comment(lib, "d3dx9")

struct PNGTexture {
	template <std::size_t N>
	PNGTexture(const std::array<char, N>& png) noexcept : pngData{ png.data() }, pngDataSize{ png.size() } {}

	ImTextureID getTexture() const noexcept
	{
		if (!texture.get()) {
			int width, height;
			stbi_set_flip_vertically_on_load_thread(false);

			if (const auto data = stbi_load_from_memory((const stbi_uc*)pngData, pngDataSize, &width, &height, nullptr, STBI_rgb_alpha)) {
				texture.init(width, height, data);
				stbi_image_free(data);
			}
			else {
				assert(false);
			}
		}

		return texture.get();
	}

	void clearTexture() const noexcept { texture.clear(); }

private:
	const char* pngData;
	std::size_t pngDataSize;

	mutable Texture texture;
};

// Simple helper function to load an image into a DX9 texture with common settings
bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(GUI::device, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

bool LoadTextureFromMemory(const char* memory, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileInMemory(GUI::device, memory, sizeof(memory), &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

std::string parseString(const std::string& szBefore, const std::string& szSource) noexcept
{
	if (!szBefore.empty() && !szSource.empty() && (szSource.find(szBefore) != std::string::npos))
	{
		std::string t = strstr(szSource.c_str(), szBefore.c_str()); //-V522
		t.erase(0, szBefore.length());
		size_t firstLoc = t.find('\"', 0);
		size_t secondLoc = t.find('\"', firstLoc + 1);
		t = t.substr(firstLoc + 1, secondLoc - 3);
		return t;
	}
	else
		return "";
}

std::string map = "de_inferno"; // DEBUG
Vector mapOrigin;
float mapScale;
std::string mapRadar;

constexpr float iconSize = 8.f;

struct TextureInfo {
	int width;
	int height;
	PDIRECT3DTEXTURE9 data;
};

TextureInfo mapTexture;
static const PNGTexture c4Texture{ resource::C4 };
static const PNGTexture c4PlantedTexture{ resource::plantedC4 };
static const PNGTexture defuserTexture{ resource::defuser };
static const PNGTexture he{ resource::he };
static const PNGTexture decoy{ resource::decoy };
static const PNGTexture flash{ resource::flash };
static const PNGTexture molotov{ resource::molotov };
static const PNGTexture smoke{ resource::smoke };

const char* mapName() {
	static auto map = csgo.Read<std::array<char, 0x80>>(IClientState.address + Offset::signatures::dwClientState_Map);
	return map.data();
}

const char* gameDir() {
	static auto map = csgo.Read<std::array<char, 0x80>>(IEngine.address + Offset::signatures::dwGameDir);
	return map.data();
}

void Minimap::_() {
	
	mapRadar = std::string(gameDir()).append("\\resource\\overviews\\").append(mapName()).append("_radar.dds");

	std::ifstream isMapExist;
	isMapExist.open(mapRadar);

	if (!isMapExist.is_open())
		return;

	isMapExist.close();
	LoadTextureFromFile(mapRadar.c_str(), &mapTexture.data, &mapTexture.width, &mapTexture.height);

	std::string mapNameFata;
	mapNameFata = std::string(gameDir()).append("\\resource\\overviews\\").append(mapName()).append(".txt");
	std::ifstream ifs(mapNameFata.c_str());

	if (ifs.bad())
		return;

	std::string szInfo((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	if (szInfo.empty())
		return;

	mapOrigin.x = std::stof(parseString(("\"pos_x\""), szInfo));
	mapOrigin.y = std::stof(parseString(("\"pos_y\""), szInfo));
	mapScale = std::stof(parseString(("\"scale\""), szInfo));

}

void drawAngles(ImDrawList* drawList, const ImVec2& center, const ImVec2& pos, unsigned color, bool outline) noexcept
{
	const auto l = std::sqrtf(ImLengthSqr(pos));
	if (!l) return;
	const auto posNormalized = pos / l;

	constexpr int bigMulti = 15;

	const ImVec2 trianglePoints[] = {
		center + ImVec2{-0.25f * posNormalized.y, 0.25f * posNormalized.x} * bigMulti,
		center + ImVec2{0.25f * posNormalized.y, -0.25f * posNormalized.x} * bigMulti,
		center + ImVec2{0.5f * posNormalized.x, 0.5f * posNormalized.y} * bigMulti
	};

	drawList->AddConvexPolyFilled(trianglePoints, 3, color);
	if (outline)
		drawList->AddPolyline(trianglePoints, 3, color | IM_COL32_A_MASK, ImDrawFlags_Closed, 1.5f);
}

void renderBomb(Entity* C4, ImVec2 windowPos, unsigned int color) {
	const float originX = C4->origin().x / 2.f / mapScale;
	const float originY = C4->origin().y / 2.f / mapScale;

	float xOnTheMap = (originX + windowPos.x + 8.f + fabs(mapOrigin.x) / mapScale / 2);
	float yOnTheMap = (-originY + windowPos.y + 27.f + fabs(mapOrigin.y) / mapScale / 2);

	std::ostringstream ss;

	if (GetClassId((uintptr_t)C4) == ClassID::PlantedC4)
		ss << std::setprecision(3) << (C4->C4Blow() - globalVars->currentTime) << "sec";

	if (GetClassId((uintptr_t)C4) == ClassID::PlantedC4 && (C4->C4Blow() - globalVars->currentTime) < 0.f) return;

	ImGui::GetForegroundDrawList()->AddImage(GetClassId((uintptr_t)C4) == ClassID::PlantedC4 ? c4PlantedTexture.getTexture() : c4Texture.getTexture(), { xOnTheMap - iconSize,  yOnTheMap - iconSize }, { xOnTheMap + iconSize,  yOnTheMap + iconSize }, { 0, 0 }, { 1, 1 }, color); // 2 times smaller

	const auto centerText = ImGui::CalcTextSize(ss.str().c_str());
	ImGui::GetForegroundDrawList()->AddText({ xOnTheMap - centerText.x / 2, yOnTheMap }, IM_COL32(255, 255, 255, 255), ss.str().c_str());
}

void renderItem(Entity* item, ImVec2 windowPos, ImTextureID texture) {
	const float originX = item->origin().x / 2.f / mapScale;
	const float originY = item->origin().y / 2.f / mapScale;

	float xOnTheMap = (originX + windowPos.x + 8.f + fabs(mapOrigin.x) / mapScale / 2);
	float yOnTheMap = (-originY + windowPos.y + 27.f + fabs(mapOrigin.y) / mapScale / 2);

	ImGui::GetForegroundDrawList()->AddImage(texture, { xOnTheMap - iconSize,  yOnTheMap - iconSize }, { xOnTheMap + iconSize,  yOnTheMap + iconSize });

}

void renderPlayer(Entity* entity, ImVec2 windowPos, unsigned int color, int index) {

	const float originX = entity->origin().x / 2.f / mapScale;
	const float originY = entity->origin().y / 2.f / mapScale;

	float xOnTheMap = (originX + windowPos.x + 8.f + fabs(mapOrigin.x) / mapScale / 2);
	float yOnTheMap = (-originY + windowPos.y + 27.f + fabs(mapOrigin.y) / mapScale / 2);

	ImGui::GetForegroundDrawList()->AddCircleFilled({ xOnTheMap,  yOnTheMap }, 4.f, color, 0);
	//const auto centerText = ImGui::CalcTextSize(entityData[index].name.c_str()); //OUT OF VECTOR
	//ImGui::GetForegroundDrawList()->AddText({ xOnTheMap - centerText.x / 2, yOnTheMap + 4.f }, color, entityData[index].name.c_str());

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

	const float originX = entity->origin().x / 2.f / mapScale;
	const float originY = entity->origin().y / 2.f / mapScale;

	const float xOnTheMap = (originX + windowPos.x + 8.f + fabs(mapOrigin.x) / mapScale / 2);
	const float yOnTheMap = (-originY + windowPos.y + 27.f + fabs(mapOrigin.y) / mapScale / 2);
		  
	if(entity->velocity().length2D() < 1.f)
		ImGui::GetForegroundDrawList()->AddCircleFilled({ xOnTheMap,  yOnTheMap }, 16.f, color, 0);

	ImTextureID texture;
	switch (GetClassId((uintptr_t)entity)) {
		default:
		case ClassID::DecoyProjectile:
			texture = decoy.getTexture();
			break;
		case ClassID::MolotovProjectile:
		case ClassID::Inferno:
			texture = molotov.getTexture();
			break;
		case ClassID::BaseCSGrenadeProjectile: { //flash needed to fix
			texture = he.getTexture();
			break;
		}
		case ClassID::SmokeGrenadeProjectile:
			texture = smoke.getTexture();
			break;
	}
	ImGui::GetForegroundDrawList()->AddImage(texture, { xOnTheMap - iconSize,  yOnTheMap - iconSize }, { xOnTheMap + iconSize,  yOnTheMap + iconSize });
}

void Minimap::Render() {
	
	constexpr auto ctColor = IM_COL32(100, 200, 255, 255);
	constexpr auto ttColor = IM_COL32(255, 200, 0, 255);
	constexpr auto lpColor = IM_COL32(203, 223, 223, 255);

	ImGui::Begin( "Minimap", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize );
	
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImGui::Image(mapTexture.data, {512.f, 512.f}); // 2 times smaller

	renderPlayer((Entity*)localPlayer.get(), windowPos, lpColor, 0);
	for (int unsigned idx = 1; idx <= 512; idx++) {
		const auto& entity = getEntity(idx);
		if (!entity) continue;

		if (entity->origin() == Vector{ 0.f, 0.f, 0.f }) continue;

		switch (GetClassId((uintptr_t)entity)) {
			case ClassID::CSPlayer: {
				if (entity->isDead() || entity->dormant())
					continue;
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

	ImGui::End();
}