#pragma once

#include <array>
#include <string>
#include "../SDK/WeaponID.h"

namespace Skin {

    struct weapon_name {
        constexpr weapon_name(WeaponID definition_index, const char* name) : definition_index(definition_index), name(name) {}

        WeaponID definition_index;
        const char* name;
    };

    constexpr auto weapon_names = std::to_array<weapon_name>({
    {WeaponID::Ak47, "AK-47"},
    {WeaponID::Aug, "AUG"},
    {WeaponID::Awp, "AWP"},
    {WeaponID::Cz75a, "CZ75 Auto"},
    {WeaponID::Deagle, "Desert Eagle"},
    {WeaponID::Elite, "Dual Berettas"},
    {WeaponID::Famas, "FAMAS"},
    {WeaponID::Fiveseven, "Five-SeveN"},
    {WeaponID::G3SG1, "G3SG1"},
    {WeaponID::GalilAr, "Galil AR"},
    {WeaponID::Glock, "Glock-18"},
    {WeaponID::M249, "M249"},
    {WeaponID::M4a1_s, "M4A1-S"},
    {WeaponID::M4A1, "M4A4"},
    {WeaponID::Mac10, "MAC-10"},
    {WeaponID::Mag7, "MAG-7"},
    {WeaponID::Mp5sd, "MP5-SD"},
    {WeaponID::Mp7, "MP7"},
    {WeaponID::Mp9, "MP9"},
    {WeaponID::Negev, "Negev"},
    {WeaponID::Nova, "Nova"},
    {WeaponID::Hkp2000, "P2000"},
    {WeaponID::P250, "P250"},
    {WeaponID::P90, "P90"},
    {WeaponID::Bizon, "PP-Bizon"},
    {WeaponID::Revolver, "R8 Revolver"},
    {WeaponID::Sawedoff, "Sawed-Off"},
    {WeaponID::Scar20, "SCAR-20"},
    {WeaponID::Ssg08, "SSG 08"},
    {WeaponID::Sg553, "SG 553"},
    {WeaponID::Tec9, "Tec-9"},
    {WeaponID::Ump45, "UMP-45"},
    {WeaponID::Usp_s, "USP-S"},
    {WeaponID::Xm1014, "XM1014"}
    });

    void add(int weaponID, int skinID, float wear, int seed, int statTrak, int quality, std::string nameTag);
    void update();
}