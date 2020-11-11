#pragma once
#include <vector>
#include <windows.h>
#include "SonicMania.h"

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
            const char* stageKey;
            const char* stageDir;
            const char* stageName;
            const char* sceneKey;
            const char* sceneID;
            const char* sceneName;
            SonicMania::Filter sceneFlags;
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

        // Loads a custom scene
        extern void ChangeScene(const char* sceneKey);

        // Performs an asset reset
        extern void PerformAssetReset();

        // Gets the key of the current custom stage
        extern StageInfo GetCurrentStage();

        // Sets a stage asset redirect
        extern void SetStageAsset(const char* stageKey, const char* basePath, const char* newPath = nullptr);
        
        // Sets a global asset redirect
        extern void SetGlobalAsset(const char* basePath, const char* newPath = nullptr);

        // Gets the list of scenes from IZ
        extern void GetIZScenes(StageInfo* buffer, size_t limit);

        // Gets the list of scenes from IZ
        extern size_t GetIZSceneCount();

    }
}
