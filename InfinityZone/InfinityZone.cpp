#include "InfinityZone.h"

string InfinityZone::OnFileLoad(string path)
{
	// Ignore when resetting
	if (resetting)
		return path;

	auto oldStageID = string(SonicMania::CurrentSceneName);

    // Check if a custom stage is loaded
	if (!currentStageID)
		return path;

    // Set Custom Stage Scene Flags
    SonicMania::SceneFlags = registeredStages[*currentStageID]->Flags;

	// Replace the stage ID with the custom stage ID from the file path
	ReplaceString(path, oldStageID, *currentStageID);

	// Load custom stage assets
    const auto iter = registeredStages[*currentStageID]->Assets.find(path);
	if (iter != registeredStages[*currentStageID]->Assets.cend())
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
	//	std::cout << "[InfinityZone::OnFrame] Resetting..." << std::endl;
	//	resetting = false;
	//	strcpy_s(SonicMania::CurrentSceneName, 12, currentStageID->c_str());
	//	
	//	// Reset
	//	SonicMania::GameState = SonicMania::GameState_NotRunning;
	//}

	if (currentStageID && (short)SonicMania::CurrentScene != currentLevelID)
	{
        currentLevelID = SonicMania::CurrentScene;
	    // Unload custom stage
		auto stage = registeredStages[*currentStageID];
		stage->DisableUnlocks();
		currentStageID = nullptr;
	}
}

void InfinityZone::Init()
{
	std::cout << "[InfinityZone::Init] Started InfinityZone" << std::endl;
}

// Loads and registers the stage information
void InfinityZone::LoadStage(string path)
{
    auto stage = new IZStage();
	stage->LoadXML(path);
	registeredStages[stage->StageID] = stage;
	std::cout << "[InfinityZone::LoadStage] Registered \"" << stage->StageName << "\"" << std::endl;
}

void InfinityZone::ChangeStage(string id)
{
	auto stage = registeredStages[id];
	stage->EnableUnlocks();

	// Set current modded stage ID
    currentStageID = &stage->StageID;
	// Reset the scene (This needs changing)
	SonicMania::CurrentScene = SonicMania::Scene_ThanksForPlaying;
    currentLevelID = SonicMania::Scene_ThanksForPlaying;
	SonicMania::GameState = SonicMania::GameState_NotRunning;
	
	std::cout << "[InfinityZone::ChangeStage] Loading Stage: \"" << registeredStages[*currentStageID]->StageName << "\"" << std::endl;
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
			IZInstance->LoadStage("Data/Stage.xml");
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
		PatchInfinityZone();
	}

	IZ_EXPORT ModInfo ManiaModInfo = { ModLoaderVer, GameVer };
}