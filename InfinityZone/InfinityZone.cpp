#include "InfinityZone.h"
#include "depends\tinyxml2\tinyxml2.h"

string InfinityZone::OnFileLoad(string path)
{
    // Ignore when resetting
    if (resetting)
        return path;

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
    //if (SonicMania::GameState & SonicMania::GameState_Running && resetting)
    //{
    //  std::cout << "[InfinityZone::OnFrame] Resetting..." << std::endl;
    //  resetting = false;
    //  strcpy_s(SonicMania::CurrentSceneName, 12, currentStageKey->c_str());
    //  
    //  // Reset
    //  SonicMania::GameState = SonicMania::GameState_NotRunning;
    //}

    if (currentStageKey && SonicMania::CurrentScene != currentLevelID)
    {
        currentLevelID = SonicMania::CurrentScene;
        // Unload custom stage
        auto stage = registeredStages[*currentStageKey];
        stage->DisableUnlocks();
        currentStageKey = nullptr;
    }
}

void InfinityZone::Init()
{
    std::cout << "[InfinityZone::Init] Started InfinityZone" << std::endl;
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

void InfinityZone::ChangeStage(string id)
{
    auto stage = registeredStages[id];
    stage->EnableUnlocks();

    // Set current modded stage ID
    currentStageKey = &stage->StageKey;
    // Reset the scene (This needs changing)
    SonicMania::CurrentScene = SonicMania::Scene_ThanksForPlaying;
    currentLevelID = SonicMania::Scene_ThanksForPlaying;
    SonicMania::GameState = SonicMania::GameState_NotRunning;
    
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
            // Test load stage
            IZInstance->LoadStages("Data/Stage.xml");
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