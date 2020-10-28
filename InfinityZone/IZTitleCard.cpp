#include "IZTitleCard.h"
#include "SonicMania.h"
#include "InfinityZone.h"
#include "depends\tinyxml2\tinyxml2.h"
#include <string>
#include <sstream>

// Default Colour Sets
static IZTitleCard::ColorSet DefaultManiaColorSet  { {240, 200, 0},   { {240, 140, 24 }, {96, 192, 160}, {240, 80 , 48 }, {64 , 96 , 170}, {240, 240, 240}} };
static IZTitleCard::ColorSet DefaultEncoreColorSet { {107, 174, 153}, { {55 , 81 , 162}, {199, 82, 91 }, {66 , 143, 195}, {223, 177, 63 }, {240, 240, 240}} };

IZTitleCard::IZTitleCard()
{
    ManiaModeColorSet = DefaultManiaColorSet;
    EncoreModeColorSet = DefaultEncoreColorSet;
}

bool IZTitleCard::LoadXML(tinyxml2::XMLElement* xmlColors)
{
    auto xmlManiaMode = xmlColors->FirstChildElement("ManiaMode");
    if (xmlManiaMode)
        ManiaModeColorSet = LoadColorSetFromXML(xmlManiaMode, DefaultManiaColorSet);
    auto xmlEncoreMode = xmlColors->FirstChildElement("EncoreMode");
    if (xmlEncoreMode)
        EncoreModeColorSet = LoadColorSetFromXML(xmlEncoreMode, DefaultEncoreColorSet);
    return true;
}

IZTitleCard::ColorSet IZTitleCard::LoadColorSetFromXML(tinyxml2::XMLElement* xmlColor, IZTitleCard::ColorSet defaultColorSet)
{
    IZTitleCard::ColorSet set = defaultColorSet;
    auto attr = xmlColor->Attribute("Background");
    if (attr)
        set.BackgroundColor = ColorFromString(attr);
    for (int i = 0; i < 5; ++i)
    {
        attr = xmlColor->Attribute(("Foreground" + std::to_string(i)).c_str());
        if (attr)
            set.ForegroundColors[i] = ColorFromString(attr);
    }
    return set;
}

SonicMania::Color ColorFromString(string value)
{
    unsigned int colorValue;
    std::stringstream sstream;
    sstream << std::hex << value;
    sstream >> colorValue;
    return SonicMania::Color(colorValue);
}

void WriteQuadColour(SonicMania::Color color)
{
    int quads[] =
    {
        (int)(baseAddress + 0x16A75),
        (int)(baseAddress + 0x1654F),
        (int)(baseAddress + 0x16323)
    };

    for (int i = 0; i < sizeof(quads) / sizeof(size_t); ++i)
    {
        WriteData((BYTE*)(quads[i] + 1 + 1 * 5), color.Blue);
        WriteData((BYTE*)(quads[i] + 1 + 2 * 5), color.Green);
        WriteData((BYTE*)(quads[i] + 1 + 3 * 5), color.Red);
    }
}

void UpdateInfinityZoneTitleCard()
{
    // TitleCard Pointer
    auto addr = *(BYTE**)(baseAddress + 0x00AA7634);
    // Is Encore Mode
    bool encore = *(addr + 0x37) == 5;

    auto customSet = encore ? DefaultEncoreColorSet : DefaultManiaColorSet;
    
    auto scene = IZInstance->GetCurrentScene();
    if (scene && scene->Parent)
        customSet = encore ? scene->Parent->TitleCardColors.EncoreModeColorSet : scene->Parent->TitleCardColors.ManiaModeColorSet;
   
    // NOTE: 0x00RRGGBB    
  
    int* colorData = SonicMania::GetAddress(baseAddress + 0x00AA7634, 0, 0x0374) - 4; // - 0x10

    // Write Colours
    for (int i = 0; i < 4; ++i)
        colorData[i] = customSet.ForegroundColors[i].ToRGB888();
    colorData[4] = customSet.BackgroundColor.ToRGB888();
    WriteQuadColour(customSet.ForegroundColors[4]);
}

void PatchInfinityZoneTitleCard()
{
    // Writes our hook into the function that handles what colours to use 
    WriteJump((void*)(baseAddress + 0x00015100), UpdateInfinityZoneTitleCard);
}



