#pragma once
enum WeaponID{
    NoID = 0,
    Deagle = 1,
    Elite,
    Fiveseven,
    Glock,
    Ak47 = 7,
    Aug,
    Awp,
    Famas,
    G3SG1,
    GalilAr = 13,
    M249,
    M4A1 = 16,
    Mac10,
    P90 = 19,
    ZoneRepulsor,
    Mp5sd = 23,
    Ump45,
    Xm1014,
    Bizon,
    Mag7,
    Negev,
    Sawedoff,
    Tec9,
    Taser,
    Hkp2000,
    Mp7,
    Mp9,
    Nova,
    P250,
    Shield,
    Scar20,
    Sg553,
    Ssg08,
    GoldenKnife,
    Knife,
    Flashbang = 43,
    HeGrenade,
    SmokeGrenade,
    Molotov,
    Decoy,
    IncGrenade,
    C4,
    Healthshot = 57,
    KnifeT = 59,
    M4a1_s,
    Usp_s,
    Cz75a = 63,
    Revolver,
    TaGrenade = 68,
    Axe = 75,
    Hammer,
    Spanner = 78,
    GhostKnife = 80,
    Firebomb,
    Diversion,
    FragGrenade,
    Snowball,
    BumpMine,
    Bayonet = 500,
    ClassicKnife = 503,
    Flip = 505,
    Gut,
    Karambit,
    M9Bayonet,
    Huntsman,
    Falchion = 512,
    Bowie = 514,
    Butterfly,
    Daggers,
    Paracord,
    SurvivalKnife,
    Ursus = 519,
    Navaja,
    NomadKnife,
    Stiletto = 522,
    Talon,
    SkeletonKnife = 525,
    NameTag = 1200,
    Sticker = 1209,
    MusicKit = 1314,
    SealedGraffiti = 1348,
    Graffiti = 1349,
    OperationHydraPass = 1352,
    CSGOWeaponCase = 4001,
    Esports2013Case = 4002,
    OperationBravoCase = 4003,
    CSGOWeaponCase2 = 4004,
    Esports2013WinterCase = 4005,
    WinterOffensiveCase = 4009,
    CSGOWeaponCase3 = 4010,
    OperationPhoenixCase = 4011,
    HuntsmanCase = 4017,
    OperationBreakoutCase = 4018,
    Esports2014SummerCase = 4019,
    OperationVanguardCase = 4029,
    ChromaCase = 4061,
    Chroma2Case = 4089,
    FalchionCase = 4091,
    ShadowCase = 4138,
    RevolverCase = 4186,
    OperationWildfireCase = 4187,
    Chroma3Case = 4233,
    GammaCase = 4236,
    Gamma2Case = 4281,
    GloveCase = 4288,
    SpectrumCase = 4351,
    OperationHydraCase = 4352,
    BronzeOperationHydraCoin = 4353,
    Spectrum2Case = 4403,
    ClutchCase = 4471,
    HorizonCase = 4482,
    DangerZoneCase = 4548,
    Katowice2019ViewerPass = 4554,
    Katowice2019BronzeCoin = 4555,
    PrismaCase = 4598,
    Patch = 4609,
    ShatteredWebCase = 4620,
    Berlin2019ViewerPass = 4622,
    Berlin2019BronzeCoin = 4623,
    Berlin2019ViewerPassWith3Tokens = 4627,
    Berlin2019SouvenirToken = 4628,
    CS20Case = 4669,
    Prisma2Case = 4695,
    FractureCase = 4698,
    GloveStuddedBrokenfang = 4725,
    OperationBrokenFangCase = 4717,
    SnakebiteCase = 4747,
    OperationRiptideCase = 4790,
    Stockholm2021ViewerPass = 4796,
    Stockholm2021BronzeCoin = 4797,
    Stockholm2021ViewerPassWith3Tokens = 4801,
    Stockholm2021SouvenirToken = 4802,
    DreamsAndNightmaresCase = 4818,
    Antwerp2022ViewerPass = 4825,
    Antwerp2022BronzeCoin = 4826,
    Antwerp2022ViewerPassWith3Tokens = 4830,
    Antwerp2022SouvenirToken = 4831,
    RecoilCase = 4846,
    Rio2022ViewerPass = 4850,
    Rio2022BronzeCoin = 4851,
    Rio2022ViewerPassWith3Tokens = 4855,
    GloveStuddedBloodhound = 5027,
    GloveT,
    GloveCT,
    GloveSporty,
    GloveSlick,
    GloveLeatherWrap,
    GloveMotorcycle,
    GloveSpecialist,
    GloveHydra,
    MAX_VALUE
};

constexpr int getWeaponIndex(WeaponID weaponId) noexcept {
    switch (weaponId) {
    default: return 0;

    case WeaponID::Glock: return 1;
    case WeaponID::Hkp2000: return 2;
    case WeaponID::Usp_s: return 3;
    case WeaponID::Elite: return 4;
    case WeaponID::P250: return 5;
    case WeaponID::Tec9: return 6;
    case WeaponID::Fiveseven: return 7;
    case WeaponID::Cz75a: return 8;
    case WeaponID::Deagle: return 9;
    case WeaponID::Revolver: return 10;

    case WeaponID::Nova: return 11;
    case WeaponID::Xm1014: return 12;
    case WeaponID::Sawedoff: return 13;
    case WeaponID::Mag7: return 14;
    case WeaponID::M249: return 15;
    case WeaponID::Negev: return 16;

    case WeaponID::Mac10: return 17;
    case WeaponID::Mp9: return 18;
    case WeaponID::Mp7: return 19;
    case WeaponID::Mp5sd: return 20;
    case WeaponID::Ump45: return 21;
    case WeaponID::P90: return 22;
    case WeaponID::Bizon: return 23;

    case WeaponID::GalilAr: return 24;
    case WeaponID::Famas: return 25;
    case WeaponID::Ak47: return 26;
    case WeaponID::M4A1: return 27;
    case WeaponID::M4a1_s: return 28;
    case WeaponID::Ssg08: return 29;
    case WeaponID::Sg553: return 30;
    case WeaponID::Aug: return 31;
    case WeaponID::Awp: return 32;
    case WeaponID::G3SG1: return 33;
    case WeaponID::Scar20: return 34;

    case WeaponID::Taser: return 39;
    }
}

constexpr int getWeaponClass(WeaponID weaponId) noexcept {
    switch (weaponId) {
    default: return 0;

    case WeaponID::Glock:
    case WeaponID::Hkp2000:
    case WeaponID::Usp_s:
    case WeaponID::Elite:
    case WeaponID::P250:
    case WeaponID::Tec9:
    case WeaponID::Fiveseven:
    case WeaponID::Cz75a:
    case WeaponID::Deagle:
    case WeaponID::Revolver: return 35;

    case WeaponID::Nova:
    case WeaponID::Xm1014:
    case WeaponID::Sawedoff:
    case WeaponID::Mag7:
    case WeaponID::M249:
    case WeaponID::Negev: return 36;

    case WeaponID::Mac10:
    case WeaponID::Mp9:
    case WeaponID::Mp7:
    case WeaponID::Mp5sd:
    case WeaponID::Ump45:
    case WeaponID::P90:
    case WeaponID::Bizon: return 37;

    case WeaponID::GalilAr:
    case WeaponID::Famas:
    case WeaponID::Ak47:
    case WeaponID::M4A1:
    case WeaponID::M4a1_s:
    case WeaponID::Ssg08:
    case WeaponID::Sg553:
    case WeaponID::Aug:
    case WeaponID::Awp:
    case WeaponID::G3SG1:
    case WeaponID::Scar20: return 38;
    }
}