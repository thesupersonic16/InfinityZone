#pragma once
#include "Common.h"

extern "C"
{

    struct StageInfo
    {
        const char* StageKey;
        const char* StageID;
        const char* StageName;
    };

    typedef void(__cdecl* StageLoadEvent)(StageInfo);

    extern vector<StageLoadEvent> OnStageLoad;
    extern vector<StageLoadEvent> OnStageUnload;
}