#include "IZStage.h"
#include "depends\tinyxml2\tinyxml2.h"

using SonicMania::Filter;

static vector<IZStage_UnlockCode*> EnabledUnlocks;

bool IZStage::LoadXML(string path)
{
    int size = 0;
    void* xml = LoadAndReadFile(path.c_str(), &size);
    if (xml && size)
    {
        tinyxml2::XMLDocument document;
        document.Parse(static_cast<const char*>(xml), size);

        // Read XML data
        auto xmlStage = document.FirstChildElement("Stage");
        StageName = xmlStage->Attribute("stageName");
        StageID   = xmlStage->Attribute("stageID");
        Flags     = (Filter)xmlStage->IntAttribute("flags", Filter::Filter_Common | Filter::Filter_Mania); // Filter_Common | Filter_Mania = 3

        auto xmlUnlocks = xmlStage->FirstChildElement("StageUnlocks");
        for (auto child = xmlUnlocks->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
        {
            auto unlock = FindUnlock(child->Name());
            if (unlock)
                for (auto &code : unlock->UnlockCodes)
                    Unlocks.push_back(&code);
            else
                printf("[InfinityZone::IZStage::LoadXML] Unlock \"%s\" was not found! Check Spelling?\n", child->Name());
        }

        auto xmlAssets = xmlStage->FirstChildElement("StageAssets");
        for (auto child = xmlAssets->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
            Assets[child->Attribute("basePath")] = child->Attribute("newPath");

        delete xml;
        return true;
    }
    return false;
}

bool IZStage::EnableUnlocks()
{
    // Abort if there are codes currently active
    if (!EnabledUnlocks.empty())
        return false;

    // NOP bytes
    char nops[8];
    memset(nops, 0x90, sizeof nops);
    
    // Write all the unlocks
    for (auto unlock : Unlocks)
    {
        auto address = (void*)(baseAddress + unlock->adress);
        memcpy(unlock->restore, address, unlock->size);
        WriteData(address, nops, unlock->size);
        EnabledUnlocks.push_back(unlock);
    }
    return true;
}

bool IZStage::DisableUnlocks()
{
    // Write all the unlocks
    for (auto code : EnabledUnlocks)
        WriteData((void*)(baseAddress + code->adress), code->restore, code->size);
    EnabledUnlocks.clear();
    return true;
}

static IZStage_Unlock* FindUnlock(string name)
{
    for (auto& unlock : AllUnlocks)
    {
        if (!name.compare(unlock.UnlockName))
            return &unlock;
    }
    return nullptr;
}

static vector<IZStage_Unlock> AllUnlocks = 
{
    // Green Hill Zone (GHZ)
    { "GHZ_Batbrain",       {{ 0x000D9780, 2} }}, // Untested
    { "GHZ_Bridge",         {{ 0x0000D2B0, 2} }}, // Untested
    { "GHZ_BuzzBomber",     {{ 0x000E9461, 2} }}, // Untested
    { "GHZ_CheckerBall",    {{ 0x000F09E1, 2} }}, // Untested
    { "GHZ_Chopper",        {{ 0x000ED880, 2} }}, // Untested
    { "GHZ_Crabmeat",       {{ 0x000EE6A0, 2} }}, // Untested
    { "GHZ_Decoration",     {{ 0x00067101, 2} }}, // Untested
    { "GHZ_Fireball",       {{ 0x0000CCE1, 2} }}, // Untested
    { "GHZ_Motobug",        {{ 0x000EECA1, 2} }}, // Untested
    { "GHZ_Newtron",        {{ 0x000EF4A1, 2} }}, // Untested
    { "GHZ_Platform",       {{ 0x0008AB6A, 2} }}, // Untested
    { "GHZ_SpikeLog",       {{ 0x00188291, 2} }}, // Untested
    { "GHZ_Splats",         {{ 0x000EFE70, 2} }}, // Untested
    { "GHZ_ZipLine",        {{ 0x001A6541, 2} }}, // Untested

    // Chemical Plant Zone (CPZ)
    { "CPZ_Ball",           {{ 0x000D6BA0, 2} }},  // Untested
    { "CPZ_Bubbler",        {{ 0x000DE130, 2} }},  // Untested
    { "CPZ_CaterkillerJr",  {{ 0x000ED386, 2} }},  // Untested
    { "CPZ_OneWayDoor",     {{ 0x001604DD, 2} }},  // Untested
    { "CPZ_SpeedBooster",   {{ 0x00133880, 2} }},  // Untested
    { "CPZ_Sweep",          {{ 0x001909AD, 2} }},  // Untested
    { "CPZ_SpringTube",     {{ 0x001952C6, 2} }},  // Untested
    { "CPZ_Decoration",     {{ 0x0006712E, 2} }},  // Untested
    { "CPZ_Platform",       {{ 0x0008ABC4, 2} }},  // Untested

    // Studiopolis Zone (SPZ)
    { "SPZ1_Boss",          {{ 0x0007D824, 2} }}, // Untested
    { "SPZ1_Canista",       {{ 0x000EB5E0, 2} }}, // Untested
    { "SPZ1_Circlebumper",  {{ 0x000F2A40, 2} }}, // Untested
    { "SPZ1_Clapperboard",  {{ 0x000F33A4, 2} }}, // Untested
    { "SPZ1_Directorchair", {{ 0x0010F681, 2} }}, // Untested
    { "SPZ1_Filmreel",      {{ 0x00126482, 2} }}, // Untested
    { "SPZ1_Micdrop",       {{ 0x0015BDD2, 2} }}, // Untested
    { "SPZ1_Rockemsockem",  {{ 0x00176114, 2} }}, // Untested
    { "SPZ1_Shutterbug",    {{ 0x00180122, 2} }}, // Untested
    { "SPZ1_Turbinaut",     {{ 0x00195B02, 2} }}, // Untested
    { "SPZ2_Canista",       {{ 0x000EB5FB, 2} }}, // Untested
    { "SPZ2_Pathinverter",  {{ 0x00162E71, 2} }}, // Untested
    { "SPZ1_Decoration",    {{ 0x0006715C, 2} }}, // Untested
    { "SPZ1_Platform",      {{ 0x0008AB2E, 2} }}, // Untested
    { "SPZ2_Platform",      {{ 0x0008AB4C, 2} }}, // Untested

    // Flying Battery Zone (FBZ)
    { "FBZ_Decoration",     {{ 0x0006718A, 2} }}, // Untested
    { "FBZ_Platform",       {{ 0x0008ABE2, 2} }}, // Untested

    // Press Garden Zone (PSZ)
    { "PSZ1_Platform",      {{ 0x0008AC02, 2} }}, // Untested
    { "PSZ2_Platform",      {{ 0x0008AC1E, 2} }}, // Untested

    // Stardust Speedway Zone (SSZ)
    { "SSZ_SDashWheel",     {{ 0x001A58B0, 2} }},
    { "SSZ1_Platform",      {{ 0x0008AC3C, 2} }}, // Untested
    { "SSZ2_Platform",      {{ 0x0008AC5A, 2} }}, // Untested

    // Hydrocity Zone (HCZ)
    { "HCZ_Decoration",     {{ 0x00067269, 2} }},
    { "HCZ_Bridge",         {{ 0x0000D2DC, 2} }},
    { "HCZ_Platform",       {{ 0x0008ACC4, 2} }},

    // Mirage Saloon Zone (MSZ)
    { "MSZ_Decoration",     {{ 0x0006720F, 2} }}, // Untested
    { "MSZ_Platform",       {{ 0x0008ACA6, 2} }}, // Untested

    // Oil Ocean Zone (OOZ)
    { "OOZ_Platform",       {{ 0x0008ADC7, 2} }}, // Untested

    // Lava Reef Zone (LRZ)
    { "LRZ1_Platform",      {{ 0x0008AD16, 2} }}, // Untested
    { "LRZ2_Platform",      {{ 0x0008AD34, 2} }}, // Untested

    // Metallic Madness Zone (MMZ)
    { "MMZ_Platform",       {{ 0x0008ABA6, 2} }}, // Untested

    // Titanic Monarch Zone (TMZ)
    { "TMZ1_Platform",    {{ 0x0008AD6D, 2} }}, // Untested

    // Angel Island Zone (AIZ)
    { "AIZ_Decoration",   {{ 0x0005EDD1, 2} }}, // Untested
    { "AIZ_Platform",     {{ 0x0008ADA3, 2} }}, // Untested

    // Hidden Palace Zone (HPZ - Unused)
    { "HPZ_Jellygnite",  {{ 0x00146A90, 2} }},
    { "HPZ_Redz",        {{ 0x00172750, 2} }},
    { "HPZ_Stegway",     {{ 0x0018F070, 2} }},
    { "HPZ_Batbot",      {{ 0x000D90C0, 2} }},


    // Dummy - Please don't actually use this
    { "Dummy",           {{ 0x00000000, 2} }}

};

