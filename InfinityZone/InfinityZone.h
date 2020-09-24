#pragma once
// Includes
#include "Common.h"
#include "IZStage.h"
#include "IZCategory.h"
#include "IZGameSession.h"
#include "Helpers.h"
#include "DevMenu.h"

using std::string;
using std::vector;

class InfinityZone
{
protected:
    IZScene* currentCustomScene = nullptr;
    short currentLevelID = 0;
    char resetting = 0;
    vector<string*> loadedStageLists;
public:
    unordered_map<string, string> GlobalAssets;
    vector<IZStage*> registeredStages;
    vector<IZScene*> registeredScenes;
    vector<IZCategory*> registeredCategories;
    string OnFileLoad(string path);
    IZStage* FindIZStage(string key);
    IZScene* FindIZScene(string sceneKey);
    void OnFrame();
    void OnActCompleted();
    void Init(string path);
    void LoadStages(string path, bool registerList);
    void LoadUnlockSets(string path);
    void ReloadStageLists();
    void StartAssetReset();
    void ChangeScene(IZScene* scene);
    IZScene* GetCurrentScene() const;
};

extern "C" extern InfinityZone* IZInstance;