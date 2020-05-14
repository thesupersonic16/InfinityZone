#pragma once
#include "Common.h"
#include "Helpers.h"
#include "depends\tinyxml2\tinyxml2.h"

// 
struct IZStage_UnlockCode
{
	intptr_t adress;
	size_t size;
    char restore[8];
};

struct IZStage_Unlock
{
	string UnlockName;
	std::vector<IZStage_UnlockCode> UnlockCodes;
};

class IZStage
{
protected:
public:
	// The name of the stage
	string StageName;
	// The name of the folder for this stage
	string StageID;
    // The scene flags for this stage
    SonicMania::Filter Flags;

	// List of unlocks to enable for a given stage
	vector<IZStage_UnlockCode*> Unlocks;
	// List of paths to assets that needs to be replaced. old => new
	unordered_map<string, string> Assets;

	// Loads XML stage data form a file
	bool LoadXML(tinyxml2::XMLElement* xmlStage);
	// Write all the needed unlocks
	bool EnableUnlocks();
    // Restore all the unlocks
    bool DisableUnlocks();

};

extern IZStage_Unlock* FindUnlock(string name);
// Codes that are currently running
extern vector<IZStage_UnlockCode*> EnabledUnlocks;
extern vector<IZStage_Unlock> AllUnlocks;
