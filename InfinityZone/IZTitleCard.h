#pragma once
#include "SonicMania.h"
#include "depends/tinyxml2/tinyxml2.h"
class IZTitleCard
{
protected:
private:
public:
    SonicMania::Color Encore_BackgroundColor;
    SonicMania::Color Encore_OrangeColor;
    SonicMania::Color Encore_CyanColor;
    SonicMania::Color Encore_RedColor;
    SonicMania::Color Encore_BlueColor;
    SonicMania::Color Encore_WhiteColor;

    SonicMania::Color Mania_BackgroundColor;
    SonicMania::Color Mania_OrangeColor;
    SonicMania::Color Mania_CyanColor;
    SonicMania::Color Mania_RedColor;
    SonicMania::Color Mania_BlueColor;
    SonicMania::Color Mania_WhiteColor;

    bool HasMania;
    bool HasEncore;

    bool LoadXML(tinyxml2::XMLElement* xmlConfig);

    ~IZTitleCard();
};

extern void UpdateInfinityZoneTitleCard();
extern void PatchInfinityZoneTitleCard();
extern SonicMania::Color ColorFromString(const char* value);

