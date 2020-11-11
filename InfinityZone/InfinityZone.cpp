#include "InfinityZone.h"
#include "IZAPI.h"
#include "depends\tinyxml2\tinyxml2.h"
#include <fstream>
#include <algorithm>
#include "../IZAPI/IZAPI.h"

extern "C" IZ_EXPORT const HelperFunctions* MML_HelperFunctions = nullptr;

extern const SonicMania::Scene IZ_SceneID = SonicMania::Scene_ThanksForPlaying;

#define RaseStageEvent(scene, handler, phase) \
for (auto& event : handler) \
event({ \
        scene->Parent->StageKey.c_str(), \
        scene->Parent->StageDir.c_str(), \
        scene->Parent->StageName.c_str(), \
        scene->SceneKey.c_str(), \
        scene->SceneID.c_str(), \
        scene->SceneName.c_str(), \
        scene->Flags \
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

IZScene* InfinityZone::FindIZScene(string sceneKey)
{
    for (IZScene* scene : registeredScenes)
    {
        if (scene->SceneKey == sceneKey)
            return scene;
    }
    return nullptr;
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
    ReplaceString(path, oldStageID, scene.Parent->StageDir);

    // Load custom stage assets
    bool found = false;
    for (auto asset : scene.Parent->Assets)
    {
        if (path.rfind(asset.first, 0) == 0)
        {
            string newPath = asset.second + path.substr(asset.first.length());
            if (MML_HelperFunctions->CheckFile(newPath.c_str()))
            {
                path = newPath;
                LogDebug("InfinityZone::OnFileLoad", "Loading Stage Asset: %s", path.c_str());
                found = true;
                break;
            }
        }
    }
    // Load custom scene file
    if (!found)
    {
        auto position = path.find("/Scene");
        if (position != string::npos && path.find(".bin") != string::npos)
        {
            position += 6;
            return path.substr(0, position) + currentCustomScene->SceneID + ".bin";
        }
    }

    for (auto asset : GlobalAssets)
    {
        if (path.rfind(asset.first, 0) == 0)
        {
            string newPath = asset.second + path.substr(asset.first.length());
            if (MML_HelperFunctions->CheckFile(newPath.c_str()))
            {
                path = newPath;
                LogDebug("InfinityZone::OnFileLoad", "Loading Global Asset: %s", path.c_str());
                found = true;
                break;
            }
        }
    }
    return path;
}

void InfinityZone::OnFrame()
{
    // Asset Reset
    if (SonicMania::GameState & SonicMania::GameState_Running && resetting == 2)
    {
        LogDebug("InfinityZone::OnFrame", "Finalising asset reset...");

        SonicMania::CurrentScene = IZ_SceneID;
        SonicMania::GameState = SonicMania::GameState_NotRunning;
        if (currentCustomScene)
        {
            auto scene = currentCustomScene;
            if (scene && scene->Parent)
                RaseStageEvent(scene, OnStageLoad, StageLoadPhase_Load);
        }
    }
    if (resetting >= 3)
    {
        // Stop the reset timer
        resetting = 0;
        if (currentCustomScene)
        {
            auto scene = currentCustomScene;
            if (scene && scene->Parent)
                RaseStageEvent(scene, OnStageLoad, StageLoadPhase_Loaded);
        }
    }
    if (resetting)
        ++resetting;

    if (currentCustomScene && SonicMania::CurrentScene != currentLevelID && !resetting && !InSpecialStage)
    {
        currentLevelID = SonicMania::CurrentScene;
        
        // Unload custom stage
        auto scene = currentCustomScene;

        // Raise OnStageUnload event
        RaseStageEvent(scene, OnStageUnload, StageLoadPhase_NotLoaded);

        scene->Parent->DisableUnlocks();
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
            auto scene = currentCustomScene;
            RaseStageEvent(scene, OnStageUnload, StageLoadPhase_Load);
            RaseStageEvent(scene, OnStageLoad, StageLoadPhase_NotLoaded);
            StartAssetReset();
        }
    }

}

// TODO: Caller needs reworking
void InfinityZone::OnActCompleted()
{
    LogDebug("InfinityZone::OnActCompleted", "Act Completed");
}


void InfinityZone::Init(string path)
{
    LogDebug("InfinityZone::Init", "Starting InfinityZone... Built at %s %s", __DATE__, __TIME__);
    LoadUnlockSets((path + "\\Unlocks.xml").c_str());
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

        auto root = document.FirstChildElement("InfinityZone");

        auto xmlStages = root->FirstChildElement("Stages");
        if (xmlStages)
        {
            for (auto xmlStage = xmlStages->FirstChildElement(); xmlStage != nullptr; xmlStage = xmlStage->NextSiblingElement())
            {
                auto stage = new IZStage();
                if (stage->LoadXML(xmlStage))
                {
                    if (FindIZStage(stage->StageKey) != nullptr)
                        LogWarn("InfinityZone::LoadStages", "Duplicate stage key of \"%s\" has been detected! Keys must be unique to work correctly.", stage->StageKey.c_str());

                    registeredStages.push_back(stage);
                    LogDebug("InfinityZone::LoadStages", "Loaded Stage \"%s\"", stage->StageName.c_str());
                }
            }
        }
        else
        {
            LogError("InfinityZone::LoadStages", "Failed to find the \"Stages\" element in \"%s\". Make sure the file structure is correct!", path.c_str());
        }
        auto xmlCategories = root->FirstChildElement("Categories");
        if (xmlCategories)
        {
            for (auto xmlCategory = xmlCategories->FirstChildElement(); xmlCategory != nullptr; xmlCategory = xmlCategory->NextSiblingElement())
            {
                auto category = new IZCategory();
                registeredCategories.push_back(category);
                category->CategoryName = xmlCategory->Attribute("categoryName");
                for (auto xmlGroup = xmlCategory->FirstChildElement(); xmlGroup != nullptr; xmlGroup = xmlGroup->NextSiblingElement())
                {
                    auto group = new IZCategoryGroup();
                    category->Groups.push_back(group);
                    group->GroupName = xmlGroup->Attribute("groupName");
                    for (auto xmlScene = xmlGroup->FirstChildElement(); xmlScene != nullptr; xmlScene = xmlScene->NextSiblingElement())
                    {
                        auto scene = new IZScene();
                        if (scene->LoadXML(xmlScene, registeredStages))
                        {
                            group->Scenes.push_back(scene);
                            registeredScenes.push_back(scene);
                        }
                        else
                            delete scene;
                    }
                }
            }
        }
        else
        {
            LogError("InfinityZone::LoadStages", "Failed to find the \"Categories\" element in \"%s\". Make sure the file structure is correct!", path.c_str());
        }

    }
    // Clean up
    free(xml);
}

void InfinityZone::LoadUnlockSets(string path)
{
    LogDebug("InfinityZone::LoadUnlockSets", "Loading Unlock Set List: %s", path.c_str());
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
        tinyxml2::XMLDocument document;
        document.Parse(static_cast<const char*>(xml), size);

        auto xmlSets = document.FirstChildElement("UnlockSets");
        if (xmlSets)
        {
            for (auto xmlSet = xmlSets->FirstChildElement(); xmlSet != nullptr; xmlSet = xmlSet->NextSiblingElement())
            {
                // Build unlock set
                IZStage_Unlock set;
                set.UnlockName = string(xmlSet->Name());
                for (auto child = xmlSet->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
                {
                    auto unlock = FindUnlock(child->Name());
                    if (unlock)
                        for (auto& code : unlock->UnlockCodes)
                            set.UnlockCodes.push_back(code);
                    else
                        LogWarn("InfinityZone::LoadUnlockSets", "The unlock code \"%s\" was not found! Was building \"%s\".", child->Name(), set.UnlockName.c_str());
                }
                AllUnlocks.push_back(set);
                LogDebug("InfinityZone::LoadUnlockSets", "Created Set \"%s\"", set.UnlockName.c_str());
            }
        }
        else
        {
            LogError("InfinityZone::LoadUnlockSets", "Failed to find the \"UnlockSets\" element in \"%s\". Make sure the file structure is correct!", path.c_str());
        }
    }
    // Clean up
    free(xml);
}

void InfinityZone::ReloadStageLists()
{
    LogDebug("InfinityZone::ReloadStageLists", "Reloading stage lists...");
    // Backup the current stage key
    string currentSceneKeyBackup;
    if (currentCustomScene)
        currentSceneKeyBackup += currentCustomScene->SceneID;

    // Clear soon to be invalid key
    currentCustomScene = nullptr;

    // Remove all registered stages
    for (auto stage : registeredStages)
        delete stage;
    registeredStages.clear();

    // Clear Scene List (IZStage will deallocate the data)
    // Remove all registered stages
    for (auto scene : registeredScenes)
        delete scene;
    registeredScenes.clear();

    // Clear Category List
    registeredCategories.clear();

    // Load all stage lists
    for (auto filepath : loadedStageLists)
        LoadStages(*filepath, false);

    // Check if the stage still exists
    if (!currentSceneKeyBackup.empty())
    {
        IZScene* scene = FindIZScene(currentSceneKeyBackup);

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

void InfinityZone::ChangeScene(IZScene* scene)
{
    if (currentCustomScene && SonicMania::CurrentScene == IZ_SceneID)
    {
        auto oldScene = currentCustomScene;
        // Raise OnStageUnload event
        RaseStageEvent(oldScene, OnStageUnload, StageLoadPhase_NotLoaded);

        // Disable old unlocks
        oldScene->Parent->DisableUnlocks();

        // Set current modded scene
        currentCustomScene = scene;

        // Raise OnStageLoad event
        RaseStageEvent(scene, OnStageLoad, StageLoadPhase_NotLoaded);

        // Enable new unlocks
        scene->Parent->EnableUnlocks();;
        StartAssetReset();
    }
    else
    {
        // Raise OnStageLoad event
        RaseStageEvent(scene, OnStageLoad, StageLoadPhase_Load);

        scene->Parent->EnableUnlocks();
        
        // Set current modded scene
        currentCustomScene = scene;
        
        // Reset the scene (This needs changing)
        SonicMania::CurrentScene = IZ_SceneID;
        currentLevelID = IZ_SceneID;
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

    void ShowError(const char* error)
    {
        char message[255];
        sprintf_s(message, "%s\n\nIZ Version: %d", error,  IZ_VERSION);
        MessageBoxA(NULL, message, "Fatal InfinityZone Error", MB_OK | MB_ICONERROR);
    }

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
        if (IZInstance)
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
        }
    }

    IZ_EXPORT void Init(const char* path)
    {
        if (!MML_HelperFunctions || MML_HelperFunctions->Version < 1)
        {
            ShowError("ManiaModLoader is too old for InfinityZone to operate correctly\nPlease update ManiaModLoader!");
            return;
        }

        IZInstance = new InfinityZone();
        IZInstance->Init(path);

        // Apply DevMenu patches for IZ
        PatchInfinityZoneDevMenu();

        // Apply TitleCard patches for IZ
        PatchInfinityZoneTitleCard();

        // Apply Water patches for IZ
        PatchInfinityZoneCustomWater();
    }

    IZ_EXPORT ModInfo ManiaModInfo = { ModLoaderVer, GameVer };
}