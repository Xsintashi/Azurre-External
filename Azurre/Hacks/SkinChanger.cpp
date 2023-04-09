#include "SkinChanger.h"
#include "../Config.h"
#include "../GUI.h"

#include "../SDK/Entity.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/SkinID.h"
#include "../SDK/WeaponID.h"

bool pleaseUpdate = false;

constexpr const char* models[]{
	"models/player/custom_player/legacy/ctm_fbi_variantb.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantf.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantg.mdl",
	"models/player/custom_player/legacy/ctm_fbi_varianth.mdl",
	"models/player/custom_player/legacy/ctm_sas_variantf.mdl",
	"models/player/custom_player/legacy/ctm_st6_variante.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantg.mdl",
	"models/player/custom_player/legacy/ctm_st6_varianti.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantk.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantm.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantf.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantg.mdl",
	"models/player/custom_player/legacy/tm_balkan_varianth.mdl",
	"models/player/custom_player/legacy/tm_balkan_varianti.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantj.mdl",
	"models/player/custom_player/legacy/tm_leet_variantf.mdl",
	"models/player/custom_player/legacy/tm_leet_variantg.mdl",
	"models/player/custom_player/legacy/tm_leet_varianth.mdl",
	"models/player/custom_player/legacy/tm_leet_varianti.mdl",
	"models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
	"models/player/custom_player/legacy/tm_phoenix_variantg.mdl",
	"models/player/custom_player/legacy/tm_phoenix_varianth.mdl",
	"models/player/custom_player/legacy/tm_phoenix_varianti.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantj.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantl.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantk.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantl.mdl",
	"models/player/custom_player/legacy/ctm_swat_variante.mdl",
	"models/player/custom_player/legacy/ctm_swat_variantf.mdl",
	"models/player/custom_player/legacy/ctm_swat_variantg.mdl",
	"models/player/custom_player/legacy/ctm_swat_varianth.mdl",
	"models/player/custom_player/legacy/ctm_swat_varianti.mdl",
	"models/player/custom_player/legacy/ctm_swat_variantj.mdl",
	"models/player/custom_player/legacy/tm_professional_varf.mdl",
	"models/player/custom_player/legacy/tm_professional_varf1.mdl",
	"models/player/custom_player/legacy/tm_professional_varf2.mdl",
	"models/player/custom_player/legacy/tm_professional_varf3.mdl",
	"models/player/custom_player/legacy/tm_professional_varf4.mdl",
	"models/player/custom_player/legacy/tm_professional_varg.mdl",
	"models/player/custom_player/legacy/tm_professional_varh.mdl",
	"models/player/custom_player/legacy/tm_professional_vari.mdl",
	"models/player/custom_player/legacy/tm_professional_varj.mdl",
	"models/player/custom_player/legacy/tm_anarchist.mdl",
	"models/player/custom_player/legacy/tm_anarchist_varianta.mdl",
	"models/player/custom_player/legacy/tm_anarchist_variantb.mdl",
	"models/player/custom_player/legacy/tm_anarchist_variantc.mdl",
	"models/player/custom_player/legacy/tm_anarchist_variantd.mdl",
	"models/player/custom_player/legacy/tm_pirate.mdl",
	"models/player/custom_player/legacy/tm_pirate_varianta.mdl",
	"models/player/custom_player/legacy/tm_pirate_variantb.mdl",
	"models/player/custom_player/legacy/tm_pirate_variantc.mdl",
	"models/player/custom_player/legacy/tm_pirate_variantd.mdl",
	"models/player/custom_player/legacy/tm_professional.mdl",
	"models/player/custom_player/legacy/tm_professional_var1.mdl",
	"models/player/custom_player/legacy/tm_professional_var2.mdl",
	"models/player/custom_player/legacy/tm_professional_var3.mdl",
	"models/player/custom_player/legacy/tm_professional_var4.mdl",
	"models/player/custom_player/legacy/tm_separatist.mdl",
	"models/player/custom_player/legacy/tm_separatist_varianta.mdl",
	"models/player/custom_player/legacy/tm_separatist_variantb.mdl",
	"models/player/custom_player/legacy/tm_separatist_variantc.mdl",
	"models/player/custom_player/legacy/tm_separatist_variantd.mdl",
	"models/player/custom_player/legacy/ctm_gign.mdl",
	"models/player/custom_player/legacy/ctm_gign_varianta.mdl",
	"models/player/custom_player/legacy/ctm_gign_variantb.mdl",
	"models/player/custom_player/legacy/ctm_gign_variantc.mdl",
	"models/player/custom_player/legacy/ctm_gign_variantd.mdl",
	"models/player/custom_player/legacy/ctm_gsg9.mdl",
	"models/player/custom_player/legacy/ctm_gsg9_varianta.mdl",
	"models/player/custom_player/legacy/ctm_gsg9_variantb.mdl",
	"models/player/custom_player/legacy/ctm_gsg9_variantc.mdl",
	"models/player/custom_player/legacy/ctm_gsg9_variantd.mdl",
	"models/player/custom_player/legacy/ctm_idf.mdl",
	"models/player/custom_player/legacy/ctm_idf_variantb.mdl",
	"models/player/custom_player/legacy/ctm_idf_variantc.mdl",
	"models/player/custom_player/legacy/ctm_idf_variantd.mdl",
	"models/player/custom_player/legacy/ctm_idf_variante.mdl",
	"models/player/custom_player/legacy/ctm_idf_variantf.mdl",
	"models/player/custom_player/legacy/ctm_swat.mdl",
	"models/player/custom_player/legacy/ctm_swat_varianta.mdl",
	"models/player/custom_player/legacy/ctm_swat_variantb.mdl",
	"models/player/custom_player/legacy/ctm_swat_variantc.mdl",
	"models/player/custom_player/legacy/ctm_swat_variantd.mdl",
	"models/player/custom_player/legacy/ctm_sas_varianta.mdl",
	"models/player/custom_player/legacy/ctm_sas_variantb.mdl",
	"models/player/custom_player/legacy/ctm_sas_variantc.mdl",
	"models/player/custom_player/legacy/ctm_sas_variantd.mdl",
	"models/player/custom_player/legacy/ctm_st6.mdl",
	"models/player/custom_player/legacy/ctm_st6_varianta.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantb.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantc.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantd.mdl",
	"models/player/custom_player/legacy/tm_balkan_variante.mdl",
	"models/player/custom_player/legacy/tm_balkan_varianta.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantb.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantc.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantd.mdl",
	"models/player/custom_player/legacy/tm_jumpsuit_varianta.mdl",
	"models/player/custom_player/legacy/tm_jumpsuit_variantb.mdl",
	"models/player/custom_player/legacy/tm_jumpsuit_variantc.mdl",
	"models/player/custom_player/legacy/tm_phoenix_heavy.mdl",
	"models/player/custom_player/legacy/ctm_heavy.mdl",
	"models/player/custom_player/legacy/tm_leet_varianta.mdl",
	"models/player/custom_player/legacy/tm_leet_variantb.mdl",
	"models/player/custom_player/legacy/tm_leet_variantc.mdl",
	"models/player/custom_player/legacy/tm_leet_variantd.mdl",
	"models/player/custom_player/legacy/tm_leet_variante.mdl",
	"models/player/custom_player/legacy/tm_phoenix.mdl",
	"models/player/custom_player/legacy/tm_phoenix_varianta.mdl",
	"models/player/custom_player/legacy/tm_phoenix_variantb.mdl",
	"models/player/custom_player/legacy/tm_phoenix_variantc.mdl",
	"models/player/custom_player/legacy/tm_phoenix_variantd.mdl",
	"models/player/custom_player/legacy/ctm_fbi.mdl",
	"models/player/custom_player/legacy/ctm_fbi_varianta.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantc.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantd.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variante.mdl"
};

int getModelIndex(const char* modelName)
{
    int modelPrecacheTable = csgo.Read<int>(IClientState.address + Offset::signatures::dwModelPrecacheTable);
    int offset = csgo.Read<int>(modelPrecacheTable + 0x40);
    int items = csgo.Read<int>(offset + 0xC);
    int offset2 = items + 0xC;

    for (int i = 0; i < 1024; i++)
    {
        int nsdi_i = csgo.Read<int>(offset2 + (i * 0x34));
        char str[128] = { 0 };
        if (ReadProcessMemory(csgo.processHandle, (LPCVOID)nsdi_i, str, sizeof(str), NULL))
        {
            if (_stricmp(str, modelName) == 0)
            {
                return i;
            }
        }
    }
    return 0;
}

int getModelIndexByID(const short knifeID)
{
    switch (knifeID)
    {
    case WeaponID::Knife:
        return getModelIndex("models/weapons/v_knife_default_ct.mdl");
        
    case WeaponID::KnifeT:
        return getModelIndex("models/weapons/v_knife_default_t.mdl");
        
    case WeaponID::Bayonet:
        return getModelIndex("models/weapons/v_knife_bayonet.mdl");
        
    case WeaponID::Butterfly:
        return getModelIndex("models/weapons/v_knife_butterfly.mdl");
        
    case WeaponID::SurvivalKnife:
        return getModelIndex("models/weapons/v_knife_canis.mdl");
        
    case WeaponID::Paracord:
        return getModelIndex("models/weapons/v_knife_cord.mdl");
        
    case WeaponID::ClassicKnife:
        return getModelIndex("models/weapons/v_knife_css.mdl");
        
    case WeaponID::Falchion:
        return getModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
        
    case WeaponID::Flip:
        return getModelIndex("models/weapons/v_knife_flip.mdl");
        
    case WeaponID::Gut:
        return getModelIndex("models/weapons/v_knife_gut.mdl");
        
    case WeaponID::Navaja:
        return getModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
        
    case WeaponID::Karambit:
        return getModelIndex("models/weapons/v_knife_karam.mdl");
        
    case WeaponID::M9Bayonet:
        return getModelIndex("models/weapons/v_knife_m9_bay.mdl");
        
    case WeaponID::NomadKnife:
        return getModelIndex("models/weapons/v_knife_outdoor.mdl");
        
    case WeaponID::Daggers:
        return getModelIndex("models/weapons/v_knife_push.mdl");
        
    case WeaponID::SkeletonKnife:
        return getModelIndex("models/weapons/v_knife_skeleton.mdl");
        
    case WeaponID::Stiletto:
        return getModelIndex("models/weapons/v_knife_stiletto.mdl");
        
    case WeaponID::Bowie:
        return getModelIndex("models/weapons/v_knife_survival_bowie.mdl");

    case WeaponID::Huntsman:
        return getModelIndex("models/weapons/v_knife_tactical.mdl");

    case WeaponID::Ursus:
        return getModelIndex("models/weapons/v_knife_ursus.mdl");

    case WeaponID::Talon:
        return getModelIndex("models/weapons/v_knife_widowmaker.mdl");

    default:
        return 0;
    }
}

void Skin::add(int idx, short weaponID, int skinID, float wear, int seed, int statTrak, int quality, char nameTag[20]) {
    cfg->s[idx].weaponID = weaponID;
    cfg->s[idx].skinID = skinID;
    cfg->s[idx].wear = wear;
    cfg->s[idx].seed = seed;
    cfg->s[idx].statTrak = statTrak;
    cfg->s[idx].quality = quality;
    std::memcpy(cfg->s[idx].nameTag, nameTag, sizeof(nameTag));

	pleaseUpdate = true;
}

void Skin::update() {
    while (GUI::isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        const auto& weapons = csgo.Read<std::array<unsigned long, 8>>(localPlayer.get() + Offset::netvars::m_hMyWeapons);
        const int& knifeIndex = getModelIndexByID(Skin::knifeNames[localPlayer->teamNumber() == Team::CT ? cfg->ch.CTKnife : cfg->ch.TTKnife].definitionIndex);

        static const int knifeIndexCT = getModelIndexByID(WeaponID::Knife);
        static const int knifeIndexTT = getModelIndexByID(WeaponID::KnifeT);

        for (const auto& handle : weapons) {
            const auto& weapon = csgo.Read<intptr_t>((IClient.address + Offset::signatures::dwEntityList + (handle & 0xFFF) * 0x10) - 0x10);

            if (!weapon) continue;

            short weaponIndex = csgo.Read<short>(weapon + Offset::netvars::m_iItemDefinitionIndex);

            if (weaponIndex == WeaponID::Knife || weaponIndex == WeaponID::KnifeT) {
                int defIndex = Skin::knifeNames[localPlayer->teamNumber() == Team::CT ? cfg->ch.CTKnife : cfg->ch.TTKnife].definitionIndex;

                csgo.Write<short>(weapon + Offset::netvars::m_iItemDefinitionIndex, defIndex);
                csgo.Write<int>(weapon + Offset::netvars::m_nModelIndex, knifeIndex);
                weaponIndex = csgo.Read<short>(weapon + Offset::netvars::m_iItemDefinitionIndex);
            }

            if (weaponIndex > WeaponID::Flashbang && weaponIndex < WeaponID::IncGrenade)
                continue;
            int ID = 0;

            while (Skin::weaponNames[ID].definitionIndex != weaponIndex) {
                if (ID == Skin::weaponNames.size() - 1)
                    break;
                ID++;
            }

            if (csgo.Read<int>(weapon + Offset::netvars::m_iItemIDHigh) != -1)
                csgo.Write<int>(weapon + Offset::netvars::m_iItemIDHigh, -1);

            const int paint = cfg->s[ID].skinID;
            const bool shouldUpdate = csgo.Read<int32_t>(weapon + Offset::netvars::m_nFallbackPaintKit) != paint || pleaseUpdate;

            if (shouldUpdate) {
                csgo.Write<std::int32_t>(IClientState.address + 0x174, -1);
                pleaseUpdate = false;
            }

            csgo.Write<int32_t>(weapon + Offset::netvars::m_iItemIDHigh, -1);

            csgo.Write<int32_t>(weapon + Offset::netvars::m_nFallbackPaintKit, paint);
            csgo.Write<float>(weapon + Offset::netvars::m_flFallbackWear, cfg->s[ID].wear);

            csgo.Write<int>(weapon + Offset::netvars::m_nFallbackSeed, cfg->s[ID].seed);
            csgo.Write<int>(weapon + Offset::netvars::m_iEntityQuality, cfg->s[ID].quality);

            if (cfg->s[ID].statTrak > 1)
                csgo.Write<int32_t>(weapon + Offset::netvars::m_nFallbackStatTrak, cfg->s[ID].statTrak);

            if (strcmp(cfg->s[ID].nameTag, "\0")) {
                WriteProcessMemory(csgo.processHandle, (LPVOID)(weapon + Offset::netvars::m_szCustomName), cfg->s[ID].nameTag, sizeof(char[20]), 0);
            }

            csgo.Write<int32_t>(weapon + Offset::netvars::m_iAccountID, csgo.Read<int32_t>(weapon + Offset::netvars::m_OriginalOwnerXuidLow));

            if ((localPlayer->teamNumber() == Team::CT && cfg->ch.CTKnife != 0) || (localPlayer->teamNumber() == Team::TT && cfg->ch.TTKnife != 1)) {
                DWORD knifeViewModel = csgo.Read<DWORD>(localPlayer.get() + Offset::netvars::m_hViewModel) & 0xfff;
                knifeViewModel = csgo.Read<DWORD>(IClient.address + Offset::signatures::dwEntityList + (knifeViewModel - 1) * 0x10);

                if (knifeViewModel == 0) { continue; }

                const int nowModelIndex = csgo.Read<DWORD>(knifeViewModel + Offset::netvars::m_nModelIndex);
                if (nowModelIndex == knifeIndexTT || nowModelIndex == knifeIndexCT)
                {
                    int defIndex = Skin::knifeNames[localPlayer->teamNumber() == Team::CT ? cfg->ch.CTKnife : cfg->ch.TTKnife].definitionIndex;

                    csgo.Write<DWORD>(knifeViewModel + Offset::netvars::m_nModelIndex, knifeIndex);
                }
            }
        }
        const auto localPlayerModel = csgo.Read<DWORD>(localPlayer.get() + Offset::netvars::m_nModelIndex);
        if (cfg->ch.TTAgent || cfg->ch.CTAgent) {
            int modelIndex = localPlayer->teamNumber() == Team::TT ? cfg->ch.TTAgent : cfg->ch.CTAgent;
            int index = getModelIndex(models[modelIndex - 1]);
            if (!index) continue;
            if (csgo.Read<DWORD>(localPlayer.get() + Offset::netvars::m_nModelIndex) != index) {
                csgo.Write<DWORD>(localPlayer.get() + Offset::netvars::m_nModelIndex, index);
            }
        }
    }
}

const char* Skin::getWeaponIDName(short ID) noexcept {

    switch (ID) {
    default: return "?";
    case WeaponID::Ak47: return "AK-47";
    case WeaponID::Aug: return "AUG";
    case WeaponID::Awp: return "AWP";
    case WeaponID::Cz75a: return "CZ75 Auto";
    case WeaponID::Deagle: return "Desert Eagle";
    case WeaponID::Elite: return "Dual Berettas";
    case WeaponID::Famas: return "FAMAS";
    case WeaponID::Fiveseven: return "Five-SeveN";
    case WeaponID::G3SG1: return "G3SG1";
    case WeaponID::GalilAr: return "Galil AR";
    case WeaponID::Glock: return "Glock-18";
    case WeaponID::M249: return "M249";
    case WeaponID::M4a1_s: return "M4A1-S";
    case WeaponID::M4A1: return "M4A4";
    case WeaponID::Mac10: return "MAC-10";
    case WeaponID::Mag7: return "MAG-7";
    case WeaponID::Mp5sd: return "MP5-SD";
    case WeaponID::Mp7: return "MP7";
    case WeaponID::Mp9: return "MP9";
    case WeaponID::Negev: return "Negev";
    case WeaponID::Nova: return "Nova";
    case WeaponID::Hkp2000: return "P2000";
    case WeaponID::P250: return "P250";
    case WeaponID::P90: return "P90";
    case WeaponID::Bizon: return "PP-Bizon";
    case WeaponID::Revolver: return "R8 Revolver";
    case WeaponID::Sawedoff: return "Sawed-Off";
    case WeaponID::Scar20: return "SCAR-20";
    case WeaponID::Ssg08: return "SSG 08";
    case WeaponID::Sg553: return "SG 553";
    case WeaponID::Tec9: return "Tec-9";
    case WeaponID::Ump45: return "UMP-45";
    case WeaponID::Usp_s: return "USP-S";
    case WeaponID::Xm1014: return "XM1014";

    case WeaponID::Bayonet: return "Bayonet Knife";
    case WeaponID::Bowie: return "Bowie Knife";
    case WeaponID::Butterfly: return "Butterfly Knife";
    case WeaponID::ClassicKnife: return "Classic Knife";
    case WeaponID::Falchion: return "Falchion Knife";
    case WeaponID::Flip: return "Flip Knife";
    case WeaponID::Gut: return "Gut Knife";
    case WeaponID::Huntsman: return "Huntsman Knife";
    case WeaponID::Karambit: return "Karambit";
    case WeaponID::M9Bayonet: return "M9 Bayonet Knife";
    case WeaponID::Navaja: return "Navaja";
    case WeaponID::NomadKnife: return "Nomad Knife";
    case WeaponID::Paracord: return "Paracord Knife";
    case WeaponID::Daggers: return "Shadow Daggers";
    case WeaponID::SkeletonKnife: return "Skeleton Knife";
    case WeaponID::Stiletto: return "Stiletto";
    case WeaponID::SurvivalKnife: return "Survival Knife";
    case WeaponID::Talon: return "Talon Knife";
    case WeaponID::Ursus: return "Ursus Knife";
    case WeaponID::GoldenKnife: return "Gold Knife";
    case WeaponID::Knife:
    case WeaponID::KnifeT: return "Knife";
    case WeaponID::Shield: return "Shield";
    case WeaponID::Taser: return "Zeus x27";

    case WeaponID::Molotov: return "Molotov";
    case WeaponID::IncGrenade: return "Incendiary Grenade";
    case WeaponID::HeGrenade: return "Explosiveu Grenade";
    case WeaponID::SmokeGrenade: return "Smoke Grenade";
    case WeaponID::Decoy: return "Decoy";
    case WeaponID::Flashbang: return "Flashbang";
    }
}