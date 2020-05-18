#include "InfinityZone.h"
#include "depends\tinyxml2\tinyxml2.h"

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
    auto stage = registeredStages[*currentStageKey];

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
        auto stage = registeredStages[*currentStageKey];
        stage->DisableUnlocks();
        currentStageKey = nullptr;
    }

    if (resetting)
        ++resetting;
}

void InfinityZone::Init()
{
    std::cout << "[InfinityZone::Init] Starting InfinityZone..." << std::endl;
    // Test load stage
    IZInstance->LoadStages("Data/Stage.xml");
}

// Loads and registers the stage information
void InfinityZone::LoadStages(string path)
{
    unordered_map<string, IZStage*> stages;
    int size = 0;
    void* xml = LoadAndReadFile(path.c_str(), &size);
    if (xml && size)
    {
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
                    if (registeredStages[stage->StageKey] != nullptr)
                        std::cerr << "[InfinityZone::LoadStages] Duplicate stage key of \"" << stage->StageKey << "\" has been detected! Keys need to unique to work" << std::endl;

                    registeredStages[stage->StageKey] = stage;
                    std::cout << "[InfinityZone::LoadStages] Registered \"" << stage->StageName << "\"" << std::endl;
                }
            }
        }
        else
        {
            std::cerr << R"([InfinityZone::LoadStages] Failed to find "Stages" element in ")" << path << R"(". Make sure the structure is correct!)" << std::endl;
        }
    }
    delete xml;
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
    auto stage = registeredStages[id];

    if (currentStageKey && SonicMania::CurrentScene == SonicMania::Scene_ThanksForPlaying)
    {
        // Disable old unlocks
        registeredStages[*currentStageKey]->DisableUnlocks();
        
        // Set current modded stage ID
        currentStageKey = &stage->StageKey;
        
        // Enable new unlocks
        registeredStages[*currentStageKey]->EnableUnlocks();;
        StartAssetReset();
    }
    else
    {
        stage->EnableUnlocks();
        // Set current modded stage ID
        currentStageKey = &stage->StageKey;
        // Reset the scene (This needs changing)
        SonicMania::CurrentScene = SonicMania::Scene_ThanksForPlaying;
        currentLevelID = SonicMania::Scene_ThanksForPlaying;
        SonicMania::GameState = SonicMania::GameState_NotRunning;
    }
    std::cout << "[InfinityZone::ChangeStage] Loading Stage: \"" << registeredStages[*currentStageKey]->StageName << "\"" << std::endl;
}


extern "C"
{
    InfinityZone* IZInstance;

    // Workaround
    static bool FirstStart = false;
    static string Path;
    static intptr_t CheckFile_addr;


    int CheckFile_i_wrapper(char* buf)
    {
        string result = IZInstance->OnFileLoad(string(buf));
        strcpy_s(buf, 128, result.c_str());
        return ((decltype(CheckFile_i_wrapper)*)CheckFile_addr)(buf);
    }

    IZ_EXPORT void OnFrame()
    {
        if (!FirstStart)
        {
            IZInstance = new InfinityZone();
            IZInstance->Init();
            FirstStart = true;

            // Hook ManiaModLoader.CheckFile_i
            intptr_t addr = GetAddressFromJump(baseAddress + 0x1C540E) + 7;
            CheckFile_addr = GetAddressFromJump(addr);
            WriteCall((void*)addr, CheckFile_i_wrapper);

        }
        IZInstance->OnFrame();
    }

    IZ_EXPORT void Init(const char* path)
    {
        // Apply DevMenu patches for IZ
        PatchInfinityZoneDevMenu();
    }

    IZ_EXPORT ModInfo ManiaModInfo = { ModLoaderVer, GameVer };
}