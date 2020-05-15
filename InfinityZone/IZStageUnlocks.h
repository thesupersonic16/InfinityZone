#pragma once
#include "Common.h"

struct IZStage_UnlockCode
{
    intptr_t adress;
    size_t size;
    char restore[8];
};

struct IZStage_Unlock
{
    string UnlockName;
    std::vector<IZStage_UnlockCode> UnlockCodes;
};

extern vector<IZStage_Unlock> AllUnlocks;
