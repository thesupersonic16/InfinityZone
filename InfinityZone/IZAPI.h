#pragma once
#include "Common.h"

extern "C"
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

    typedef void(__cdecl* StageLoadEvent)(StageInfo, StageLoadPhase);

    extern vector<StageLoadEvent> OnStageLoad;
    extern vector<StageLoadEvent> OnStageUnload;
}