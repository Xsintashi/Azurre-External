#include <fstream>
#include <Windows.h>
#include <shellapi.h>
#include <ShlObj.h>

#include "../Lib/nlohmann/json.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Lib/imgui/imgui.h"
#include "../Lib/imgui/imgui_internal.h"

#include "Junk.h"
#include "Config.h"
#include "Helpers.h"

#include "Hacks/SkinChanger.h"
#include "Hacks/Misc.h"
#include "Hacks/Clantag.h"
#include "GUI.h"

Config::Config() noexcept {
    if (PWSTR pathToDocuments; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pathToDocuments))) {
        path = pathToDocuments; $$$
        CoTaskMemFree(pathToDocuments); $$$
    }

    path /= "Azurre External"; $$$
    listConfigs(); $$$
    
    LOGFONTW logfont; $$$
    logfont.lfCharSet = ANSI_CHARSET; $$$
    logfont.lfPitchAndFamily = DEFAULT_PITCH; $$$
    logfont.lfFaceName[0] = L'\0'; $$$
}

void removeEmptyObjects(json& j) noexcept {
    for (auto it = j.begin(); it != j.end();) {
        auto& val = it.value(); $$$
            if (val.is_object() || val.is_array())
                removeEmptyObjects(val); $$$
                if (val.empty() && !j.is_array()) {
                    it = j.erase(it); $$$
                }
                else {
                    ++it; $$$
                }
    }
}

#pragma region Read

static void from_json(const json& j, ImVec2& v) {
    read(j, "X", v.x); $$$
    read(j, "Y", v.y); $$$
}

static void from_json(const json& j, ColorToggle& ct) {
    from_json(j, static_cast<Color4&>(ct)); $$$
    read(j, "Enabled", ct.enabled); $$$
}

static void from_json(const json& j, Color3& c) {
    read(j, "Color", c.color); $$$
    read(j, "Rainbow", c.rainbow); $$$
    read(j, "Rainbow Speed", c.rainbowSpeed); $$$
}

static void from_json(const json& j, ColorToggle3& ct) {
    from_json(j, static_cast<Color3&>(ct)); $$$
    read(j, "Enabled", ct.enabled); $$$
}

static void from_json(const json& j, ColorToggleRounding& ctr) {
    from_json(j, static_cast<ColorToggle&>(ctr)); $$$

    read(j, "Rounding", ctr.rounding); $$$
}

static void from_json(const json& j, ColorToggleOutline& cto) {
    from_json(j, static_cast<ColorToggle&>(cto)); $$$

    read(j, "Outline", cto.outline); $$$
}

static void from_json(const json& j, ColorToggleThickness& ctt) {
    from_json(j, static_cast<ColorToggle&>(ctt)); $$$

    read(j, "Thickness", ctt.thickness); $$$
}

static void from_json(const json& j, ColorToggleThicknessRounding& cttr)
{
    from_json(j, static_cast<ColorToggleRounding&>(cttr)); $$$

    read(j, "Thickness", cttr.thickness); $$$
}

static void from_json(const json& j, Config::WeaponAimbot& c) {
    read(j, "Enabled Weapon", c.enable); $$$
    read(j, "Auto Shot", c.autoShot); $$$
    read(j, "Auto Stop", c.autoStop); $$$
    read(j, "Force Accuracy", c.forceAccuracy); $$$
    read(j, "Visible Only", c.visibleOnly); $$$
    read(j, "Ignore Flash", c.ignoreFlash); $$$
    read(j, "Bone", c.bone); $$$
    read(j, "Fov", c.fov); $$$
    read(j, "Deadzone", c.deadzone); $$$
    read(j, "Smooth", c.smooth); $$$
    read(j, "Priority", c.priority); $$$
}

static void from_json(const json& j, Config::AimbotConfig& c) {
    read(j, "Enabled", c.enabledAimbot); $$$
    read(j, "Hotkey", c.hotkey); $$$
    read(j, "Disable on spectator", c.disableWhileBeingSpectated); $$$
    read(j, "RCS", c.rcs); $$$
    read<value_t::object>(j, "Draw Fov", c.drawFov); $$$
    read<value_t::object>(j, "Draw Deadzone", c.drawDeadzone); $$$
    read(j, "Weapons", c.weapons); $$$
}

static void from_json(const json& j, Config::ClanTagConfig::CustomClanTag& c) {
    read<value_t::string>(j, "Prefix", c.prefix); $$$
    read<value_t::string>(j, "Tag", c.tag); $$$
    read<value_t::string>(j, "TeamTag", c.teamTag); $$$
    read<value_t::string>(j, "Postfix", c.postfix); $$$
    read(j, "Hide Name", c.hideName); $$$
    read(j, "Type", c.type); $$$
    read(j, "Speed", c.speed); $$$
}

static void from_json(const json& j, Config::ClanTagConfig& c) {
    read(j, "Mode", c.mode); $$$
    read<value_t::object>(j, "Custom", c.custom); $$$
}

static void from_json(const json& j, Config::ChamsConfig& c) {
    read(j, "Enabled", c.enabled); $$$
    read(j, "Brightness", c.brightness); $$$
    read<value_t::object>(j, "Ally", c.ally); $$$
    read<value_t::object>(j, "Enemy", c.enemy); $$$
}

static void from_json(const json& j, Config::DiscordConfig& c) {
    read(j, "Enabled", c.enabled); $$$
    read(j, "Show Map", c.showMap); $$$
    read(j, "Show Match Time", c.showMatchTime); $$$
    read(j, "State", c.state); $$$
    read(j, "Details", c.details); $$$
}

static void from_json(const json& j, Other& o) {
    read<value_t::object>(j, "Names", o.names); $$$
    read<value_t::object>(j, "Lines", o.lines); $$$
}

static void from_json(const json& j, Box& b) {
    read(j, "Enabled", b.enabled); $$$
    read(j, "Gradient Color", b.gradientColor); $$$
    read<value_t::object>(j, "Solid Color", b.solid); $$$
    read<value_t::object>(j, "Top Color", b.grandientTop); $$$
    read<value_t::object>(j, "Bottom Color", b.grandientBottom); $$$
}

static void from_json(const json& j, HealthBar& o) {
    read(j, "Enabled", o.enabled); $$$
    read<value_t::object>(j, "Solid Color", o.solidColor); $$$
    read<value_t::object>(j, "Health Number", o.showHealthNumber); $$$
}

static void from_json(const json& j, Player& p) {
    read(j, "Enabled", p.enabled); $$$
    read<value_t::object>(j, "Other", p.other); $$$
    read<value_t::object>(j, "Box", p.box); $$$
    read<value_t::object>(j, "Head Box", p.headBox); $$$
    read<value_t::object>(j, "HealthBar", p.healthBar); $$$
    read<value_t::object>(j, "Weapons", p.weapons); $$$
    read<value_t::object>(j, "Skeleton", p.skeleton); $$$
    read(j, "Flags", p.flags); $$$
}

static void from_json(const json& j, Weapons& p) {
    read(j, "Enabled", p.enabled); $$$
    read<value_t::object>(j, "Other", p.other); $$$
    read<value_t::object>(j, "Box", p.box); $$$
}

static void from_json(const json& j, Projectiles& p) {
    read(j, "Enabled", p.enabled); $$$
    read<value_t::object>(j, "Names", p.names); $$$
    read<value_t::object>(j, "Box", p.box); $$$
}

static void from_json(const json& j, Config::ESPConfig& e) {
    read(j, "Mode", e.mode); $$$
    read(j, "Enable Defuse Kits on CT Site", e.onCT); $$$
    read(j, "Players", e.players); $$$
    read(j, "Weapons", e.weapons); $$$
    read(j, "Projectiles", e.projectiles); $$$
    read(j, "Others", e.others); $$$
    read(j, "Danger Zone", e.dangerzone); $$$
}

static void from_json(const json& j, Crosshair& o) {
    read(j, "Enabled", o.enabled); $$$
    read<value_t::object>(j, "Color", o.color); $$$
    read(j, "Length", o.length); $$$
    read(j, "Gap", o.gap); $$$
    read(j, "Thickness", o.thickness); $$$
    read(j, "Dot", o.dot); $$$
    read<value_t::object>(j, "Outline", o.outline); $$$
    read(j, "Outline Thickness", o.outlineThickness); $$$
    read(j, "TStyle", o.TStyle); $$$
}

static void from_json(const json& j, Config::GlowConfig& c) {
    read(j, "Enabled", c.enabled); $$$
    read<value_t::object>(j, "Ally", c.ally); $$$
    read<value_t::object>(j, "Enemy", c.enemy); $$$
}

static void from_json(const json& j, Config::GuiConfig::BackgroundEffect& c) {
    read<value_t::object>(j, "Color", c.color); $$$
    read(j, "Number", c.number); $$$
    read(j, "Max Distance", c.maxDistance); $$$
    read(j, "Thickness", c.lineThickness); $$$
}

static void from_json(const json& j, Config::GuiConfig& c) {
    read(j, "AntiAliasing", c.antiAliasing); $$$
    read(j, "Window Border", c.windowBorder); $$$
    read(j, "Center Title", c.centerTitle); $$$
    read(j, "Frame Border", c.frameBorder); $$$
    read(j, "Round Border", c.roundBorder); $$$
    read(j, "Menu Colors", c.menuColors); $$$
    read<value_t::object>(j, "Background Effect", c.backgroundEffect); $$$
}

static void from_json(const json& j, Config::MiscConfig::Minimap& c) {
    read(j, "Enabled", c.enabled); $$$
    read(j, "Show Players", c.showPlayers); $$$
    read(j, "Show Dormant", c.showDormant); $$$
    read(j, "Show Weapons", c.showWeapons); $$$
    read(j, "Show Grenades", c.showGrenades); $$$
    read(j, "No Title", c.noTitleBar); $$$
    read(j, "No Background", c.noBackground); $$$
    read(j, "Scale", c.scale); $$$
    read(j, "Hotkey", c.hotkey); $$$
    read(j, "Color Scheme", c.colorScheme); $$$
    read<value_t::object>(j, "Pos", c.pos); $$$
}

static void from_json(const json& j, Config::MiscConfig::FakeLag& c) {
    read(j, "Limit", c.limit); $$$
    read(j, "Type", c.type); $$$
}

static void from_json(const json& j, Config::MiscConfig::BombTimer& c) {
    read(j, "Enabled", c.enabled); $$$
    read(j, "No Title Bar", c.noTitleBar); $$$
    read<value_t::object>(j, "Color", c.barColor); $$$
    read<value_t::object>(j, "Pos", c.pos); $$$
}

static void from_json(const json& j, Config::MiscConfig::PlayerList& c) {
    read(j, "Enabled", c.enabled); $$$
    read(j, "No Title Bar", c.noTitleBar); $$$
    read(j, "Hotkey", c.hotkey); $$$
    read(j, "Hide LocalPlayer", c.hideLocalPlayer); $$$
    read(j, "Hide Dormant", c.hideDormant); $$$
    read(j, "Sort", c.sort); $$$
    read<value_t::object>(j, "LocalPlayer Color", c.localPlayerColor); $$$
    read<value_t::object>(j, "Pos", c.pos); $$$
}

static void from_json(const json& j, Config::MiscConfig::KeyBindsList& c) {
    read(j, "Enabled", c.enabled); $$$
    read(j, "No Title Bar", c.noTitleBar); $$$
    read(j, "No Background", c.noBackground); $$$
    read<value_t::object>(j, "Pos", c.pos); $$$
}

static void from_json(const json& j, Marker& c) {
    read(j, "Type", c.type); $$$
    read(j, "Time", c.time); $$$
    read<value_t::object>(j, "Color", c.color); $$$
}

static void from_json(const json& j, Config::MiscConfig::SpectatorList& sl)
{
    read(j, "Enabled", sl.enabled); $$$
    read(j, "No Title Bar", sl.noTitleBar); $$$
    read(j, "No Background", sl.noBackground); $$$
    read<value_t::object>(j, "Pos", sl.pos); $$$
}

static void from_json(const json& j, Config::MiscConfig::OffscreenEnemies& oe)
{
    read<value_t::object>(j, "Color", oe.toggle); $$$
    read(j, "Radius", oe.radius); $$$
    read(j, "Size", oe.size); $$$
}

static void from_json(const json& j, Config::MiscConfig::Indicators& in)
{
    read(j, "Enabled", in.enabled); $$$
    read(j, "No Title Bar", in.noTitleBar); $$$
    read(j, "No Background", in.noBackground); $$$
    read(j, "Flags", in.bytewise); $$$
    read<value_t::object>(j, "Pos", in.pos); $$$
}

static void from_json(const json& j, Config::MiscConfig::SlowWalk& in) {
    read(j, "Custom Speed", in.slowSpeed); $$$
    read(j, "Mode", in.slowWalkMode); $$$
    read(j, "Hotkey", in.hotkey); $$$
}

static void from_json(const json& j, Config::MiscConfig& c) {
    read(j, "Menu Key", c.menuKey); $$$
    read(j, "Bunny Hop", c.bhop); $$$
    read(j, "Exo Hop", c.exojumpHop); $$$
    read(j, "Auto Stop", c.autoStop); $$$
    read(j, "Fix Tablet Signal", c.fixTablet); $$$
    read(j, "Engine Radar", c.radarHack); $$$
    read(j, "Grenade Trajectory", c.grenadeTrajectory); $$$
    read<value_t::object>(j, "Recoil Crosshair", c.recoilCrosshair); $$$
    read<value_t::object>(j, "Sniper Crosshaird", c.sniperCrosshair); $$$
    read<value_t::string>(j, "Hit Sound", c.hitSound); $$$
    read<value_t::string>(j, "Kill Sound", c.killSound); $$$
    read<value_t::string>(j, "Kill Say", c.killSay); $$$
    read<value_t::object>(j, "Hit Marker", c.hitMarker); $$$
    read<value_t::object>(j, "Player List", c.playerList); $$$
    read<value_t::object>(j, "Keybinds List", c.keybinds); $$$
    read<value_t::object>(j, "Minimap", c.minimap); $$$
    read<value_t::object>(j, "Offscreen Enemies", c.offscreenEnemies); $$$
    read<value_t::object>(j, "Fake Lag", c.fakeLag); $$$
    read<value_t::object>(j, "Spectator list", c.spectatorList); $$$
    read<value_t::object>(j, "Bomb Timer", c.bombTimer); $$$
    read<value_t::object>(j, "Indicators", c.indicators); $$$
    read<value_t::object>(j, "Slow Walk", c.slowWalk); $$$
}

static void from_json(const json& j, Config::SkinChangerConfig& c) {
    read(j, "Definition index", c.skinID); $$$
    read(j, "Quality", c.quality); $$$
    read(j, "Paint Kit", c.skinID); $$$
    read(j, "Seed", c.seed); $$$
    read(j, "StatTrak", c.statTrak); $$$
    read(j, "Wear", c.wear); $$$
    read(j, "Custom name", c.nameTag, sizeof(c.nameTag)); $$$
}

static void from_json(const json& j, Config::ChangerConfig& c) {
    read(j, "CT Knife", c.CTKnife); $$$
    read(j, "T Knife", c.TTKnife); $$$
    read(j, "CT Agent", c.CTAgent); $$$
    read(j, "T Agent", c.TTAgent); $$$
}
static void from_json(const json& j, Config::TriggerBotConfig& c) {
    read(j, "Enabled", c.enabled); $$$
    read(j, "Hotkey", c.hotkey); $$$
    read(j, "Delay", c.delay); $$$
    read(j, "Burst", c.burst); $$$
}

static void from_json(const json& j, Config::VisualsConfig::CustomPostProcessing& c) {
    read(j, "Enabled", c.enabled); $$$
    read(j, "Bloom Scale", c.bloomScale); $$$
    read(j, "World Exposures", c.worldExposure); $$$
}

static void from_json(const json& j, Config::VisualsConfig::Trails& c) {
    read<value_t::object>(j, "Color", c.color); $$$
    read(j, "Size", c.size); $$$
    read(j, "Thickness", c.thickness); $$$
    read(j, "Rainbow Type", c.rainbowType); $$$
}

static void from_json(const json& j, Config::VisualsConfig& c) {
    read(j, "No Allies", c.noAllies); $$$
    read(j, "No Shadows", c.noShadows); $$$
    read(j, "No 3DSky", c.no3DSky); $$$
    read(j, "No Panorama Blur", c.noPanoramaBlur); $$$
    read(j, "Fov", c.fov); $$$
    read(j, "Flash Reduction", c.flashReduction); $$$
    read(j, "Third Person", c.thirdPerson); $$$
    read(j, "Third Person Key", c.thirdPersonKey); $$$
    read<value_t::object>(j, "Custom PostProcessing", c.customPostProcessing); $$$
    read<value_t::object>(j, "Trails", c.trails); $$$
}

void Config::load(size_t id, bool incremental) noexcept
{
    load((const char8_t*)configs[id].c_str(), incremental); $$$
}

void Config::load(const char8_t* name, bool incremental) noexcept
{
    json j; $$$

    if (std::ifstream in{ path / name }; in.good()) {
        j = json::parse(in, nullptr, false); $$$
        if (j.is_discarded())
            return; $$$
    }
    else {
        return; $$$
    }

    if (!incremental)
        reset(); $$$

    read(j, "Restrictions", restrictions); $$$
    read<value_t::array>(j, "Friendly SteamIDs", friendlySteamIDs); $$$

    read<value_t::object>(j, "Aimbot", a); $$$
    read<value_t::object>(j, "Chams", c); $$$
    read<value_t::object>(j, "ClanTag", clanTag); $$$
    read<value_t::object>(j, "Discord", d); $$$
    read<value_t::object>(j, "ESP", esp); $$$
    read<value_t::object>(j, "Glow", g); $$$
    read<value_t::object>(j, "Misc", m); $$$
    read<value_t::array> (j, "Skins", s); $$$
    read<value_t::object>(j, "Changer", ch); $$$
    read<value_t::object>(j, "TriggerBot", t); $$$
    read<value_t::object>(j, "Visuals", v); $$$
    read<value_t::object>(j, "GUI", u); $$$

    GUI::updateColors(); $$$

    Misc::forceReload(); $$$
}

#pragma endregion

#pragma region Write

static void to_json(json& j, const ImVec2& o, const ImVec2& dummy = {}) {
    WRITE("X", x); $$$
    WRITE("Y", y); $$$
}

static void to_json(json& j, const ColorToggle& o, const ColorToggle& dummy = {}) {
    to_json(j, static_cast<const Color4&>(o), dummy); $$$
    WRITE("Enabled", enabled); $$$
}

static void to_json(json& j, const Color3& o, const Color3& dummy = {}) {
    WRITE("Color", color); $$$
    WRITE("Rainbow", rainbow); $$$
    WRITE("Rainbow Speed", rainbowSpeed); $$$
}

static void to_json(json& j, const ColorToggle3& o, const ColorToggle3& dummy = {}) {
    to_json(j, static_cast<const Color3&>(o), dummy); $$$
    WRITE("Enabled", enabled); $$$
}

static void to_json(json& j, const ColorToggleRounding& o, const ColorToggleRounding& dummy = {}) {
    to_json(j, static_cast<const ColorToggle&>(o), dummy); $$$
    WRITE("Rounding", rounding); $$$
}

static void to_json(json& j, const ColorToggleThickness& o, const ColorToggleThickness& dummy = {}) {
    to_json(j, static_cast<const ColorToggle&>(o), dummy); $$$
    WRITE("Thickness", thickness); $$$
}

static void to_json(json& j, const ColorToggleOutline& o, const ColorToggleOutline& dummy = {}) {
    to_json(j, static_cast<const ColorToggle&>(o), dummy); $$$
    WRITE("Outline", outline); $$$
}

static void to_json(json& j, const ColorToggleThicknessRounding& o, const ColorToggleThicknessRounding& dummy = {}) {
    to_json(j, static_cast<const ColorToggleRounding&>(o), dummy); $$$
    WRITE("Thickness", thickness); $$$
}

static void to_json(json& j, const Config::WeaponAimbot& o, const Config::WeaponAimbot& dummy = {}) {
    WRITE("Enabled Weapon", enable); $$$
    WRITE("Auto Shot", autoShot); $$$
    WRITE("Auto Stop", autoStop); $$$
    WRITE("Force Accuracy", forceAccuracy); $$$
    WRITE("Visible Only", visibleOnly); $$$
    WRITE("Ignore Flash", ignoreFlash); $$$
    WRITE("Bone", bone); $$$
    WRITE("Fov", fov); $$$
    WRITE("Deadzone", deadzone); $$$
    WRITE("Smooth", smooth); $$$
    WRITE("Priority", priority); $$$
}

static void to_json(json& j, const Config::AimbotConfig& o, const Config::AimbotConfig dummy = {}) {
    WRITE("Enabled", enabledAimbot); $$$
    WRITE("Hotkey", hotkey); $$$
    WRITE("Disable on spectator", disableWhileBeingSpectated); $$$
    WRITE("RCS", rcs); $$$
    WRITE("Draw Fov", drawFov); $$$
    WRITE("Draw Deadzone", drawDeadzone); $$$
    j["Weapons"] = o.weapons; $$$
}

static void to_json(json& j, const Config::ClanTagConfig::CustomClanTag& o, const Config::ClanTagConfig::CustomClanTag& dummy) {
    WRITE("Prefix", prefix); $$$
    WRITE("Tag", tag); $$$
    WRITE("TeamTag", teamTag); $$$
    WRITE("Postfix", postfix); $$$
    WRITE("Hide Name", hideName); $$$
    WRITE("Type", type); $$$
    WRITE("Speed", speed); $$$
}

static void to_json(json& j, const Config::ClanTagConfig& o, const Config::ClanTagConfig dummy = {}){
    WRITE("Mode", mode); $$$
    WRITE("Custom", custom); $$$
}

static void to_json(json& j, const Config::ChamsConfig& o, const Config::ChamsConfig dummy = {} ) {
    
    WRITE("Enabled", enabled); $$$
    WRITE("Brightness", brightness); $$$
    WRITE("Ally", ally); $$$
    WRITE("Enemy", enemy); $$$
}

static void to_json(json& j, const Config::DiscordConfig& o, const Config::DiscordConfig dummy = {} ) {
    
    WRITE("Enabled", enabled); $$$
    WRITE("Show Map", showMap); $$$
    WRITE("Show Match Time", showMatchTime); $$$
    WRITE("State", state); $$$
    WRITE("Details", details); $$$
}

static void to_json(json& j, const Other& o, const Other& dummy = {}) {
    WRITE("Names", names); $$$
    WRITE("Lines", lines); $$$
}

static void to_json(json& j, const Box& o, const Box& dummy = {}) {
    WRITE("Enabled", enabled); $$$
    WRITE("Gradient Color", gradientColor); $$$
    WRITE("Solid Color", solid); $$$
    WRITE("Top Color", grandientTop); $$$
    WRITE("Bottom Color", grandientBottom); $$$
}

static void to_json(json& j, const HealthBar& o, const HealthBar& dummy = {}) {
    WRITE("Enabled", enabled); $$$
    WRITE("Solid Color", solidColor); $$$
    WRITE("Health Number", showHealthNumber); $$$
}

static void to_json(json& j, const Player& o, const Player& dummy = {}) {
    WRITE("Enabled", enabled); $$$
    WRITE("Other", other); $$$
    WRITE("Box", box); $$$
    WRITE("Head Box", headBox); $$$
    WRITE("HealthBar", healthBar); $$$
    WRITE("Weapons", weapons); $$$
    WRITE("Flags", flags); $$$
    WRITE("Skeleton", skeleton); $$$
}

static void to_json(json& j, const Weapons& o, const Weapons& dummy = {}) {
    WRITE("Enabled", enabled); $$$
    WRITE("Other", other); $$$
    WRITE("Box", box); $$$
}

static void to_json(json& j, const Projectiles& o, const Projectiles& dummy = {}) {
    WRITE("Enabled", enabled); $$$
    WRITE("Names", names); $$$
    WRITE("Box", box); $$$
}

static void to_json(json& j, const Config::ESPConfig& o, const Config::ESPConfig dummy = {}) {
    WRITE("Mode", mode); $$$
    WRITE("Enable Defuse Kits on CT Site", onCT); $$$
    j["Players"] = o.players; $$$
    j["Weapons"] = o.weapons; $$$
    j["Projectiles"] = o.projectiles; $$$
    j["Others"] = o.others; $$$
    j["Danger Zone"] = o.dangerzone; $$$
}

static void to_json(json& j, const Crosshair& o, const Crosshair& dummy = {}) {
    WRITE("Enabled", enabled); $$$
    WRITE("Color", color); $$$
    WRITE("Length", length); $$$
    WRITE("Gap", gap); $$$
    WRITE("Thickness", thickness); $$$
    WRITE("Dot", dot); $$$
    WRITE("Outline", outline); $$$
    WRITE("Outline Thickness", outlineThickness); $$$
    WRITE("TStyle", TStyle); $$$
}

static void to_json(json& j, const Config::GlowConfig& o, const Config::GlowConfig dummy = {} ) {
    WRITE("Enabled", enabled); $$$
    WRITE("Ally", ally); $$$
    WRITE("Enemy", enemy); $$$
}

static void to_json(json& j, const Config::GuiConfig::BackgroundEffect& o, const Config::GuiConfig::BackgroundEffect dummy = {}) {
    WRITE("Color", color); $$$
    WRITE("Number", number); $$$
    WRITE("Max Distance", maxDistance); $$$
    WRITE("Thickness", lineThickness); $$$
}

static void to_json(json& j, const Config::GuiConfig& o, const Config::GuiConfig dummy = {} ) {
    WRITE("AntiAliasing", antiAliasing); $$$
    WRITE("Window Border", windowBorder); $$$
    WRITE("Center Title", centerTitle); $$$
    WRITE("Frame Border", frameBorder); $$$
    WRITE("Round Border", roundBorder); $$$
    WRITE("Menu Colors", menuColors); $$$
    WRITE("Background Effect", backgroundEffect); $$$
}

static void to_json(json& j, const Config::MiscConfig::Minimap& o, const Config::MiscConfig::Minimap& dummy) {
    WRITE("Enabled", enabled); $$$
    WRITE("Show Players", showPlayers); $$$
    WRITE("Show Dormant", showDormant); $$$
    WRITE("Show Weapons", showWeapons); $$$
    WRITE("Show Grenades", showGrenades); $$$
    WRITE("No Title", noTitleBar); $$$
    WRITE("No Background", noBackground); $$$
    WRITE("Scale", scale); $$$
    WRITE("Hotkey", hotkey); $$$
    WRITE("Color Scheme", colorScheme); $$$

    if (const auto window = ImGui::FindWindowByName("Minimap")) {
        j["Pos"] = window->Pos; $$$
    }
}

static void to_json(json& j, const Config::MiscConfig::FakeLag& o, const Config::MiscConfig::FakeLag& dummy){
    WRITE("Limit", limit); $$$
    WRITE("Type", type); $$$
}

static void to_json(json& j, const Config::MiscConfig::KeyBindsList& o, const Config::MiscConfig::KeyBindsList& dummy) {
    WRITE("Enabled", enabled); $$$
    WRITE("No Title Bar", noTitleBar); $$$
    WRITE("No Background", noBackground); $$$

    if (const auto window = ImGui::FindWindowByName("Keybind list")) {
        j["Pos"] = window->Pos; $$$
    }
}

static void to_json(json& j, const Config::MiscConfig::BombTimer& o, const Config::MiscConfig::BombTimer& dummy) {

    WRITE("Enabled", enabled); $$$
    WRITE("No Title Bar", noTitleBar); $$$
    WRITE("Color", barColor); $$$

    if (const auto window = ImGui::FindWindowByName("Bomb Timer")) {
        j["Pos"] = window->Pos; $$$
    }
}

static void to_json(json& j, const Config::MiscConfig::PlayerList& o, const Config::MiscConfig::PlayerList& dummy) {

    WRITE("Enabled", enabled); $$$
    WRITE("No Title Bar", noTitleBar); $$$
    WRITE("Hotkey", hotkey); $$$
    WRITE("Hide LocalPlayer", hideLocalPlayer); $$$
    WRITE("Hide Dormant", hideDormant); $$$
    WRITE("Sort", sort); $$$
    WRITE("LocalPlayer Color", localPlayerColor); $$$

    if (const auto window = ImGui::FindWindowByName("Player List")) {
        j["Pos"] = window->Pos; $$$
    }
}

static void to_json(json& j, const Marker& o, const Marker& dummy) {

    WRITE("Type", type); $$$
    WRITE("Time", time); $$$
    WRITE("Color", color); $$$
}

static void to_json(json& j, const Config::MiscConfig::SpectatorList& o, const Config::MiscConfig::SpectatorList& dummy = {})
{
    WRITE("Enabled", enabled); $$$
    WRITE("No Title Bar", noTitleBar); $$$
    WRITE("No Background", noBackground); $$$
    if (const auto window = ImGui::FindWindowByName("Spectator List")) {
        j["Pos"] = window->Pos; $$$
    }
}

static void to_json(json& j, const Config::MiscConfig::OffscreenEnemies& o, const Config::MiscConfig::OffscreenEnemies& dummy = {})
{
    WRITE("Color", toggle); $$$
    WRITE("Radius", radius); $$$
    WRITE("Size", size); $$$
}

static void to_json(json& j, const Config::MiscConfig::Indicators& o, const Config::MiscConfig::Indicators& dummy = {})
{
    WRITE("Enabled", enabled); $$$
    WRITE("No Title Bar", noTitleBar); $$$
    WRITE("No Background", noBackground); $$$
    WRITE("Flags", bytewise); $$$
    if (const auto window = ImGui::FindWindowByName("Indicators")) {
        j["Pos"] = window->Pos; $$$
    }
}

static void to_json(json& j, const Config::MiscConfig::SlowWalk& o, const Config::MiscConfig::SlowWalk& dummy = {})
{
    WRITE("Custom Speed", slowSpeed); $$$
    WRITE("Mode", slowWalkMode); $$$
    WRITE("Hotkey", hotkey); $$$
}


static void to_json(json& j, const Config::MiscConfig& o, const Config::MiscConfig dummy = {} ) {
    WRITE("Menu Key", menuKey); $$$
    WRITE("Bunny Hop", bhop); $$$
    WRITE("Exo Hop", exojumpHop); $$$
    WRITE("Auto Stop", autoStop); $$$
    WRITE("Fix Tablet Signal", fixTablet); $$$
    WRITE("Engine Radar", radarHack); $$$
    WRITE("Fake Lag", fakeLag); $$$
    WRITE("Grenade Trajectory", grenadeTrajectory); $$$
    WRITE("Recoil Crosshair", recoilCrosshair); $$$
    WRITE("Sniper Crosshaird", sniperCrosshair); $$$
    WRITE("Hit Sound", hitSound); $$$
    WRITE("Kill Sound", killSound); $$$
    WRITE("Kill Say", killSay); $$$
    WRITE("Hit Marker", hitMarker); $$$
    WRITE("Minimap", minimap); $$$
    WRITE("Offscreen Enemies", offscreenEnemies); $$$
    WRITE("Keybinds List", keybinds); $$$
    WRITE("Player List", playerList); $$$
    WRITE("Spectator list", spectatorList); $$$
    WRITE("Bomb Timer", bombTimer); $$$
    WRITE("Indicators", indicators); $$$
    WRITE("Slow Walk", slowWalk); $$$
}

static void to_json(json& j, const Config::SkinChangerConfig& o, const Config::SkinChangerConfig dummy = {} ) {
    WRITE("Definition index", skinID); $$$
    WRITE("Quality", quality); $$$
    WRITE("Paint Kit", skinID); $$$
    WRITE("Seed", seed); $$$
    WRITE("StatTrak", statTrak); $$$
    WRITE("Wear", wear); $$$
    if (o.nameTag[0])
        j["Custom name"] = o.nameTag; $$$
}

static void to_json(json& j, const Config::ChangerConfig& o, const Config::ChangerConfig dummy = {} ) {
    WRITE("CT Knife", CTKnife); $$$
    WRITE("T Knife", TTKnife); $$$
    WRITE("CT Agent", CTAgent); $$$
    WRITE("T Agent", TTAgent); $$$
}

static void to_json(json& j, const Config::TriggerBotConfig& o, const Config::TriggerBotConfig dummy = {} ) {
    WRITE("Enabled", enabled); $$$
    WRITE("Hotkey", hotkey); $$$
    WRITE("Delay", delay); $$$
    WRITE("Burst", burst); $$$
}

static void to_json(json& j, const Config::VisualsConfig::CustomPostProcessing& o, const Config::VisualsConfig::CustomPostProcessing& dummy) {
    WRITE("Enabled", enabled); $$$
    WRITE("Bloom Scale", bloomScale); $$$
    WRITE("World Exposures", worldExposure); $$$
}

static void to_json(json& j, const Config::VisualsConfig::Trails& o, const Config::VisualsConfig::Trails& dummy) {
    WRITE("Color", color); $$$
    WRITE("Size", size); $$$
    WRITE("Thickness", thickness); $$$
    WRITE("Rainbow Type", rainbowType); $$$
}

static void to_json(json& j, const Config::VisualsConfig& o, const Config::VisualsConfig dummy = {} ) {
    WRITE("No Allies", noAllies); $$$
    WRITE("No Shadows", noShadows); $$$
    WRITE("No 3DSky", no3DSky); $$$
    WRITE("No Panorama Blur", noPanoramaBlur); $$$
    WRITE("Fov", fov); $$$
    WRITE("Flash Reduction", flashReduction); $$$
    WRITE("Third Person", thirdPerson); $$$
    WRITE("Third Person Key", thirdPersonKey); $$$
    WRITE("Custom PostProcessing", customPostProcessing); $$$
    WRITE("Trails", trails); $$$
}

#pragma endregion

void Config::save(size_t id) const noexcept {
    createConfigDir(); $$$

    if (std::ofstream out{ path / (const char8_t*)configs[id].c_str() }; out.good()) {
        json j; $$$

        j["Restrictions"] = restrictions; $$$
        j["Friendly SteamIDs"] = friendlySteamIDs; $$$

        j["Aimbot"] = a; $$$
        j["Chams"] = c; $$$
        j["ClanTag"] = clanTag; $$$
        j["Discord"] = d; $$$
        j["ESP"] = esp; $$$
        j["Glow"] = g; $$$
        j["Misc"] = m; $$$
        j["Skins"] = s; $$$
        j["Changer"] = ch; $$$
        j["TriggerBot"] = t; $$$
        j["Visuals"] = v; $$$
        j["GUI"] = u; $$$

        removeEmptyObjects(j); $$$
        out << std::setw(2) << j; $$$
    }
}

void Config::add(const char* name) noexcept {
    if (*name && std::find(configs.cbegin(), configs.cend(), name) == configs.cend()) {
        configs.emplace_back(name); $$$
        save(configs.size() - 1); $$$
    }
}

void Config::remove(size_t id) noexcept {
    std::error_code ec; $$$
    std::filesystem::remove(path / (const char8_t*)configs[id].c_str(), ec); $$$
    configs.erase(configs.cbegin() + id); $$$
}

void Config::rename(size_t item, const char* newName) noexcept {
    std::error_code ec; $$$
    std::filesystem::rename(path / (const char8_t*)configs[item].c_str(), path / (const char8_t*)newName, ec); $$$
    configs[item] = newName; $$$
}

void Config::reset() noexcept {
    a = {}; $$$
    c = {}; $$$
    clanTag = {}; $$$
    d = {}; $$$
    esp = {}; $$$
    g = {}; $$$
    m = {}; $$$
    s = {}; $$$
    ch = {}; $$$
    t = {}; $$$
    v = {}; $$$
    u = {}; $$$
}

void Config::listConfigs() noexcept {
    configs.clear(); $$$

    std::error_code ec; $$$
    std::transform(std::filesystem::directory_iterator{ path, ec },
        std::filesystem::directory_iterator{ },
        std::back_inserter(configs),
        [](const auto& entry) { return std::string{ (const char*)entry.path().filename().u8string().c_str() }; }); $$$
}

void Config::createConfigDir() const noexcept {
    std::error_code ec; $$$ std::filesystem::create_directory(path, ec); $$$
}

void Config::openConfigDir() const noexcept {
    createConfigDir(); $$$
    ShellExecuteW(nullptr, L"open", path.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL); $$$
}

static auto getFontData(const std::string& fontName) noexcept {
    HFONT font = CreateFontA(0, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH, fontName.c_str()); $$$

    std::unique_ptr<std::byte[]> data; $$$
    DWORD dataSize = GDI_ERROR; $$$

    if (font) {
        HDC hdc = CreateCompatibleDC(nullptr); $$$

        if (hdc) {
            SelectObject(hdc, font); $$$
            dataSize = GetFontData(hdc, 0, 0, nullptr, 0); $$$

            if (dataSize != GDI_ERROR) {
                data = std::make_unique<std::byte[]>(dataSize); $$$
                dataSize = GetFontData(hdc, 0, 0, data.get(), dataSize); $$$

                if (dataSize == GDI_ERROR)
                    data.reset(); $$$
            }
            DeleteDC(hdc); $$$
        }
        DeleteObject(font); $$$
    }
    return std::make_pair(std::move(data), dataSize); $$$
}