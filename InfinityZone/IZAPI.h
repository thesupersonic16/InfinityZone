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
        const char* StageKey;
        const char* StageID;
        const char* StageName;
        const char* SceneID;
    };

    typedef void(__cdecl* StageLoadEvent)(StageInfo, StageLoadPhase);

    extern vector<StageLoadEvent> OnStageLoad;
    extern vector<StageLoadEvent> OnStageUnload;
}