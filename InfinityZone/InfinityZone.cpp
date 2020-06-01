#include "InfinityZone.h"
#include "IZAPI.h"
#include "depends\tinyxml2\tinyxml2.h"
#include <fstream>
#include <algorithm>

static bool TrackerL, TrackerR;

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

void InfinityZone::SetIZStage(IZStage* stage)
{
    // Check if stage exists
    auto izStage = FindIZStage(stage->StageKey);
    if (izStage)
        registeredStages.erase(std::remove(registeredStages.begin(), registeredStages.end(), izStage));
    registeredStages.push_back(stage);
}

string InfinityZone::OnFileLoad(string path)
{
    // Load Null Space Zone when performing asset resetting
    if (resetting)
    {
        strcpy_s(SonicMania::CurrentSceneName, 12, "IZ_NSZ");
        return path;
    }

    auto oldStageID = string(SonicMania::CurrentSceneName);

    // Check if a custom stage is loaded
    if (!currentStageKey)
        return path;

    // The loaded custom stage
    auto stage = FindIZStage(*currentStageKey);

    // Set Custom Stage Scene Flags
    SonicMania::SceneFlags = stage->Flags;

    // Replace the stage ID with the custom stage ID from the file path
    ReplaceString(path, oldStageID, stage->StageID);

    // Load custom stage assets
    const auto iter = stage->Assets.find(path);
    if (iter != stage->Assets.cend())
    {
        path = iter->second;
        std::cout << "[InfinityZone::OnFileLoad] Loading Asset: " << path << std::endl;
    }
    return path;
}

void InfinityZone::OnFrame()
{
    // Asset Reset
    if (SonicMania::GameState & SonicMania::GameState_Running && resetting >= 2)
    {
      std::cout << "[InfinityZone::OnFrame] Finalising asset reset..." << std::endl;
      // Stop the reset timer
      resetting = 0;

      SonicMania::CurrentScene = SonicMania::Scene_ThanksForPlaying;
      SonicMania::GameState = SonicMania::GameState_NotRunning;
    }

    if (currentStageKey && SonicMania::CurrentScene != currentLevelID && !resetting)
    {
        currentLevelID = SonicMania::CurrentScene;
        
        // Unload custom stage
        auto stage = FindIZStage(*currentStageKey);

        // Raise OnStageUnload event
        for (auto& event : OnStageUnload)
            event({
                    stage->StageKey.c_str(),
                    stage->StageID.c_str(),
                    stage->StageName.c_str()
                });

        stage->DisableUnlocks();
        currentStageKey = nullptr;
    }
    bool keyState = GetCtrlKeyState();

    if (CheckKey('L', keyState, &TrackerL))
    {
        ReloadStageLists();
    }

    if (CheckKey('R', keyState, &TrackerR))
    {
        // Check if a custom stage is loaded
        if (currentStageKey)
            StartAssetReset();
    }

    if (resetting)
        ++resetting;
}

void InfinityZone::OnActCompleted()
{
    std::cout << "[InfinityZone::OnActCompleted] Act Completed" << std::endl;

}


void InfinityZone::Init()
{
    std::cout << "[InfinityZone::Init] Starting InfinityZone..." << std::endl;
}

// Loads and registers the stage information
void InfinityZone::LoadStages(string path, bool registerList)
{
    unsigned int size = 0;
    //void* xml = LoadAndReadFile(path.c_str(), &size);

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
                        std::cerr << "[InfinityZone::LoadStages] Duplicate stage key of \"" << stage->StageKey << "\" has been detected! Keys need to unique to work" << std::endl;

                    SetIZStage(stage);
                    std::cout << "[InfinityZone::LoadStages] Registered \"" << stage->StageName << "\"" << std::endl;
                }
            }
        }
        else
        {
            std::cerr << R"([InfinityZone::LoadStages] Failed to find "Stages" element in ")" << path << R"(". Make sure the structure is correct!)" << std::endl;
        }
    }
    free(xml);
}

void InfinityZone::ReloadStageLists()
{
    std::cerr << "[InfinityZone::ReloadStageLists] Reloading stage lists..." << std::endl;
    // Backup the current stage key
    string currentStageKeyBackup = "";
    if (currentStageKey)
        currentStageKeyBackup += *currentStageKey;

    // Clear soon to be invalid key
    currentStageKey = nullptr;

    // Remove all registered stages
    for (auto stage : registeredStages)
        delete stage;
    registeredStages.clear();

    // Load all stage lists
    for (auto filepath : loadedStageLists)
        LoadStages(*filepath, false);

    // Check if the stage still exists
    if (!currentStageKeyBackup.empty())
    {
        IZStage* stage = FindIZStage(currentStageKeyBackup);

        if (!stage)
        {
            std::cerr << "[InfinityZone::ReloadStageLists] WARNING: Current stage no longer exists! Returning to the title screen..." << std::endl;
            SonicMania::CurrentScene = SonicMania::Scene_Title;
            SonicMania::GameState = SonicMania::GameState_NotRunning;
        }
        else
        {
            currentStageKey = &stage->StageKey;
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
    std::cout << "[InfinityZone::StartAssetReset] Performing asset reset..." << std::endl;
}

void InfinityZone::ChangeStage(string id)
{
    auto stage = FindIZStage(id);

    if (currentStageKey && SonicMania::CurrentScene == SonicMania::Scene_ThanksForPlaying)
    {
        // Raise OnStageUnload event
        for (auto& event : OnStageUnload)
            event({
                    stage->StageKey.c_str(),
                    stage->StageID.c_str(),
                    stage->StageName.c_str()
                });

        // Disable old unlocks
        FindIZStage(*currentStageKey)->DisableUnlocks();
        
        // Set current modded stage ID
        currentStageKey = &stage->StageKey;

        // Raise OnStageLoad event
        for (auto& event : OnStageLoad)
            event({
                    stage->StageKey.c_str(),
                    stage->StageID.c_str(),
                    stage->StageName.c_str()
                });

        // Enable new unlocks
        FindIZStage(*currentStageKey)->EnableUnlocks();;
        StartAssetReset();
    }
    else
    {
        // Raise OnStageLoad event
        for (auto& event : OnStageLoad)
            event({
                    stage->StageKey.c_str(),
                    stage->StageID.c_str(),
                    stage->StageName.c_str()
                });

        stage->EnableUnlocks();
        // Set current modded stage ID
        currentStageKey = &stage->StageKey;
        // Reset the scene (This needs changing)
        SonicMania::CurrentScene = SonicMania::Scene_ThanksForPlaying;
        currentLevelID = SonicMania::Scene_ThanksForPlaying;
        SonicMania::GameState = SonicMania::GameState_NotRunning;
    }
    std::cout << "[InfinityZone::ChangeStage] Loading Stage: \"" << FindIZStage(*currentStageKey)->StageName << "\"" << std::endl;
}

IZStage* InfinityZone::GetCurrentStage()
{
    return FindIZStage(*currentStageKey);
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


    IZ_EXPORT void OnFrame()
    {
        IZInstance->OnFrame();
    }

    IZ_EXPORT void PostInit(const char* path)
    {
        printf("test\n");
        // Hook ManiaModLoader.CheckFile_i
        intptr_t addr = GetAddressFromJump(baseAddress + 0x1C540E) + 7;
        CheckFile_addr = GetAddressFromJump(addr);
        WriteCall((void*)addr, CheckFile_i_wrapper);

        // Hook ActComplete
        WriteJump((void*)(baseAddress + 0x001EF0B2), ActComplete_hook);

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