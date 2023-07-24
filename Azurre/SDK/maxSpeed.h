#pragma once
// Cuz of didn't figure out way to get WeaponData. MaxSpeed are just hardcoded

#include "WeaponID.h"

#include <array>

//https://docs.google.com/spreadsheets/d/11tDzUNBq9zIX6_9Rel__fdAUezAQzSnh5AVYzCP060c/htmlview
static float getWeaponMaxSpeed(short ID) noexcept {
    switch (ID) {
        default: return 750.f;
        case WeaponID::Ak47: return 215.f;
        case WeaponID::Aug: return 220.f;
        case WeaponID::Awp: return 200.f;
        case WeaponID::Cz75a: return 240.f;
        case WeaponID::Deagle: return 230.f;
        case WeaponID::Elite: return 240.f;
        case WeaponID::Famas: return 220.f;
        case WeaponID::Fiveseven: return 240.f;
        case WeaponID::G3SG1: return 215.f;
        case WeaponID::GalilAr: return 215.f;
        case WeaponID::Glock: return 240.f;
        case WeaponID::M249: return 195.f;
        case WeaponID::M4a1_s: return 225.f;
        case WeaponID::M4A1: return 225.f;
        case WeaponID::Mac10: return 240.f;
        case WeaponID::Mag7: return 225.f;
        case WeaponID::Mp5sd: return 235.f;
        case WeaponID::Mp7: return 220.f;
        case WeaponID::Mp9: return 240.f;
        case WeaponID::Negev: return 150.f;
        case WeaponID::Nova: return 220.f;
        case WeaponID::Hkp2000: return 240.f;
        case WeaponID::P250: return 240.f;
        case WeaponID::P90: return 230.f;
        case WeaponID::Bizon: return 240.f;
        case WeaponID::Revolver: return 180.f;
        case WeaponID::Sawedoff: return 210.f;
        case WeaponID::Scar20: return 215.f;
        case WeaponID::Ssg08: return 230.f;
        case WeaponID::Sg553: return 210.f;
        case WeaponID::Tec9: return 240.f;
        case WeaponID::Ump45: return 230.f;
        case WeaponID::Usp_s: return 240.f;
        case WeaponID::Xm1014: return 215.f;
    }
}