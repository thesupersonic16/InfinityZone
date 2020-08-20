#include "IZAPI.h"
#include "InfinityZone.h"
extern "C"
{
    vector<StageLoadEvent> OnStageLoad;
    vector<StageLoadEvent> OnStageUnload;

    // Gets the version of InfinityZone
    IZ_EXPORT int GetIZVersion()
    {
        return IZ_VERSION;
    }

    // Gets the min supported version of IZAPI
    IZ_EXPORT int GetIZAPIMajorVersion()
    {
        return IZAPI_MAJORVERSION;
    }

    // Registers an event for when a stage loads
    IZ_EXPORT void RegisterStageLoadEvent(StageLoadEvent event)
    {
        OnStageLoad.push_back(event);
    }

    // Registers an event for when a stage unloads
    IZ_EXPORT void RegisterStageUnloadEvent(StageLoadEvent event)
    {
        OnStageUnload.push_back(event);
    }

    // Loads and registers all stages
    IZ_EXPORT void LoadStagesFile(const char* path)
    {
        IZInstance->LoadStages(path, true);
    }

    // Switches custom stages
    IZ_EXPORT void ChangeStage(const char* key, const char* sceneID)
    {
        if (sceneID)
            IZInstance->ChangeStage(key, sceneID);
        else
        {
            // Load default scene
            auto stage = IZInstance->FindIZStage(key);
            if (stage->Scenes.size())
                IZInstance->ChangeStage(key, stage->Scenes[0]->SceneID);
        }
    }

    // Performs an asset reset
    IZ_EXPORT void PerformAssetReset()
    {
        IZInstance->StartAssetReset();
    }

    // Gets the key of the current custom stage
    IZ_EXPORT StageInfo GetCurrentStage()
    {
        const auto scene = IZInstance->GetCurrentScene();
        if (scene == nullptr)
            return {nullptr, nullptr, nullptr, nullptr };
        return
        {
            scene->Parent->StageKey.c_str(),
            scene->Parent->StageID.c_str(),
            scene->Parent->StageName.c_str(),
            scene->SceneID.c_str()
        };
    }

    // Sets a stage asset redirect
    IZ_EXPORT void SetStageAsset(const char* stageKey, const char* basePath, const char* newPath)
    {
        if (stageKey)
        {
            auto stage = IZInstance->FindIZStage(stageKey);
            if (!newPath)
                stage->Assets.erase(string(basePath));
            else
                stage->Assets[string(basePath)] = string(newPath);
        }
    }
    // Sets a global asset redirect
    IZ_EXPORT void SetGlobalAsset(const char* basePath, const char* newPath)
    {
        if (!newPath)
            IZInstance->GlobalAssets.erase(string(basePath));
        else
            IZInstance->GlobalAssets[string(basePath)] = string(newPath);
    }

}