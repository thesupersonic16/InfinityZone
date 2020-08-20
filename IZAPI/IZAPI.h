#pragma once
#include <vector>
#include <windows.h>

extern "C"
{
    namespace IZAPI
    {
        // Enums
        enum StageLoadPhase
        {
            StageLoadPhase_NotLoaded,
            StageLoadPhase_Load,
            StageLoadPhase_Loaded
        };
        
        // Structs
        struct StageInfo
        {
            const char* StageKey;
            const char* StageID;
            const char* StageName;
            const char* SceneID;
        };


        // Function Types
        typedef void(__cdecl* StageLoadEvent)(StageInfo, StageLoadPhase);

        // Functions
        extern bool IZInit();

        extern HMODULE DetectIZDLL();

        // Gets the version of InfinityZone
        extern int GetIZVersion();

        // Gets the supported version of IZAPI
        extern int GetIZAPIMajorVersion();

        // Registers an event for when a stage loads
        extern void RegisterStageLoadEvent(StageLoadEvent event);

        // Registers an event for when a stage unloads
        extern void RegisterStageUnloadEvent(StageLoadEvent event);

        // Loads and registers all stages
        extern void LoadStagesFile(const char* path);

        // Switches custom stages
        extern void ChangeStage(const char* key, const char* sceneID = nullptr);

        // Performs an asset reset
        extern void PerformAssetReset();

        // Gets the key of the current custom stage
        extern StageInfo GetCurrentStage();

        // Sets a stage asset redirect
        extern void SetStageAsset(const char* stageKey, const char* basePath, const char* newPath = nullptr);
        
        // Sets a global asset redirect
        extern void SetGlobalAsset(const char* basePath, const char* newPath = nullptr);
    }
}