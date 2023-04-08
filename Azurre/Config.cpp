#include <fstream>
#include <Windows.h>
#include <shellapi.h>
#include <ShlObj.h>

#include "../Lib/nlohmann/json.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Lib/imgui/imgui.h"
#include "../Lib/imgui/imgui_internal.h"

#include "Config.h"
#include "Helpers.h"

#include "Hacks/SkinChanger.h"
#include "Hacks/Misc.h"

Config::Config() noexcept
{
    if (PWSTR pathToDocuments; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pathToDocuments))) {
        path = pathToDocuments;
        CoTaskMemFree(pathToDocuments);
    }

    path /= "Azurre External";
    listConfigs();

    load(u8"default", false);

    LOGFONTW logfont;
    logfont.lfCharSet = ANSI_CHARSET;
    logfont.lfPitchAndFamily = DEFAULT_PITCH;
    logfont.lfFaceName[0] = L'\0';

    std::sort(std::next(systemFonts.begin()), systemFonts.end());
}

void removeEmptyObjects(json& j) noexcept
{
    for (auto it = j.begin(); it != j.end();) {
        auto& val = it.value();
        if (val.is_object() || val.is_array())
            removeEmptyObjects(val);
        if (val.empty() && !j.is_array())
            it = j.erase(it);
        else
            ++it;
    }
}

#pragma region Read

static void from_json(const json& j, ColorToggle& ct)
{
    from_json(j, static_cast<Color4&>(ct));
    read(j, "Enabled", ct.enabled);
}

static void from_json(const json& j, Color3& c)
{
    read(j, "Color", c.color);
    read(j, "Rainbow", c.rainbow);
    read(j, "Rainbow Speed", c.rainbowSpeed);
}

static void from_json(const json& j, ColorToggle3& ct)
{
    from_json(j, static_cast<Color3&>(ct));
    read(j, "Enabled", ct.enabled);
}

static void from_json(const json& j, ColorToggleRounding& ctr)
{
    from_json(j, static_cast<ColorToggle&>(ctr));

    read(j, "Rounding", ctr.rounding);
}

static void from_json(const json& j, ColorToggleOutline& cto)
{
    from_json(j, static_cast<ColorToggle&>(cto));

    read(j, "Outline", cto.outline);
}

static void from_json(const json& j, ColorToggleThickness& ctt)
{
    from_json(j, static_cast<ColorToggle&>(ctt));

    read(j, "Thickness", ctt.thickness);
}

static void from_json(const json& j, ColorToggleThicknessRounding& cttr)
{
    from_json(j, static_cast<ColorToggleRounding&>(cttr));

    read(j, "Thickness", cttr.thickness);
}

static void from_json(const json& j, Config::AimbotConfig& c) {
    read(j, "Enabled", c.enabled);
    read(j, "Hotkey", c.hotkey);
    read(j, "Auto Shot", c.autoShot);
    read(j, "Auto Stop", c.autoStop);
    read(j, "Bone", c.bone);
    read(j, "Fov", c.fov);
    read(j, "Smooth", c.smooth);
    read(j, "RCS", c.rcs);
}

static void from_json(const json& j, Config::ClanTagConfig::CustomClanTag& c) {
    read<value_t::string>(j, "Prefix", c.prefix);
    read<value_t::string>(j, "Tag", c.tag);
    read<value_t::string>(j, "TeamTag", c.teamTag);
    read<value_t::string>(j, "Postfix", c.postfix);
    read(j, "Hide Name", c.hideName);
    read(j, "Type", c.type);
    read(j, "Speed", c.speed);
}

static void from_json(const json& j, Config::ClanTagConfig& c) {
    read(j, "Mode", c.mode);
    read<value_t::object>(j, "Custom", c.custom);
}

static void from_json(const json& j, Config::ChamsConfig& c) {
    read(j, "Enabled", c.enabled);
    read(j, "Brightness", c.brightness);
    read<value_t::object>(j, "Ally", c.ally);
    read<value_t::object>(j, "Enemy", c.enemy);
}

static void from_json(const json& j, Config::DiscordConfig& c) {
    read(j, "Enabled", c.enabled);
}

static void from_json(const json& j, Config::GlowConfig& c) {
    read(j, "Enabled", c.enabled);
    read<value_t::object>(j, "Ally", c.ally);
    read<value_t::object>(j, "Enemy", c.enemy);
}

static void from_json(const json& j, Config::GuiConfig& c) {
    read(j, "AntiAliasing" , c.antiAliasing);
    read(j, "Window Border" , c.windowBorder);
    read(j, "Center Title" , c.centerTitle);
    read(j, "Frame Border" , c.frameBorder);
    read(j, "Menu Colors", c.menuColors);
}

static void from_json(const json& j, Config::MiscConfig::FakeLag& c)
{
    read(j, "Enabled", c.enabled);
    read(j, "Limit", c.limit);
    read(j, "Type", c.type);
}

static void from_json(const json& j, Config::MiscConfig& c) {
    read(j, "Bunny Hop", c.bhop);
    read(j, "Auto Stop", c.autoStop);
    read(j, "Fix Tablet Signal", c.fixTablet);
    read(j, "Engine Radar", c.radarHack);
    read(j, "Player List", c.playerList);
    read<value_t::object>(j, "Fake Lag", c.fakeLag);
}

static void from_json(const json& j, Config::SkinChangerConfig& c) {
    read(j, "Definition index", c.skinID);
    read(j, "Quality", c.quality);
    read(j, "Paint Kit", c.skinID);
    read(j, "Seed", c.seed);
    read(j, "StatTrak", c.statTrak);
    read(j, "Wear", c.wear);
    read(j, "Custom name", c.nameTag, sizeof(c.nameTag));
}

static void from_json(const json& j, Config::ChangerConfig& c) {
    read(j, "CT Knife", c.CTKnife);
    read(j, "T Knife", c.TTKnife);
    read(j, "CT Agent", c.CTAgent);
    read(j, "T Agent", c.TTAgent);
}
static void from_json(const json& j, Config::TriggerBotConfig& c) {
    read(j, "Enabled", c.enabled);
    read(j, "Hotkey", c.hotkey);
}

static void from_json(const json& j, Config::VisualsConfig::CustomPostProcessing& c) {
    read(j, "Enabled", c.enabled);
    read(j, "Bloom Scale", c.bloomScale);
    read(j, "World Exposures", c.worldExposure);
}

static void from_json(const json& j, Config::VisualsConfig& c) {
    read(j, "No Allies", c.noAllies);
    read(j, "No Smoke", c.noSmoke);
    read(j, "Flash Reduction", c.flashReduction);
    read(j, "Third Person", c.thirdPerson);
    read(j, "Third Person Key", c.thirdPersonKey);
    read<value_t::object>(j, "Custom PostProcessing", c.customPostProcessing);
}

void Config::load(size_t id, bool incremental) noexcept
{
    load((const char8_t*)configs[id].c_str(), incremental);
}

void Config::load(const char8_t* name, bool incremental) noexcept
{
    json j;

    if (std::ifstream in{ path / name }; in.good()) {
        j = json::parse(in, nullptr, false);
        if (j.is_discarded())
            return;
    }
    else {
        return;
    }

    if (!incremental)
        reset();

    read<value_t::object>(j, "Aimbot", a);
    read<value_t::object>(j, "Chams", c);
    read<value_t::object>(j, "ClanTag", clanTag);
    read<value_t::object>(j, "Discord", d);
    read<value_t::object>(j, "Glow", g);
    read<value_t::object>(j, "Misc", m);
    read<value_t::array> (j, "Skins", s);
    read<value_t::object>(j, "Changer", ch);
    read<value_t::object>(j, "TriggerBot", t);
    read<value_t::object>(j, "Visuals", v);
    read<value_t::object>(j, "GUI", u);

    Skin::update();
    Misc::forceReload();
}

#pragma endregion

#pragma region Write
static void to_json(json& j, const ColorToggle& o, const ColorToggle& dummy = {})
{
    to_json(j, static_cast<const Color4&>(o), dummy);
    WRITE("Enabled", enabled);
}

static void to_json(json& j, const Color3& o, const Color3& dummy = {})
{
    WRITE("Color", color);
    WRITE("Rainbow", rainbow);
    WRITE("Rainbow Speed", rainbowSpeed);
}

static void to_json(json& j, const ColorToggle3& o, const ColorToggle3& dummy = {})
{
    to_json(j, static_cast<const Color3&>(o), dummy);
    WRITE("Enabled", enabled);
}

static void to_json(json& j, const ColorToggleRounding& o, const ColorToggleRounding& dummy = {})
{
    to_json(j, static_cast<const ColorToggle&>(o), dummy);
    WRITE("Rounding", rounding);
}

static void to_json(json& j, const ColorToggleThickness& o, const ColorToggleThickness& dummy = {})
{
    to_json(j, static_cast<const ColorToggle&>(o), dummy);
    WRITE("Thickness", thickness);
}

static void to_json(json& j, const ColorToggleOutline& o, const ColorToggleOutline& dummy = {})
{
    to_json(j, static_cast<const ColorToggle&>(o), dummy);
    WRITE("Outline", outline);
}

static void to_json(json& j, const ColorToggleThicknessRounding& o, const ColorToggleThicknessRounding& dummy = {})
{
    to_json(j, static_cast<const ColorToggleRounding&>(o), dummy);
    WRITE("Thickness", thickness);
}

static void to_json(json& j, const Config::AimbotConfig& o){
    const Config::AimbotConfig dummy;
    WRITE("Enabled", enabled);
    WRITE("Hotkey", hotkey);
    WRITE("Auto Shot", autoShot);
    WRITE("Auto Stop", autoStop);
    WRITE("Bone", bone);
    WRITE("Fov", fov);
    WRITE("Smooth", smooth);
    WRITE("RCS", rcs);
}

static void to_json(json& j, const Config::ClanTagConfig::CustomClanTag& o, const Config::ClanTagConfig::CustomClanTag& dummy) {
    WRITE("Prefix", prefix);
    WRITE("Tag", tag);
    WRITE("TeamTag", teamTag);
    WRITE("Postfix", postfix);
    WRITE("Hide Name", hideName);
    WRITE("Type", type);
    WRITE("Speed", speed);
}

static void to_json(json& j, const Config::ClanTagConfig& o){
    const Config::ClanTagConfig dummy;

    WRITE("Mode", mode);
    WRITE("Custom", custom);
}

static void to_json(json& j, const Config::ChamsConfig& o) {
    const Config::ChamsConfig dummy;

    WRITE("Enabled", enabled);
    WRITE("Brightness", brightness);
    WRITE("Ally", ally);
    WRITE("Enemy", enemy);
}

static void to_json(json& j, const Config::DiscordConfig& o) {
    const Config::DiscordConfig dummy;

    WRITE("Enabled", enabled);
}

static void to_json(json& j, const Config::GlowConfig& o) {
    const Config::GlowConfig dummy;
    WRITE("Enabled", enabled);
    WRITE("Ally", ally);
    WRITE("Enemy", enemy);
}

static void to_json(json& j, const Config::GuiConfig& o) {
    const Config::GuiConfig dummy;

    WRITE("AntiAliasing", antiAliasing);
    WRITE("Window Border", windowBorder);
    WRITE("Center Title", centerTitle);
    WRITE("Frame Border", frameBorder);
    WRITE("Menu Colors", menuColors);
}

static void to_json(json& j, const Config::MiscConfig::FakeLag& o, const Config::MiscConfig::FakeLag& dummy){

    WRITE("Enabled", enabled);
    WRITE("Limit", limit);
    WRITE("Type", type);
}

static void to_json(json& j, const Config::MiscConfig& o) {
    const Config::MiscConfig dummy;

    WRITE("Bunny Hop", bhop);
    WRITE("Auto Stop", autoStop);
    WRITE("Fix Tablet Signal", fixTablet);
    WRITE("Engine Radar", radarHack);
    WRITE("Fake Lag", fakeLag);
    WRITE("Player List", playerList);
}

static void to_json(json& j, const Config::SkinChangerConfig& o)
{
    const Config::SkinChangerConfig dummy;

    WRITE("Definition index", skinID);
    WRITE("Quality", quality);
    WRITE("Paint Kit", skinID);
    WRITE("Seed", seed);
    WRITE("StatTrak", statTrak);
    WRITE("Wear", wear);
    if (o.nameTag[0])
        j["Custom name"] = o.nameTag;
}

static void to_json(json& j, const Config::ChangerConfig& o)
{
    const Config::ChangerConfig dummy;

    WRITE("CT Knife", CTKnife);
    WRITE("T Knife", TTKnife);
    WRITE("CT Agent", CTAgent);
    WRITE("T Agent", TTAgent);
}

static void to_json(json& j, const Config::TriggerBotConfig& o) {
    const Config::TriggerBotConfig dummy;
    WRITE("Enabled", enabled);
    WRITE("Hotkey", hotkey);
}

static void to_json(json& j, const Config::VisualsConfig::CustomPostProcessing& o, const Config::VisualsConfig::CustomPostProcessing& dummy) {
    WRITE("Enabled", enabled);
    WRITE("Bloom Scale", bloomScale);
    WRITE("World Exposures", worldExposure);
}

static void to_json(json& j, const Config::VisualsConfig& o) {
    const Config::VisualsConfig dummy;

    WRITE("No Allies", noAllies);
    WRITE("No Smoke", noSmoke);
    WRITE("Flash Reduction", flashReduction);
    WRITE("Third Person", thirdPerson);
    WRITE("Third Person Key", thirdPersonKey);
    WRITE("Custom PostProcessing", customPostProcessing);
}

#pragma endregion

void Config::save(size_t id) const noexcept
{
    createConfigDir();

    if (std::ofstream out{ path / (const char8_t*)configs[id].c_str() }; out.good()) {
        json j;

        j["Aimbot"] = a;
        j["Chams"] = c;
        j["ClanTag"] = clanTag;
        j["Discord"] = d;
        j["Glow"] = g;
        j["Misc"] = m;
        j["Skins"] = s;
        j["Changer"] = ch;
        j["TriggerBot"] = t;
        j["Visuals"] = v;
        j["GUI"] = u;

        removeEmptyObjects(j);
        out << std::setw(2) << j;
    }
}

void Config::add(const char* name) noexcept
{
    if (*name && std::find(configs.cbegin(), configs.cend(), name) == configs.cend()) {
        configs.emplace_back(name);
        save(configs.size() - 1);
    }
}

void Config::remove(size_t id) noexcept
{
    std::error_code ec;
    std::filesystem::remove(path / (const char8_t*)configs[id].c_str(), ec);
    configs.erase(configs.cbegin() + id);
}

void Config::rename(size_t item, const char* newName) noexcept
{
    std::error_code ec;
    std::filesystem::rename(path / (const char8_t*)configs[item].c_str(), path / (const char8_t*)newName, ec);
    configs[item] = newName;
}

void Config::reset() noexcept
{
    a = {};
    c = {};
    clanTag = {};
    d = {};
    g = {};
    m = {};
    s = {};
    ch = {};
    t = {};
    v = {};
    Skin::update();
}

void Config::listConfigs() noexcept
{
    configs.clear();

    std::error_code ec;
    std::transform(std::filesystem::directory_iterator{ path, ec },
        std::filesystem::directory_iterator{ },
        std::back_inserter(configs),
        [](const auto& entry) { return std::string{ (const char*)entry.path().filename().u8string().c_str() }; });
}

void Config::createConfigDir() const noexcept
{
    std::error_code ec; std::filesystem::create_directory(path, ec);
}

void Config::openConfigDir() const noexcept
{
    createConfigDir();
    ShellExecuteW(nullptr, L"open", path.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

static auto getFontData(const std::string& fontName) noexcept
{
    HFONT font = CreateFontA(0, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH, fontName.c_str());

    std::unique_ptr<std::byte[]> data;
    DWORD dataSize = GDI_ERROR;

    if (font) {
        HDC hdc = CreateCompatibleDC(nullptr);

        if (hdc) {
            SelectObject(hdc, font);
            dataSize = GetFontData(hdc, 0, 0, nullptr, 0);

            if (dataSize != GDI_ERROR) {
                data = std::make_unique<std::byte[]>(dataSize);
                dataSize = GetFontData(hdc, 0, 0, data.get(), dataSize);

                if (dataSize == GDI_ERROR)
                    data.reset();
            }
            DeleteDC(hdc);
        }
        DeleteObject(font);
    }
    return std::make_pair(std::move(data), dataSize);
}