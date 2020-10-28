#pragma once
#include "SonicMania.h"
#include "depends/tinyxml2/tinyxml2.h"
class IZTitleCard
{
protected:
public:
    struct ColorSet
    {
        SonicMania::Color BackgroundColor;
        SonicMania::Color ForegroundColors[5];
    };
    
    ColorSet ManiaModeColorSet;
    ColorSet EncoreModeColorSet;

    bool LoadXML(tinyxml2::XMLElement* xmlColors);
    ColorSet LoadColorSetFromXML(tinyxml2::XMLElement* xmlColor, IZTitleCard::ColorSet defaultColorSet);

};

extern IZTitleCard::ColorSet DefaultManiaColorSet;
extern IZTitleCard::ColorSet DefaultEncoreColorSet;

extern void UpdateInfinityZoneTitleCard();
extern void PatchInfinityZoneTitleCard();
extern SonicMania::Color ColorFromString(string value);

