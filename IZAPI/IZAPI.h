#pragma once
#include <vector>

extern "C"
{
    namespace IZAPI
    {
        
        // Structs
        struct StageInfo
        {
            const char* StageKey;
            const char* StageID;
            const char* StageName;
        };

        // Function Types
        typedef void(__cdecl* StageLoadEvent)(StageInfo);

        // Functions
        extern void IZInit();

        // Registers an event for when a stage loads
        extern void RegisterStageLoadEvent(StageLoadEvent event);

        // Registers an event for when a stage unloads
        extern void RegisterStageUnloadEvent(StageLoadEvent event);

        // Loads and registers all stages
        extern void LoadStagesFile(const char* path);

        // Switches custom stages
        extern void ChangeStage(const char* key);

        // Performs an asset reset
        extern void PerformAssetReset();

        // Gets the key of the current custom stage
        extern StageInfo GetCurrentStage();
    }
}