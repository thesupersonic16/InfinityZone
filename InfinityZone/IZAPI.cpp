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
    IZ_EXPORT void ChangeStage(const char* key)
    {
        IZInstance->ChangeStage(key);
    }

    // Performs an asset reset
    IZ_EXPORT void PerformAssetReset()
    {
        IZInstance->StartAssetReset();
    }

    // Gets the key of the current custom stage
    IZ_EXPORT StageInfo GetCurrentStage()
    {
        const auto stage = IZInstance->GetCurrentStage();
        if (stage == nullptr)
            return {};
        return
        {
            stage->StageKey.c_str(),
            stage->StageID.c_str(),
            stage->StageName.c_str()
        };
    }
}