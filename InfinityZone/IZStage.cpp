#include "IZStage.h"
#include "depends\tinyxml2\tinyxml2.h"

using SonicMania::Filter;

static vector<IZStage_UnlockCode*> EnabledUnlocks;

bool IZStage::LoadXML(tinyxml2::XMLElement* xmlStage)
{
    StageName = xmlStage->Attribute("stageName");
    StageID = xmlStage->Attribute("stageID");
    StageKey = xmlStage->Attribute("stageKey");
    Flags = (Filter)xmlStage->IntAttribute("flags", Filter::Filter_Common | Filter::Filter_Mania); // Filter_Common | Filter_Mania = 3

    auto xmlUnlocks = xmlStage->FirstChildElement("StageUnlocks");
    if (xmlUnlocks)
    {
        for (auto child = xmlUnlocks->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            auto unlock = FindUnlock(child->Name());
            if (unlock)
                for (auto& code : unlock->UnlockCodes)
                    Unlocks.push_back(&code);
            else
                std::cerr << "[InfinityZone::IZStage::LoadXML] Attribute \"" << child->Name() << "\" is not defined, aborting!" << std::endl;
        }
    }

    auto xmlAssets = xmlStage->FirstChildElement("StageAssets");
    if (xmlAssets)
    {
        for (auto child = xmlAssets->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
            Assets[child->Attribute("basePath")] = child->Attribute("newPath");
    }

    return true;
}

bool IZStage::EnableUnlocks()
{
    // Abort if there are codes currently active
    if (!EnabledUnlocks.empty())
        return false;

    // NOP bytes
    char nops[24];
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

