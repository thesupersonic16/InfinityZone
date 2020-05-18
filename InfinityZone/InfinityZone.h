#pragma once
// Includes
#include "Common.h"
#include "IZStage.h"
#include "Helpers.h"
#include "DevMenu.h"

using std::string;
using std::vector;
using std::unordered_map;

class InfinityZone
{
protected:
    string* currentStageKey;
    short currentLevelID;
    char resetting;
public:
    unordered_map<string, IZStage*> registeredStages;
    string OnFileLoad(string path);
    void OnFrame();
    void Init();
    void LoadStages(string path);
    void StartAssetReset();
    void ChangeStage(string id);
};

extern "C" extern InfinityZone* IZInstance;