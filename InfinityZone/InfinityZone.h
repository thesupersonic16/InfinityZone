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
    IZScene* currentCustomScene;
    short currentLevelID;
    char resetting;
    vector<string*> loadedStageLists;
public:
    vector<IZStage*> registeredStages;
    vector<IZScene*> registeredScenes;
    string OnFileLoad(string path);
    IZStage* FindIZStage(string key);
    IZScene* FindIZScene(const IZStage* stage, string id);
    void SetIZStage(IZStage* stage);
    void OnFrame();
    void OnActCompleted();
    void Init();
    void LoadStages(string path, bool registerList);
    void ReloadStageLists();
    void StartAssetReset();
    void ChangeStage(string id, string sceneID);
    void ChangeScene(IZScene* scene);
    IZScene* GetCurrentScene() const;
};

extern "C" extern InfinityZone* IZInstance;