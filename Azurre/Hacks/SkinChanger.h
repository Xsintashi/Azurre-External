#pragma once

#include <array>
#include <string>
#include "../SDK/WeaponID.h"

namespace Skin {

    struct weaponName {
        constexpr weaponName(WeaponID definitionIndex, const char* name) : definitionIndex(definitionIndex), name(name) {}

        WeaponID definitionIndex;
        const char* name;
    };

    constexpr auto weaponNames = std::to_array<weaponName>({
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
        {WeaponID::Xm1014, "XM1014"},
        {WeaponID::C4, "C4"},

        {WeaponID::Bayonet, "Bayonet Knife"},
        {WeaponID::Bowie, "Bowie Knife"},
        {WeaponID::Butterfly, "Butterfly Knife"},
        {WeaponID::ClassicKnife, "Classic Knife"},
        {WeaponID::Falchion, "Falchion Knife"},
        {WeaponID::Flip, "Flip Knife"},
        {WeaponID::Gut, "Gut Knife"},
        {WeaponID::Huntsman, "Huntsman Knife"},
        {WeaponID::Karambit, "Karambit"},
        {WeaponID::M9Bayonet, "M9 Bayonet Knife"},
        {WeaponID::Navaja, "Navaja"},
        {WeaponID::NomadKnife, "Nomad Knife"},
        {WeaponID::Paracord, "Paracord Knife"},
        {WeaponID::Daggers, "Shadow Daggers"},
        {WeaponID::SkeletonKnife, "Skeleton Knife"},
        {WeaponID::Stiletto, "Stiletto"},
        {WeaponID::SurvivalKnife, "Survival Knife"},
        {WeaponID::Talon, "Talon Knife"},
        {WeaponID::Ursus, "Ursus Knife"},
        {WeaponID::GoldenKnife, "Gold Knife"}
    });

    constexpr auto knifeNames = std::to_array<weaponName>({
        {WeaponID::Knife, "Default CT Knife"},
        {WeaponID::KnifeT, "Default T Knife"},
        {WeaponID::Bayonet, "Bayonet Knife"},
        {WeaponID::Bowie, "Bowie Knife"},
        {WeaponID::Butterfly, "Butterfly Knife"},
        {WeaponID::ClassicKnife, "Classic Knife"},
        {WeaponID::Falchion, "Falchion Knife"},
        {WeaponID::Flip, "Flip Knife"},
        {WeaponID::Gut, "Gut Knife"},
        {WeaponID::Huntsman, "Huntsman Knife"},
        {WeaponID::Karambit, "Karambit"},
        {WeaponID::M9Bayonet, "M9 Bayonet Knife"},
        {WeaponID::Navaja, "Navaja"},
        {WeaponID::NomadKnife, "Nomad Knife"},
        {WeaponID::Paracord, "Paracord Knife"},
        {WeaponID::Daggers, "Shadow Daggers"},
        {WeaponID::SkeletonKnife, "Skeleton Knife"},
        {WeaponID::Stiletto, "Stiletto"},
        {WeaponID::SurvivalKnife, "Survival Knife"},
        {WeaponID::Talon, "Talon Knife"},
        {WeaponID::Ursus, "Ursus Knife"},
        {WeaponID::GoldenKnife, "Gold Knife"}
    });

    void add(int idx, short weaponID, int skinID, float wear, int seed, int statTrak, int quality, char nameTag[161]);
    void update();
    const char* getWeaponIDName(short ID) noexcept;

}