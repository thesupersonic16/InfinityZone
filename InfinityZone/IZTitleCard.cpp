#include "IZTitleCard.h"
#include "SonicMania.h"
#include "IZStage.h"
#include "InfinityZone.h"
#include "IZScene.h"
#include "depends\tinyxml2\tinyxml2.h"
#include <string>
#include <sstream>


IZTitleCard::~IZTitleCard()
{
    HasMania = false;
    HasEncore = false;
}

bool IZTitleCard::LoadXML(tinyxml2::XMLElement* xmlColors)
{
    for (auto child = xmlColors->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        std::string Encore = "EncoreMode";
        std::string Mania = "ManiaMode";
        auto entry = child->Name();
        if (!Mania.compare(entry))
        {
            HasMania = true;
            Mania_BackgroundColor = ColorFromString(child->Attribute("Background"));
            Mania_OrangeColor = ColorFromString(child->Attribute("OrangeFG"));
            Mania_CyanColor = ColorFromString(child->Attribute("CyanFG"));
            Mania_RedColor = ColorFromString(child->Attribute("RedFG"));
            Mania_BlueColor = ColorFromString(child->Attribute("BlueFG"));
            Mania_WhiteColor = ColorFromString(child->Attribute("WhiteFG"));
        }
        else if (!Encore.compare(entry))
        {
            HasEncore = true;
            Encore_BackgroundColor = ColorFromString(child->Attribute("Background"));
            Encore_OrangeColor = ColorFromString(child->Attribute("OrangeFG"));
            Encore_CyanColor = ColorFromString(child->Attribute("CyanFG"));
            Encore_RedColor = ColorFromString(child->Attribute("RedFG"));
            Encore_BlueColor = ColorFromString(child->Attribute("BlueFG"));
            Encore_WhiteColor = ColorFromString(child->Attribute("WhiteFG"));
        }
        else
        {
            LogWarn("InfinityZone::IZStage::LoadXML", "Invalid Custom Title Card Color Name! Check Spelling!", child->Name());
            return false;
        }
    }
    return true;
}

SonicMania::Color ColorFromString(const char* value)
{
    unsigned int colorValue;
    std::stringstream sstream;
    sstream << std::hex << value;
    sstream >> colorValue;
    return SonicMania::Color(colorValue);
}

void WriteColour(BYTE*& address, BYTE red, BYTE green, BYTE blue)
{
    // Writes all three colours into the buffer in BGR order
    *(int*)address = red << 16 | green << 8 | blue;
    // Offsets the Address for the next set
    address += 4;
}

void WriteWhiteColours(BYTE Red, BYTE Green, BYTE Blue)
{
    int* ZoneQuad_1 = (int*)(baseAddress + 0x16A75);
    int* ZoneQuad_2 = (int*)(baseAddress + 0x1654F);
    int* ZoneQuad_3 = (int*)(baseAddress + 0x16323);

    BYTE blank = 0;
    BYTE Alpha = 255;

    WriteData((void*)(ZoneQuad_1),
        {
            (byte)0x68, (byte)Alpha, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Blue, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Green, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Red, (byte)blank, (byte)blank, (byte)blank
        }
    );

    WriteData((void*)(ZoneQuad_2),
        {
            (byte)0x68, (byte)Alpha, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Blue, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Green, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Red, (byte)blank, (byte)blank, (byte)blank
        }
    );

    WriteData((void*)(ZoneQuad_3),
        {
            (byte)0x68, (byte)Alpha, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Blue, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Green, (byte)blank, (byte)blank, (byte)blank,
            (byte)0x68, (byte)Red, (byte)blank, (byte)blank, (byte)blank
        }
    );

}

void UpdateInfinityZoneTitleCard()
{
    // TitleCard Pointer
    auto addr = *(BYTE**)(baseAddress + 0x00AA7634);
    // Is in Encore Mode
    bool encore = *(addr + 0x37) == 5;
    // Address to the Background Colour within the Buffer 
    addr = (BYTE*)((*(int*)addr) + 0x0374);

    // Checking For Custom TitleCard Colours
    bool hasCustomMania = false;
    bool hasCustomEncore = false;
    const auto scene = IZInstance->GetCurrentScene();
    if (scene && scene->Parent)
    {
        if (scene->Parent->TitleCardColors.HasMania) hasCustomMania = true;
        if (scene->Parent->TitleCardColors.HasEncore) hasCustomEncore = true;
    }

    // NOTE: 0x00RRGGBB    
    if (hasCustomEncore && encore)
    {
        // Write Custom Encore Mode Background Colour
        *(int*)addr = scene->Parent->TitleCardColors.Encore_BackgroundColor.Red << 16 | scene->Parent->TitleCardColors.Encore_BackgroundColor.Green << 8 | scene->Parent->TitleCardColors.Encore_BackgroundColor.Blue;
    }
    else if (hasCustomMania)
    {
        // Write Custom Mania Mode Background Colour
        *(int*)addr = scene->Parent->TitleCardColors.Mania_BackgroundColor.Red << 16 | scene->Parent->TitleCardColors.Mania_BackgroundColor.Green << 8 | scene->Parent->TitleCardColors.Mania_BackgroundColor.Blue;
    }
    else if (encore) 
    {
        // Write Encore Mode Background Colour
        *(int*)addr = 0x006BAE99;
    }
    else 
    {
        // Write Mania Mode Background Colour
        *(int*)addr = 0x00F0C800;
    }


    // Jump back
    addr -= 0x10;

    if (hasCustomEncore && encore)
    {
        // Write Custom Encore Mode Colours
        auto orange = scene->Parent->TitleCardColors.Encore_OrangeColor;
        auto cyan = scene->Parent->TitleCardColors.Encore_CyanColor;
        auto red = scene->Parent->TitleCardColors.Encore_RedColor;
        auto blue = scene->Parent->TitleCardColors.Encore_BlueColor;
        auto white = scene->Parent->TitleCardColors.Encore_WhiteColor;
        // NOTE: ADDR, R, G, B
        WriteColour(addr, orange.Red, orange.Green, orange.Blue);
        WriteColour(addr, cyan.Red, cyan.Green, cyan.Blue);
        WriteColour(addr, red.Red, red.Green, red.Blue);
        WriteColour(addr, blue.Red, blue.Green, blue.Blue);
        WriteWhiteColours(white.Red, white.Green, white.Blue);
    }
    else if (hasCustomMania)
    {
        // Write Custom Mania Mode Colours
        auto orange = scene->Parent->TitleCardColors.Mania_OrangeColor;
        auto cyan = scene->Parent->TitleCardColors.Mania_CyanColor;
        auto red = scene->Parent->TitleCardColors.Mania_RedColor;
        auto blue = scene->Parent->TitleCardColors.Mania_BlueColor;
        auto white = scene->Parent->TitleCardColors.Mania_WhiteColor;
        // NOTE: ADDR, R, G, B
        WriteColour(addr, orange.Red, orange.Green, orange.Blue);
        WriteColour(addr, cyan.Red, cyan.Green, cyan.Blue);
        WriteColour(addr, red.Red, red.Green, red.Blue);
        WriteColour(addr, blue.Red, blue.Green, blue.Blue);
        WriteWhiteColours(white.Red, white.Green, white.Blue);
    }
    else if (encore)
    {
        // Write Encore Mode Colours
         // NOTE: ADDR, R, G, B
        WriteColour(addr, 55, 81, 162); 
        WriteColour(addr, 199, 82, 91);
        WriteColour(addr, 66, 143, 195);
        WriteColour(addr, 223, 177, 63);
        WriteWhiteColours(240, 240, 240);
    }
    else
    {
        // Write Mania Mode Colours
        // NOTE: ADDR, R, G, B 
        WriteColour(addr, 240, 140, 24);  //Orange
        WriteColour(addr, 96, 192, 160);  //Cyan
        WriteColour(addr, 240, 80, 48);   //Red
        WriteColour(addr, 64, 96, 176);   //Blue
        WriteWhiteColours(240, 240, 240); //White
    }
}

void PatchInfinityZoneTitleCard()
{
    // Writes our hook into the function that handles what colours to use 
    WriteJump((void*)(baseAddress + 0x00015100), UpdateInfinityZoneTitleCard);
}



