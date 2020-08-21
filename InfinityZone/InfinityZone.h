#pragma once
// Includes
#include "Common.h"
#include "IZStage.h"
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
    string OnFileLoad(string path);
    IZStage* FindIZStage(string key);
    IZScene* FindIZScene(const IZStage* stage, string id);
    void RegisterIZStage(IZStage* stage);
    void OnFrame();
    void OnActCompleted();
    void Init(string path);
    void LoadStages(string path, bool registerList);
    void LoadUnlockSets(string path);
    void ReloadStageLists();
    void StartAssetReset();
    void ChangeStage(string id, string sceneID);
    void ChangeScene(IZScene* scene);
    IZScene* GetCurrentScene() const;
};

extern "C" extern InfinityZone* IZInstance;