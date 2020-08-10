#include "IZStage.h"
#include "depends\tinyxml2\tinyxml2.h"

using SonicMania::Filter;

static vector<IZStage_UnlockCode*> EnabledUnlocks;

IZStage::~IZStage()
{
    // Remove all scenes
    for (auto scene : Scenes)
        delete scene;
    Scenes.clear();
}

bool IZStage::LoadXML(tinyxml2::XMLElement* xmlStage)
{
    StageName = xmlStage->Attribute("stageName"); // Required
    StageID = xmlStage->Attribute("stageID");     // Required
    StageKey = xmlStage->Attribute("stageKey");   // Required
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
                LogWarn("InfinityZone::IZStage::LoadXML", "The unlock code \"%s\" was not found! Check Spelling!", child->Name());
        }
    }

    auto xmlAssets = xmlStage->FirstChildElement("StageAssets");
    if (xmlAssets)
    {
        for (auto child = xmlAssets->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
            Assets[child->Attribute("basePath")] = child->Attribute("newPath");
    }

    auto xmlScenes = xmlStage->FirstChildElement("Scenes");
    if (xmlScenes)
    {
        for (auto child = xmlScenes->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            const char* sceneID    = child->Attribute("id"); // Required
            const char* sceneName  = child->Attribute("name"); // Optional
            if (!sceneID)
            {
                LogError("InfinityZone::IZStage::LoadXML", "The id attribute for a scene from the stage \"%s\" was not found! Current scene will be skipped!", StageName.c_str());
                continue;
            }

            // Set name to ID if a name wasn't given
            if (!sceneName)
                sceneName = sceneID;

            auto scene = new IZScene();
            scene->Parent = this;
            scene->SceneID = sceneID;
            scene->SceneName = sceneName;
            scene->Flags = static_cast<Filter>(child->IntAttribute("flags", Flags));
            Scenes.push_back(scene);
        }
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
    // Write back the original code
    for (vector<IZStage_UnlockCode*>::reverse_iterator iter = EnabledUnlocks.rbegin(); iter != EnabledUnlocks.rend(); ++iter)
        WriteData((void*)(baseAddress + (*iter)->adress), (*iter)->restore, (*iter)->size);
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

