#pragma once

#include <array>
#include <iomanip>
#include <sstream>
#include <string>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Lib/nlohmann/json.hpp"
#include "InputUtil.h"
#include "../Lib/imgui/imgui.h"

enum class WeaponId : short;

#pragma pack(push, 1)
struct Color4 {
    std::array<float, 4> color{ 1.0f, 1.0f, 1.0f, 1.0f };
    float rainbowSpeed = 1.f;
    bool rainbow = false;
};

struct Color3 {
    std::array<float, 3> color{ 1.0f, 1.0f, 1.0f };
    float rainbowSpeed = 1.f;
    bool rainbow = false;
};
#pragma pack(pop)

struct ColorToggle3 : public Color3 {
    bool enabled = false;
};

struct ColorToggle : Color4 {
    bool enabled = false;
};

struct ColorToggleOutline : ColorToggle {
    bool outline = true;
};

struct ColorToggleThickness : ColorToggle {
    ColorToggleThickness() = default;
    ColorToggleThickness(float thickness) : thickness{ thickness } { }
    float thickness = 1.0f;
};

struct ColorToggleRounding : ColorToggle {
    float rounding = 0.0f;
};

struct ColorToggleThicknessRounding : ColorToggleRounding {
    float thickness = 1.0f;
};

struct Other {
    ColorToggle3 names = { 1.f, 1.f, 1.f };
    ColorToggle3 weapons = { 1.f, 1.f, 1.f };
    ColorToggle3 lines = { 1.f, 1.f, 1.f };
};

struct Box {
    bool enabled = false;
    bool gradientColor = false;
    Color3 solid = { 1.f, 1.f, 1.f };
    Color3 grandientTop = { 1.f, 1.f, 1.f };
    Color3 grandientBottom = { 1.f, 1.f, 1.f };
};

struct HealthBar {
    bool enabled = false;
    ColorToggle3 solidColor = { 0.f, 1.f, 0.f };
    ColorToggle3 showHealthNumber = { 1.f, 1.f, 1.f };
};

struct Player {
    Other other;
    Box box;
    HealthBar healthBar;
    bool skeleton;
};

using json = nlohmann::basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t, float>;
using value_t = json::value_t;

// WRITE macro requires:
// - json object named 'j'
// - object holding default values named 'dummy'
// - object to write to json named 'o'
#define WRITE(name, valueName) to_json(j[name], o.valueName, dummy.valueName)
#define WRITE_ENUM(name, valueName) to_json(j[name], static_cast<int>(o.valueName), static_cast<int>(dummy.valueName))

template <typename T>
static void to_json(json& j, const T& o, const T& dummy)
{
    if (o != dummy)
        j = o;
}

static void to_json(json& j, const Color4& o, const Color4& dummy = {})
{
    if (o.color != dummy.color) {
        std::ostringstream s;
        s << '#' << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(o.color[0] * 255) << std::setw(2) << static_cast<int>(o.color[1] * 255) << std::setw(2) << static_cast<int>(o.color[2] * 255);
        j["Color"] = s.str();
        j["Alpha"] = o.color[3];
    }
    WRITE("Rainbow", rainbow);
    WRITE("Rainbow Speed", rainbowSpeed);
}

static void to_json(json& j, const KeyBind& o, const KeyBind& dummy)
{
    if (o != dummy)
    {
        j["Key"] = o.toString();
        j["Key mode"] = o.keyMode;
    }
}

template <value_t Type, typename T>
static typename std::enable_if_t<!std::is_same_v<T, bool>> read(const json& j, const char* key, T& o) noexcept
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.type() == Type)
        val.get_to(o);
}

static void read(const json& j, const char* key, bool& o) noexcept
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.type() == value_t::boolean)
        val.get_to(o);
}

static void read(const json& j, const char* key, float& o) noexcept
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.type() == value_t::number_float)
        val.get_to(o);
}

static void read(const json& j, const char* key, int& o) noexcept
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.is_number_integer())
        val.get_to(o);
}

static void read(const json& j, const char* key, WeaponId& o) noexcept
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.is_number_integer())
        val.get_to(o);
}

static void read(const json& j, const char* key, KeyMode& o) noexcept
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.is_number_integer())
        val.get_to(o);
}

static void read(const json& j, const char* key, KeyBind& o) noexcept
{
    if (!j.contains(key) || !j[key].contains("Key"))
        return;

    const auto backupName = o.activeName;

    if (const auto& val = j[key]["Key"]; val.is_string())
        o = val.get<std::string>().c_str();

    if (const auto& val = j[key]["Key mode"]; val.is_number_integer())
        val.get_to(o.keyMode);

    o.activeName = backupName;
    o.setToggleTo(false);
}

static void read(const json& j, const char* key, char* o, std::size_t size) noexcept
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.is_string()) {
#pragma warning( disable : 4996 )
        std::strncpy(o, val.get<std::string>().c_str(), size);
        o[size - 1] = '\0';
    }
}

template <typename T, size_t Size>
static void read_array_opt(const json& j, const char* key, std::array<T, Size>& o) noexcept
{
    if (j.contains(key) && j[key].type() == value_t::array) {
        std::size_t i = 0;
        for (const auto& e : j[key]) {
            if (i >= o.size())
                break;

            if (e.is_null())
                continue;

            e.get_to(o[i]);
            ++i;
        }
    }
}

template <typename T, size_t Size>
static void read(const json& j, const char* key, std::array<T, Size>& o) noexcept
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.type() == value_t::array && val.size() == o.size()) {
        for (std::size_t i = 0; i < val.size(); ++i) {
            if (!val[i].empty())
                val[i].get_to(o[i]);
        }
    }
}

template <typename T>
static void read(const json& j, const char* key, std::unordered_map<std::string, T>& o) noexcept
{
    if (j.contains(key) && j[key].is_object()) {
        for (auto& element : j[key].items())
            element.value().get_to(o[element.key()]);
    }
}

static void from_json(const json& j, Color4& c)
{
    if (j.contains("Color")) {
        const auto& val = j["Color"];
        // old format -> [1.0f, 0.0f, 0.0f, 1.0f]
        // new format -> #ff0000 + alpha as float
        if (val.type() == value_t::array && val.size() == c.color.size()) {
            for (std::size_t i = 0; i < val.size(); ++i) {
                if (!val[i].empty())
                    val[i].get_to(c.color[i]);
            }
        }
        else if (val.type() == value_t::string) {
            const auto str = val.get<std::string>();
            if (str.length() == 7 && str[0] == '#') {
                const auto color = std::strtol(str.c_str() + 1, nullptr, 16);
                c.color[0] = ((color >> 16) & 0xFF) / 255.0f;
                c.color[1] = ((color >> 8) & 0xFF) / 255.0f;
                c.color[2] = (color & 0xFF) / 255.0f;
            }
            read(j, "Alpha", c.color[3]);
        }
    }

    read(j, "Rainbow", c.rainbow);
    read(j, "Rainbow Speed", c.rainbowSpeed);
}