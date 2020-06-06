#pragma once
#include "Common.h"

class IZStage;

class IZScene
{
protected:
public:
    // The owner of the scene (Base)
    IZStage* Parent;
    // The id of this scene (Scene%s.bin)
    string SceneID;
    // The scene flags for this scene
    SonicMania::Filter Flags;
};