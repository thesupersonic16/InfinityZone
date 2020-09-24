#include "IZScene.h"
#include "IZStage.h"

using SonicMania::Filter;

bool IZScene::LoadXML(tinyxml2::XMLElement* xmlScene, vector<IZStage*> stages)
{
    auto stageKey   = xmlScene->Attribute("stageKey");
    SceneKey        = xmlScene->Attribute("sceneKey");
    SceneID         = xmlScene->Attribute("sceneID");
    Flags           = (Filter)xmlScene->IntAttribute("sceneFlags", Filter::Filter_Common | Filter::Filter_Mania);
    SceneName       = xmlScene->Attribute("sceneName");

    for (auto stage : stages)
    {
        if (stage->StageKey == stageKey)
        {
            Parent = stage;
            break;
        }
    }
    if (!Parent)
    {
        LogError("InfinityZone::IZScene::LoadXML", "Attempted to load scene for a non-existent Stage \"%s\"! This scene will not be loaded!", SceneKey.c_str());
        return false;
    }
    return true;
}