#pragma once
// Includes
#include "Common.h"
#include "IZStage.h"
#include "Helpers.h"
#include "DevMenu.h"

using std::string;
using std::vector;

class InfinityZone
{
protected:
    string* currentStageKey;
    short currentLevelID;
    char resetting;
    vector<string*> loadedStageLists;
public:
    vector<IZStage*> registeredStages;
    string OnFileLoad(string path);
    IZStage* FindIZStage(string key);
    void SetIZStage(IZStage* stage);
    void OnFrame();
    void OnActCompleted();
    void Init();
    void LoadStages(string path, bool registerList);
    void ReloadStageLists();
    void StartAssetReset();
    void ChangeStage(string id);
    IZStage* GetCurrentStage();
};

extern "C" extern InfinityZone* IZInstance;