#pragma once
#include "Common.h"
#include "Helpers.h"
#include "depends\tinyxml2\tinyxml2.h"
#include "IZStageUnlocks.h"
#include "IZScene.h"
#include "IZTitleCard.h"
#include "IZCustomWater.h"

class IZStage
{
protected:
public:
    // The name of the stage
    string StageName;
    // The internal name of this stage (Must be unique)
    string StageKey;
    // The name of the folder for this stage
    string StageDir;
    // The scene flags for this stage
    SonicMania::Filter Flags = SonicMania::Filter_Common | SonicMania::Filter_Mania;

    // List of unlocks to enable for a given stage
    vector<IZStage_UnlockCode*> Unlocks;
    // List of paths to assets that needs to be replaced. old => new
    unordered_map<string, string> Assets;

    // Custom Titlecard colors
    IZTitleCard TitleCardColors;

    // Custom Water colors
    IZCustomWater CustomWaterSettings;

    // Loads XML stage data from a file
    bool LoadXML(tinyxml2::XMLElement* xmlStage);
    // Write all the needed unlocks
    bool EnableUnlocks();
    // Restore all the unlocks
    bool DisableUnlocks(); 

    ~IZStage();
};

extern IZStage_Unlock* FindUnlock(string name);
// Codes that are currently running
extern vector<IZStage_UnlockCode*> EnabledUnlocks;
