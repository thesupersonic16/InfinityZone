#include "IZStageUnlocks.h"

vector<IZStage_Unlock> AllUnlocks =
{
    // Green Hill Zone (GHZ)
    { "GHZ_Batbrain",       {{ 0x000D9780, 2 }} }, 
    { "GHZ_Bridge",         {{ 0x0000D2B0, 2 }} }, 
    { "GHZ_BuzzBomber",     {{ 0x000E9461, 2 }} }, 
    { "GHZ_CheckerBall",    {{ 0x000F09E1, 2 }} }, 
    { "GHZ_Chopper",        {{ 0x000EDBB0, 2 }} }, 
    { "GHZ_Crabmeat",       {{ 0x000EE6A0, 2 }} }, 
    { "GHZ_Decoration",     {{ 0x00067101, 2 }} }, 
    { "GHZ_Fireball",       {{ 0x0000CCE1, 2 }} }, 
    { "GHZ_Motobug",        {{ 0x000EECA1, 2 }} }, 
    { "GHZ_Newtron",        {{ 0x000EF4A1, 2 }} }, 
    { "GHZ_Platform",       {{ 0x0008AB6A, 2 }} }, 
    { "GHZ_SpikeLog",       {{ 0x00188291, 2 }} }, 
    { "GHZ_Splats",         {{ 0x000EFE70, 2 }} }, 
    { "GHZ_ZipLine",        {{ 0x001A6541, 2 }} }, 

    // Chemical Plant Zone (CPZ)
    { "CPZ_Parallax",       {{ 0x0005EDFF, 2 }} },  
    { "CPZ_Ball",           {{ 0x000D6BA0, 2 }} }, 
    { "CPZ_Bubbler",        {{ 0x000DE130, 2 }} }, 
    { "CPZ_CaterkillerJr",  {{ 0x000ED386, 2 }} }, 
    { "CPZ_OneWayDoor",     {{ 0x001604DD, 2 }, { 0x0016047E, 2 }} },
    { "CPZ_SpeedBooster",   {{ 0x00133880, 2 }} }, 
    { "CPZ_Sweep",          {{ 0x001909AD, 2 }} }, 
    { "CPZ_Staircase",      {{ 0x0018E900, 6 }} },
    { "CPZ_SpringTube",     {{ 0x001952C6, 2 }} }, 
    { "CPZ_Decoration",     {{ 0x0006712E, 2 }} }, 
    { "CPZ_Platform",       {{ 0x0008ABC4, 2 }} }, 

    // Studiopolis Zone (SPZ) : Act 1
    { "SPZ1_Parallax",      {{ 0x0005EE2D, 2 }} },
    { "SPZ1_Boss",          {{ 0x0007D824, 2 }} },
    { "SPZ1_Canista",       {{ 0x000EB5E0, 2 }} },
    { "SPZ1_Circlebumper",  {{ 0x000F2A40, 2 }} },
    { "SPZ1_Directorchair", {{ 0x0010F681, 2 }} },
    { "SPZ1_Filmreel",      {{ 0x00126482, 2 }} },
    { "SPZ1_Micdrop",       {{ 0x0015BDD2, 2 }} },
    { "SPZ1_Rockemsockem",  {{ 0x00176112, 2 }} },
    { "SPZ1_Shutterbug",    {{ 0x00180122, 2 }} },
    { "SPZ1_Turbinaut",     {{ 0x00195B02, 2 }} },
    { "SPZ1_Decoration",    {{ 0x0006715C, 2 }} },
    { "SPZ1_Platform",      {{ 0x0008AB2E, 2 }} },
    { "SPZ1_Clapperboard",  {{ 0x000F33A2, 2 }} },

    // Studiopolis Zone (SPZ) : Act 2
    { "SPZ2_Canista",       {{ 0x000EB5FB, 2 }} },
    { "SPZ2_Pathinverter",  {{ 0x00162E71, 2 }} },
    { "SPZ2_Platform",      {{ 0x0008AB4C, 2 }} },
    { "SPZ2_Circlebumper",  {{ 0x000F2A5B, 2 }} },

    // Flying Battery Zone (FBZ)
    { "FBZ_WarpDoor",       {{ 0x000325A6, 6 }, { 0x00032758, 2 }, { 0x00033034, 2 }} },
    { "FBZ_Parallax",       {{ 0x0005EE5B, 2 }} }, 
    { "FBZ_Decoration",     {{ 0x0006718A, 2 }} }, 
    { "FBZ_Platform",       {{ 0x0008ABE2, 2 }} },
    { "FBZ_Blaster",        {{ 0x00025780, 2 }} },
    { "FBZ_Button",         {{ 0x00086670, 2 }} },
    { "FBZ_Current",        {{ 0x0001C4E2, 2 }} },
    { "FBZ_Electromagnet",  {{ 0x0011B44A, 2 }} },
    { "FBZ_Flamespring",    {{ 0x00128AD1, 2 }} },
    { "FBZ_HangGlider",     {{ 0x001230A9, 2 }} },
    { "FBZ_HangPoint",      {{ 0x00088C50, 2 }} },
    { "FBZ_MagSpikeball",   {{ 0x0011B261, 2 }} },
    { "FBZ_Spikes",         {{ 0x000A06AD, 2 }} },
    { "FBZ_Technosqueek",   {{ 0x00024ED0, 2 }} },
    { "FBZ_TubeSpring",     {{ 0x001952F2, 2 }} },
    { "FBZ_Tuesday",        {{ 0x00196FA6, 2 }} },
    { "FBZ_TwistingDoor",   {{ 0x0019C841, 2 }} },

    // Press Garden Zone (PSZ) : Act 1
    { "PSZ1_Platform",      {{ 0x0008AC00, 2 }} }, 
    { "PSZ1_Acetone",       {{ 0x000CF0A0, 2 }} }, 
    { "PSZ1_Crate",         {{ 0x00090EC0, 2 }} }, 
    { "PSZ1_DoorTrigger",   {{ 0x00111F31, 2 }} }, 
    { "PSZ1_Dragonfly",     {{ 0x001121F0, 2 }} }, 
    { "PSZ1_FrostThrower",  {{ 0x0012B660, 2 }} }, 
    { "PSZ1_Ice",           {{ 0x000A1C00, 2 }} }, 
    { "PSZ1_IceBomba",      {{ 0x001452C0, 2 }} }, 
    { "PSZ1_Ink",           {{ 0x000CEE81, 2 }} }, 
    { "PSZ1_InkWipe",       {{ 0x00145D51, 2 }} }, 
    { "PSZ1_JuggleSaw",     {{ 0x00147716, 2 }} }, 
    { "PSZ1_PSZDoor",       {{ 0x00111811, 2 }} }, 
    { "PSZ1_Petal",         {{ 0x00167EE0, 2 }} }, 
    { "PSZ1_Press",         {{ 0x0009F6E1, 2 }} }, 
    { "PSZ1_PrintBlock",    {{ 0x0016CDD0, 2 }} }, 
    { "PSZ1_SP500",         {{ 0x00181507, 2 }} }, 
    { "PSZ1_Splats",        {{ 0x000EFEA7, 2 }} }, 
    { "PSZ1_Woodrow",       {{ 0x001A48C0, 2 }} }, 

    // Press Garden Zone (PSZ) : Act 2
    { "PSZ2_Platform",      {{ 0x0008AC1E, 2 }} },
    { "PSZ2_Shinobi",       {{ 0x0007D857, 2 }} },
    { "PSZ2_Spikes",        {{ 0x000A06CB, 2 }} },
    { "PSZ2_FrostThrower",  {{ 0x0012B67B, 2 }} },
    { "PSZ2_Ice",           {{ 0x000A1C1B, 2 }} },
    { "PSZ2_Dragonfly",     {{ 0x0011220B, 2 }} },
    { "PSZ2_IceBomba",      {{ 0x001452DB, 2 }} },
    { "PSZ2_JuggleSaw",     {{ 0x00147731, 2 }} },
    { "PSZ2_Petal",         {{ 0x00167EFB, 2 }} },
    { "PSZ1_Woodrow",       {{ 0x001A48DB, 2 }} },
    
    // Stardust Speedway Zone (SSZ) : Multi-Act
    { "SSZ_RTeleporter",   {{ 0x00178D7E, 2 }} },

    // Stardust Speedway Zone (SSZ) : Act 1
    // Common Unlocks
    { "SSZ1_SpeedBooster",  {{ 0x0013392A, 2 }, { 0x00133900, 2 }, { 0x00133912, 6 }, { 0x133A8F, 2 }, { 0x133AA0, 2, new unsigned char[2] { 0xEB, 0x05 } }} },
    { "SSZ1_Platform",      {{ 0x0008AC3C, 2 }} }, 
    { "SSZ1_LaunchSpring",  {{ 0x0014A5A1, 2 }, { 0x14A4D7, 2, new unsigned char[2] { 0xEB, 0x5C } }, { 0x14A384, 2, new unsigned char[2] { 0xEB, 0x17 } } }},
    { "SSZ1_Dango",         {{ 0x00101E41, 2 }} },
    { "SSZ1_Hotaru",        {{ 0x00140E71, 2 }} },
    { "SSZ1_Kabasira",      {{ 0x00148BA1, 2 }} },
    { "SSZ1_Kanabun",       {{ 0x00149440, 2 }} },
    { "SSZ1_MSHologram",    {{ 0x00134077, 2 }} },
    { "SSZ1_Spark",         {{ 0x00182C83, 2 }} },
    { "SSZ1_HotauraMKII",   {{ 0x00144367, 2 }} },
    { "SSZ1_SpikeBall",     {{ 0x0018E379, 2 }} },
    // Act Specific
    { "SSZ1_SDashWheel",    {{ 0x001A58B0, 2 }, { 0x8794F, 2 }, { 0x875E9, 6 }}},
    { "SSZ1_RotatingSpike", {{ 0x00177B2F, 2 }} },
    { "SSZ1_JunctionWheel", {{ 0x00148850, 2 }} },
    { "SSZ1_Flowerpod",     {{ 0x0012A600, 2 }} },

    // Stardust Speedway Zone (SSZ) : Act 2
    // Notes: Need to test the Metal Sonic Fight
    // Common Unlocks
    { "SSZ2_LaunchSpring",  {{ 0x0014A5BD, 2 }, { 0x14A4E9, 2, new unsigned char[2] { 0xEB, 0x4A } } , { 0x14A396, 2, new unsigned char[2] { 0xEB, 0x05 } }  }},
    { "SSZ2_Platform",      {{ 0x0008AC5A, 2 }} },
    { "SSZ2_SpeedBooster",  {{ 0x0013392A, 2 }, { 0x00133900, 2 }, { 0x00133912, 6 }, { 0x0013392C, 7 }, { 0x00133931, 2 }, { 0x133A8F, 2 }, { 0x133AA0, 2, new unsigned char[2] { 0xEB, 0x05 } }} },
    { "SSZ2_Dango",         {{ 0x00101E5D, 2 }} },
    { "SSZ2_Hotaru",        {{ 0x00140E8D, 2 }} },
    { "SSZ2_Kabasira",      {{ 0x00148BBD, 2 }} },
    { "SSZ2_Kanabun",       {{ 0x0014945B, 2 }} },
    { "SSZ2_MSHologram",    {{ 0x00134093, 2 }} },
    { "SSZ2_HotauraMKII",   {{ 0x00144383, 2 }} },
    // Act Specific
    { "SSZ2_MetalSonic",    {{ 0x001366F1, 2 }} },

    // Hydrocity Zone (HCZ)
    { "HCZ_Decoration",     {{ 0x00067269, 2 }} },
    { "HCZ_Bridge",         {{ 0x0000D2DC, 2 }} },
    { "HCZ_Platform",       {{ 0x0008ACC4, 2 }} },
    { "HCZ_Blastoid",       {{ 0x000DB500, 2 }} },
    { "HCZ_Button",         {{ 0x00086707, 2 }} },
    { "HCZ_ButtonDoor",     {{ 0x000E8DDF, 2 }, { 0x000E8E71, 2 }} },
    { "HCZ_DiveEggman",     {{ 0x00105416, 2 }, { 0x00108426, 2 }} },
    { "HCZ_Fan",            {{ 0x0012193E, 6 }} },
    { "HCZ_Jaws",           {{ 0x00146560, 2 }} },
    { "HCZ_Jellygnite",     {{ 0x00146AAB, 2 }} },
    { "HCZ_MegaChopper",    {{ 0x00158CE0, 2 }} },
    { "HCZ_PointDexter",    {{ 0x0016B8F0, 2 }} },
    { "HCZ_ScrewMobile",    {{ 0x001064B6, 2 }} },
    { "HCZ_TurboSpiker",    {{ 0x001989D1, 2 }} },
    { "HCZ_Wake",           {{ 0x000196E9, 6 }} },

    // Mirage Saloon Zone (MSZ)
    // Notes: Need to Fix Act 1 (Non-Knuckles Crash)
    { "MSZ_Decoration",     {{ 0x0006720F, 2 }} }, 
    { "MSZ_Platform",       {{ 0x0008ACA6, 2 }} }, 
    { "MSZ_Armadilloid",    {{ 0x0006A530, 2 }} }, 
    { "MSZ_Bumpalo",        {{ 0x000E74D0, 2 }} }, 
    { "MSZ_Cactula",        {{ 0x000EAF86, 2 }} }, 
    { "MSZ_Vultron",        {{ 0x0006678B, 2 }} }, 
    { "MSZ_Ending",         {{ 0x0006723C, 2 }} }, 
    { "MSZ_HeavyMystic",    {{ 0x0007D8B3, 6 }} }, 
    { "MSZ_Rogues",         {{ 0x0007D95C, 6 }, { 0x0007d976, 6 }, { 0x0007d9ad, 6 }} },
    { "MSZ_RollerMKII",     {{ 0x00176480, 2 }} }, 
    { "MSZ_RotatingSpikes", {{ 0x00177B01, 2 }} }, 
    { "MSZ_SwingRope",      {{ 0x00191910, 2 }} }, 
    { "MSZ_Tornado",        {{ 0x00067784, 2 }} }, 
    { "MSZ_Cutscene",       {{ 0x00067771, 2 }} }, 
    { "MSZ_Parallax",       {{ 0x0005EEA0, 2, new unsigned char[2] { 0xEB, 0x6F }} } },

    // Oil Ocean Zone (OOZ) : Act 1/2
    { "OOZ_Platform",       {{ 0x0008ADC7, 2 }, { 0x8AC79, 6, new unsigned char[6] { 0xE9, 0x4B, 0x01, 0x00, 0x00, 0x90 }} } },
    { "OOZ_Aquis",          {{ 0x000D6154, 2 }} }, 
    { "OOZ_Fan",            {{ 0x0012193E, 6 }, { 0x00121A0E, 5 }} },
    { "OOZ_Hatch",          {{ 0x0013E174, 2 }} }, 
    { "OOZ_Octus",          {{ 0x0015FC44, 2 }} }, 
    { "OOZ_PullSwitch",     {{ 0x001709C4, 2 }} }, 
    { "OOZ_PushSpring",     {{ 0x00170BD4, 2 }} },
    { "OOZ_Sol",            {{ 0x00020574, 2 }} }, 
    { "OOZ_Valve",          {{ 0x00159F94, 2 }} },

    // Oil Ocean Zone (OOZ) : Act 2
    { "OOZ2_Parallax",      {{ 0x0005EEB3, 2 }} },

    // Lava Reef Zone (LRZ) : Act 1
    { "LRZ1_Platform",      {{ 0x0008AD16, 2 }} }, 
    { "LRZ1_Bridge",        {{ 0x0000D308, 2 }} },   
    { "LRZ1_Button",        {{ 0x0008668C, 2 }} }, 
    { "LRZ1_ButtonDoor",    {{ 0x000E8D91, 2 }, { 0x000E8E39, 2 }} },
    { "LRZ1_DrillerDroid",  {{ 0x000E1906, 2 }, { 0x0011357c, 2 }} },
    { "LRZ1_Fireworm",      {{ 0x000E4786, 2 }} },
    { "LRZ1_Iwamodoki",     {{ 0x000A3E66, 2 }} },
    { "LRZ1_LRZFireball",   {{ 0x00112D11, 2 }} },
    { "LRZ1_LRZRockPile",   {{ 0x000DFF39, 2 }, { 0x000292B5, 2, new unsigned char[2] { 0xEB, 0x3D } }} },
    { "LRZ1_LavaFall",      {{ 0x00112900, 2 }} },
    { "LRZ1_LavaGeyser",    {{ 0x000DE970, 2 }} },
    { "LRZ1_OrbitSpike",    {{ 0x00161627, 2 }} },
    { "LRZ1_Rexon",         {{ 0x00173196, 2 }} },
    { "LRZ1_Rockdrill",     {{ 0x00174D46, 2 }} },
    { "LRZ1_Stalactite",    {{ 0x000E1136, 2 }} },
    { "LRZ1_Toxomister",    {{ 0x000E52D6, 2 }} },
    { "LRZ1_WalkerLegs",    {{ 0x001A1B61, 2 }} },

    // Lava Reef Zone (LRZ) : Act 2
    { "LRZ2_Platform",      {{ 0x0008AD34, 2 }} },
    { "LRZ2_Button",        {{ 0x000866EB, 2 }} },
    { "LRZ2_ButtonDoor",    {{ 0x000E8DB8, 2 }, { 0x000E8E55, 2 }} },
    { "LRZ2_Fireworm",      {{ 0x000E47A1, 2 }, { 0x000E47B3, 2 }} },
    { "LRZ2_Iwamodoki",     {{ 0x000A3E93, 2 }, { 0x000A3E81, 2 }} },
    { "LRZ2_LRZRockPile",   {{ 0x000DFF8C, 2 }, { 0x000292C7, 2, new unsigned char[2] { 0xEB, 0x2B }}, { 0xE0849, 6 }, { 0x1C569A, 6 } } },
    { "LRZ2_LavaGeyser",    {{ 0x000DE98B, 2 }} },
    { "LRZ2_OrbitSpike",    {{ 0x00161643, 2 }} },
    { "LRZ2_Parallax",      {{ 0x0005EEE1, 2 }, { 0x0005EEF4, 2 }} },
    { "LRZ2_Rexon",         {{ 0x001731B1, 2 }, { 0x001731C3, 2 }} },
    { "LRZ2_Toxomister",    {{ 0x000E52F1, 2 }, { 0x000E5303, 2 }} },
    { "LRZ2_Flamethrower",  {{ 0x00075924, 2 }} },
    { "LRZ2_ParallaxSprite",{{ 0x0005EEF4, 2 }} },
    { "LRZ2_Turbine",       {{ 0x001983F1, 2 }} },
    { "LRZ2_WalkerLegs",    {{ 0x001A1B8A, 2 }} },
    { "LRZ2_Unknown1",      {{ 0x000A74A2, 6 }} }, 
    { "LRZ2_Unknown2",      {{ 0x000E8F0C, 2, new unsigned char[2] { 0xEB, 0x15 }} } },
    { "LRZ2_Unknown3",      {{ 0x001A270B, 2 } } }, 

    // Lava Reef Zone (LRZ) : Act 3
    { "LRZ3_HeavyKing",     {{ 0x0007D900, 2 }} },
    { "LRZ3_HeavyRider",    {{ 0x0007D8C0, 6 }} },

    // Metallic Madness Zone (MMZ)
    { "MMZ_Platform",       {{ 0x0008ABA6, 2 }} },
    { "MMZ_Decoration",     {{ 0x0016AEB0, 2 }} },
    { "MMZ_Scarab",         {{ 0x00179360, 2 }} },
    { "MMZ_Bomb",           {{ 0x000DC1E0, 2 }} },
    { "MMZ_Button",         {{ 0x00086641, 2 }} },
    { "MMZ_Caterkiller",    {{ 0x000EC410, 2 }} },
    { "MMZ_ConveyorWheel",  {{ 0x000F60A1, 2 }, { 0x000F6511, 2 }} },
    { "MMZ_Matryoshkabom",  {{ 0x00157610, 2 }} }, 
    { "MMZ_Mechabu",        {{ 0x001583A7, 2 }} }, 
    { "MMZ_OneWayDoor",     {{ 0x001604C1, 2 }, { 0x00160436, 2 }, { 0x0016044D, 2 }} },
    { "MMZ_Orbinaut",       {{ 0x00160A10, 2 }} },
    { "MMZ_PohBee",         {{ 0x0016AEB0, 2 }} },
    { "MMZ_Scarab",         {{ 0x00179360, 2 }} },

    // Titanic Monarch Zone (TMZ) : Multi-Act
    { "TMZ_WarpDoor",       {{ 0x00032785, 6 }, { 0x00032758, 2 }, { 0x323FE, 6 }, { 0x6706A, 2 } }},
    { "TMZ_Decoration",     {{ 0x00067296, 2 }} },
    { "TMZ_Ballhog",        {{ 0x000D81F2, 2 }} },
    { "TMZ_Button",         {{ 0x00086723, 2 }} },
    { "TMZ_FlasherMKII",    {{ 0x00129333, 2 }} },
    { "TMZ_GymBar",         {{ 0x0001FC82, 2 }} },
    { "TMZ_MagnetSphere",   {{ 0x00156DC8, 2 }} },
    { "TMZ_SentryBug",      {{ 0x0017B313, 2 }} },
    { "TMZ_TeeterTotter",   {{ 0x001926E2, 2 }} },
    { "TMZ_WallBumper",     {{ 0x001A2E92, 2 }} },
    { "TMZ_LaunchSpring",   {{ 0x0014A4FB, 2, new unsigned char[2] { 0xEB, 0x12 } }} },

    // Titanic Monarch Zone (TMZ) : Act 1
    { "TMZ1_Platform",      {{ 0x0008AD6D, 2 }} },

    // Titanic Monarch Zone (TMZ) : Act 2
    { "TMZ2_Setup",         {{ 0x0008037E, 6 } } },
    { "TMZ2_Platform",      {{ 0x0008AD88, 2 } } },

    // Angel Island Zone (AIZ)
    { "AIZ_Portal",         {{ 0x00080455, 2 }} },
    { "AIZ_Parallax",       {{ 0x0005EDD1, 2 }} },
    { "AIZ_Decoration",     {{ 0x0005EDD1, 2 }} },
    { "AIZ_Platform",       {{ 0x0008ADA3, 2 }} },
    { "AIZ_Claw",           {{ 0x000CFCC0, 2 }} },
    { "AIZ_Bloominator",    {{ 0x000DBBC1, 2 }} },
    { "AIZ_CaterkillerJr",  {{ 0x000ED3A1, 2 }} },
    { "AIZ_Rhinobot",       {{ 0x00173EE1, 2 }} },
    { "AIZ_Sweep",          {{ 0x00190991, 2 }} },
    { "AIZ_SwingRope",      {{ 0x0019192B, 2 }} },
    { "AIZ_AIZTornado",     {{ 0x000D0E64, 2 }} },

    // Hidden Palace Zone (HPZ - UNUSED)
    { "HPZ_Jellygnite",     {{ 0x00146A90, 2 }} },
    { "HPZ_Redz",           {{ 0x00172750, 2 }} },
    { "HPZ_Stegway",        {{ 0x0018F070, 2 }} },
    { "HPZ_Batbot",         {{ 0x000D90C0, 2 }} },

    // UI/Internal
    { "Summary_UIPicture",  {{ 0x00151148, 2 }} },

    // Dummy - Please don't actually use this
    { "Dummy",              {{ 0x00000000, 2 }} }
};
