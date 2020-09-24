#pragma once
#include "Common.h"
#include "depends\tinyxml2\tinyxml2.h"

class IZStage;

class IZScene
{
protected:
public:
    // The owner of the scene (Base)
    IZStage* Parent;
    // The key for this scene, This is the reference name
    string SceneKey;
    // The id of this scene (Scene%s.bin)
    string SceneID;
    // The name of this scene. Name should be short
    string SceneName;
    // The scene flags for this scene
    SonicMania::Filter Flags;
    // Loads XML stage data from a file
    bool LoadXML(tinyxml2::XMLElement* xmlScene, vector<IZStage*> stages);
};