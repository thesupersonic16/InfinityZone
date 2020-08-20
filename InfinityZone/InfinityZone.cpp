#include "InfinityZone.h"
#include "IZAPI.h"
#include "depends\tinyxml2\tinyxml2.h"
#include <fstream>
#include <algorithm>
#include "../IZAPI/IZAPI.h"

#define RaseStageEvent(stage, handler, phase) \
for (auto& event : handler) \
event({ \
        stage->StageKey.c_str(), \
        stage->StageID.c_str(), \
        stage->StageName.c_str(), \
    }, phase); \

static bool TrackerL, TrackerR, InSpecialStage;

bool GetCtrlKeyState()
{
    if (SonicMania::MainWindowHandle == GetActiveWindow())
        return GetAsyncKeyState(VK_LCONTROL) & 1 << 15 || GetAsyncKeyState(VK_RCONTROL) & 1 << 15;
    return false;
}

bool CheckKey(char key, bool state, bool* tracker)
{
    state &= static_cast<bool>(GetAsyncKeyState(key) & 1 << 15);
    if (*tracker && !state)
        *tracker = false;
    if (!*tracker && state)
    {
        *tracker = true;
        return true;
    }
    return false;
}


IZStage* InfinityZone::FindIZStage(const string key)
{
    for (IZStage* stage : registeredStages)
    {
        if (stage->StageKey == key)
            return stage;
    }
    return nullptr;
}

IZScene* InfinityZone::FindIZScene(const IZStage* stage, string id)
{
    if (!stage)
        return nullptr;
    for (IZScene* scene : stage->Scenes)
    {
        if (scene->SceneID == id)
            return scene;
    }
    return nullptr;
}

void InfinityZone::RegisterIZStage(IZStage* stage)
{
    // Check if stage exists
    auto izStage = FindIZStage(stage->StageKey);
    if (izStage)
    {
        // Clear
        registeredStages.erase(std::remove(registeredStages.begin(), registeredStages.end(), izStage));
        for (auto scene : izStage->Scenes)
            registeredScenes.erase(std::remove(registeredScenes.begin(), registeredScenes.end(), scene));
    }
    registeredStages.push_back(stage);
    if (stage)
    {
        for (auto scene : stage->Scenes)
            registeredScenes.push_back(scene);
    }
}

string InfinityZone::OnFileLoad(string path)
{
    // Load Null Space Zone when performing asset resetting
    if (resetting && resetting < 3)
    {
        strcpy_s(SonicMania::CurrentSceneName, 12, "IZ_NSZ");
        return path;
    }

    auto oldStageID = string(SonicMania::CurrentSceneName);

    // Check if a custom stage is loaded
    if (!currentCustomScene || InSpecialStage)
        return path;

    // The loaded custom scene
    auto &scene = *currentCustomScene;

    // Set custom scene flags
    SonicMania::SceneFlags = scene.Flags;

    // Replace the stage ID with the custom stage ID from the file path
    ReplaceString(path, oldStageID, scene.Parent->StageID);

    // Load custom stage assets
    auto iter = scene.Parent->Assets.find(path);
    if (iter != scene.Parent->Assets.cend())
    {
        path = iter->second;
        LogDebug("InfinityZone::OnFileLoad", "Loading Stage Asset: %s", path.c_str());
    }else
    {
        auto position = path.find("/Scene");
        if (position != string::npos && path.find(".bin") != string::npos)
        {
            position += 6;
            return path.substr(0, position) + currentCustomScene->SceneID + ".bin";
        }
    }
    iter = GlobalAssets.find(path);
    if (iter != GlobalAssets.cend())
    {
        path = iter->second;
        LogDebug("InfinityZone::OnFileLoad", "Loading Global Asset: %s", path.c_str());
    }
    return path;
}

void InfinityZone::OnFrame()
{
    // Asset Reset
    if (SonicMania::GameState & SonicMania::GameState_Running && resetting == 2)
    {
        LogDebug("InfinityZone::OnFrame", "Finalising asset reset...");

        SonicMania::CurrentScene = SonicMania::Scene_ThanksForPlaying;
        SonicMania::GameState = SonicMania::GameState_NotRunning;
        if (currentCustomScene)
        {
            auto stage = currentCustomScene->Parent;
            if (stage)
                RaseStageEvent(stage, OnStageLoad, StageLoadPhase_Load);
        }
    }
    if (resetting >= 3)
    {
        // Stop the reset timer
        resetting = 0;
        if (currentCustomScene)
        {
            auto stage = currentCustomScene->Parent;
            if (stage)
                RaseStageEvent(stage, OnStageLoad, StageLoadPhase_Loaded);
        }
    }
    if (resetting)
        ++resetting;

    if (currentCustomScene && SonicMania::CurrentScene != currentLevelID && !resetting && !InSpecialStage)
    {
        currentLevelID = SonicMania::CurrentScene;
        
        // Unload custom stage
        auto stage = currentCustomScene->Parent;

        // Raise OnStageUnload event
        RaseStageEvent(stage, OnStageUnload, StageLoadPhase_NotLoaded);

        stage->DisableUnlocks();
        currentCustomScene = nullptr;
    }
    bool keyState = GetCtrlKeyState();

    // Reload the stage list
    if (CheckKey('L', keyState, &TrackerL))
    {
        ReloadStageLists();
    }

    // Reload custom stage
    if (CheckKey('R', keyState, &TrackerR))
    {
        // Reload if a custom stage is loaded
        if (currentCustomScene)
        {
            auto stage = currentCustomScene->Parent;
            RaseStageEvent(stage, OnStageUnload, StageLoadPhase_Load);
            RaseStageEvent(stage, OnStageLoad, StageLoadPhase_NotLoaded);
            StartAssetReset();
        }
    }

}

// TODO: Caller needs reworking
void InfinityZone::OnActCompleted()
{
    LogDebug("InfinityZone::OnActCompleted", "Act Completed");
}


void InfinityZone::Init()
{
    LogDebug("InfinityZone::Init", "Starting InfinityZone... Built at %s %s", __DATE__, __TIME__);
}

// Loads and registers the stage information
void InfinityZone::LoadStages(string path, bool registerList)
{
    LogDebug("InfinityZone::LoadStages", "Loading Stage List: %s", path.c_str());
    unsigned int size = 0;

    // Open file
    std::ifstream file(path);

    // Get size and allocate memory
    file.seekg(0, std::ios::end);
    size = static_cast<unsigned int>(file.tellg());
    char* xml = (char*)malloc(size);
    file.seekg(0, std::ios::beg);
    
    // Read file
    file.read(xml, size);
    
    if (xml && size)
    {
        // Record the file path to the stage list
        if (registerList)
            loadedStageLists.push_back(new string(path));
        tinyxml2::XMLDocument document;
        document.Parse(static_cast<const char*>(xml), size);

        auto xmlStages = document.FirstChildElement("Stages");
        if (xmlStages)
        {
            for (auto xmlStage = xmlStages->FirstChildElement(); xmlStage != nullptr; xmlStage = xmlStage->NextSiblingElement())
            {
                auto stage = new IZStage();
                if (stage->LoadXML(xmlStage))
                {
                    if (FindIZStage(stage->StageKey) != nullptr)
                        LogWarn("InfinityZone::LoadStages", "Duplicate stage key of \"%s\" has been detected! Keys must be unique to work correctly.", stage->StageKey.c_str());

                    RegisterIZStage(stage);
                    LogDebug("InfinityZone::LoadStages", "Loaded Stage \"%s\"", stage->StageName.c_str());
                }
            }
        }
        else
        {
            LogError("InfinityZone::LoadStages", "Failed to find the \"Stages\" element in \"%s\". Make sure the file structure is correct!", path.c_str());
        }
    }
    // Clean up
    free(xml);
}

void InfinityZone::ReloadStageLists()
{
    LogDebug("InfinityZone::ReloadStageLists", "Reloading stage lists...");
    // Backup the current stage key
    string currentStageKeyBackup;
    string currentSceneIDBackup;
    if (currentCustomScene)
    {
        currentStageKeyBackup += currentCustomScene->Parent->StageKey;
        currentSceneIDBackup += currentCustomScene->SceneID;
    }

    // Clear soon to be invalid key
    currentCustomScene = nullptr;

    // Remove all registered stages
    for (auto stage : registeredStages)
        delete stage;
    registeredStages.clear();

    // Clear Scene List (IZStage will deallocate the data)
    registeredScenes.clear();

    // Load all stage lists
    for (auto filepath : loadedStageLists)
        LoadStages(*filepath, false);

    // Check if the stage still exists
    if (!currentStageKeyBackup.empty())
    {
        IZStage* stage = FindIZStage(currentStageKeyBackup);
        IZScene* scene = FindIZScene(stage, currentSceneIDBackup);

        if (!scene)
        {
            LogWarn("InfinityZone::ReloadStageLists", "WARNING: Current scene no longer exists! Returning to the title screen...");
            SonicMania::CurrentScene = SonicMania::Scene_Title;
            SonicMania::GameState = SonicMania::GameState_NotRunning;
        }
        else
        {
            currentCustomScene = scene;
        }
    }
}

void InfinityZone::StartAssetReset()
{
    // Set reset phase to 1
    resetting = 1;

    // Can really be any stage that's not Thanks For Playing
    SonicMania::CurrentScene = SonicMania::Scene_GHZ1;
    SonicMania::GameState = SonicMania::GameState_NotRunning;
    LogDebug("InfinityZone::StartAssetReset", "Performing asset reset...");
}

void InfinityZone::ChangeStage(string id, string sceneID)
{
    auto stage = FindIZStage(id);
    auto scene = FindIZScene(stage, sceneID);

    if (currentCustomScene && SonicMania::CurrentScene == SonicMania::Scene_ThanksForPlaying)
    {
        auto oldStage = currentCustomScene->Parent;
        // Raise OnStageUnload event
        RaseStageEvent(oldStage, OnStageUnload, StageLoadPhase_NotLoaded);

        // Disables the old unlock codes
        oldStage->DisableUnlocks();
        
        // Set current modded scene
        currentCustomScene = scene;

        // Raise OnStageLoad event
        RaseStageEvent(stage, OnStageLoad, StageLoadPhase_NotLoaded);

        // Enable new unlocks
        stage->EnableUnlocks();;

        // Reset the stage
        StartAssetReset();
    }
    else
    {
        // Raise OnStageLoad event
        RaseStageEvent(stage, OnStageLoad, StageLoadPhase_Load);
        
        // Enables the unlock codes
        stage->EnableUnlocks();
        
        // Set current modded scene
        currentCustomScene = scene;
        
        // Reset the scene (This needs changing)
        SonicMania::CurrentScene = SonicMania::Scene_ThanksForPlaying;
        currentLevelID = SonicMania::Scene_ThanksForPlaying;
        SonicMania::GameState = SonicMania::GameState_NotRunning;
        
        // Skip Asset Reset
        resetting = 2;
    }
    LogInfo("InfinityZone::ChangeStage", "Loading Stage: \"%s\" with \"Scene%s.bin\"", stage->StageName.c_str(), scene->SceneID.c_str());
}

void InfinityZone::ChangeScene(IZScene* scene)
{
    if (currentCustomScene && SonicMania::CurrentScene == SonicMania::Scene_ThanksForPlaying)
    {
        auto oldStage = currentCustomScene->Parent;
        // Raise OnStageUnload event
        RaseStageEvent(oldStage, OnStageUnload, StageLoadPhase_NotLoaded);

        // Disable old unlocks
        oldStage->DisableUnlocks();

        // Set current modded scene
        currentCustomScene = scene;

        // Raise OnStageLoad event
        RaseStageEvent(scene->Parent, OnStageLoad, StageLoadPhase_NotLoaded);

        // Enable new unlocks
        scene->Parent->EnableUnlocks();;
        StartAssetReset();
    }
    else
    {
        // Raise OnStageLoad event
        RaseStageEvent(scene->Parent, OnStageLoad, StageLoadPhase_Load);

        scene->Parent->EnableUnlocks();
        
        // Set current modded scene
        currentCustomScene = scene;
        
        // Reset the scene (This needs changing)
        SonicMania::CurrentScene = SonicMania::Scene_ThanksForPlaying;
        currentLevelID = SonicMania::Scene_ThanksForPlaying;
        SonicMania::GameState = SonicMania::GameState_NotRunning;
        
        // Skip Asset Reset
        resetting = 2;
    }
    LogInfo("InfinityZone::ChangeStage", "Loading Stage: \"%s\" with \"Scene%s.bin\"", scene->Parent->StageName.c_str(), scene->SceneID.c_str());
}

IZScene* InfinityZone::GetCurrentScene() const
{
    return currentCustomScene;
}


extern "C"
{
    InfinityZone* IZInstance;

    // Workaround
    static intptr_t CheckFile_addr;


    int CheckFile_i_wrapper(char* buf)
    {
        string result = IZInstance->OnFileLoad(string(buf));
        strcpy_s(buf, 128, result.c_str());
        return ((decltype(CheckFile_i_wrapper)*)CheckFile_addr)(buf);
    }

    SonicMania::GameStates ActComplete_hook()
    {
        IZInstance->OnActCompleted();
        return SonicMania::GameState;
    }

    int __cdecl mid_EnterSpecialStage_hook()
    {
        InSpecialStage = true;
        auto orig = (decltype(mid_EnterSpecialStage_hook)*)(baseAddress + 0x949F0);
        return orig();
    }

    int __cdecl mid_ExitSpecialStage_hook()
    {
        InSpecialStage = false;
        auto orig = (decltype(mid_ExitSpecialStage_hook)*)(*(int*)(baseAddress + 0xAA7798));
        return orig();
    }


    IZ_EXPORT void OnFrame()
    {
        IZInstance->OnFrame();
    }

    IZ_EXPORT void PostInit(const char* path)
    {
        // Hook ManiaModLoader.CheckFile_i
        intptr_t addr = GetAddressFromJump(baseAddress + 0x1C540E) + 7;
        CheckFile_addr = GetAddressFromJump(addr);
        WriteCall((void*)addr, CheckFile_i_wrapper);

        // Hook ActComplete
        WriteJump((void*)(baseAddress + 0x001EF0B2), ActComplete_hook);

        // Hook mid_EnterSpecialStage
        WriteCall((void*)(baseAddress + 0x0005B134), mid_EnterSpecialStage_hook);

        // Hook mid_ExitSpecialStage
        WriteJump((void*)(baseAddress + 0x00166F59), mid_ExitSpecialStage_hook);

#ifdef _DEBUG
        const std::string path_cpp = path;
        IZInstance->LoadStages((path_cpp + "\\Stages.xml").c_str(), true);
#endif
    }

    IZ_EXPORT void Init(const char* path)
    {

        IZInstance = new InfinityZone();
        IZInstance->Init();

        // Apply DevMenu patches for IZ
        PatchInfinityZoneDevMenu();
    }

    IZ_EXPORT ModInfo ManiaModInfo = { ModLoaderVer, GameVer };
}