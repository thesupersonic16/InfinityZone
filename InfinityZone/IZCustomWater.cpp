#include "IZCustomWater.h"
#include "InfinityZone.h"
#include "depends\tinyxml2\tinyxml2.h"
#include <string>
#include <sstream>

#pragma region Helpers

INT GetPointer(int SpritePointer, int offset) //CnG Uses this everywhere, tired of copying it into every header. 
{
    int* pointer = (int*)(baseAddress + SpritePointer);
    if (!*pointer)
        return 0;
    pointer = (int*)(*pointer + offset);
    if (!*pointer)
        return 0;
    return *pointer;
}

SonicMania::InkEffect GetInkEffect(int value)
{
    switch (value)
    {
    case 0:
        return SonicMania::InkEffect::Ink_None;
    case 1:
        return SonicMania::InkEffect::Ink_Blend;
    case 2:
        return SonicMania::InkEffect::Ink_Alpha;
    case 3:
        return SonicMania::InkEffect::Ink_Add;
    case 4:
        return SonicMania::InkEffect::Ink_Subtract;
    case 5:
        return SonicMania::InkEffect::Ink_Distort;
    case 6:
        return SonicMania::InkEffect::Ink_Masked;
    case 7:
        return SonicMania::InkEffect::Ink_Unmasked;
    default:
        return SonicMania::InkEffect::Ink_None;
    }
}

#pragma endregion

static int OnWaterDrawReturn = baseAddress + 0x19B9D;

//Default Water Colors
static IZCustomWater::WaterColorDefintion DefaultWater = IZCustomWater::WaterColorDefintion(SonicMania::Color(0, 0, 0), 0, SonicMania::InkEffect::Ink_Alpha);

IZCustomWater::IZCustomWater()
{
    WaterSettings = DefaultWater;
}

bool IZCustomWater::LoadXML(tinyxml2::XMLElement* xmlWater)
{
    auto colorAttr = xmlWater->Attribute("color");
    if (colorAttr)
        WaterSettings.Color = ColorFromString(colorAttr);

    WaterSettings.Alpha = xmlWater->IntAttribute("alpha", 0);
    WaterSettings.InkEffect = GetInkEffect(xmlWater->IntAttribute("inkEffect", 0));

    return true;
}

IZCustomWater::WaterColorDefintion GetCurrentWaterColorDraw() 
{
    IZCustomWater::WaterColorDefintion customSet = DefaultWater;

    auto scene = IZInstance->GetCurrentScene();
    if (scene && scene->Parent)
        customSet = scene->Parent->CustomWaterSettings.WaterSettings;

    return customSet;
}

int DrawWater(SonicMania::EntityAnimationData* AnimData, SonicMania::Vector2* Position, BOOL ScreenRelative)
{
    ushort pointer = GetPointer(0xAA7634, 0x14);
    int screen = 0;

    if (pointer == 0) screen = 0;
    else if (pointer == 1) screen = 1;
    else if (pointer == 2) screen = 2;
    else if (pointer == 3) screen = 3;

    int offset = 0x96030 * screen;

    int x = 0 - GetPointer(0xAA7628, 0x96000 + offset);
    int y = Position->Y - GetPointer(0xAA7628, 0x96004 + offset);

    auto color1 = GetCurrentWaterColorDraw();
    SonicMania::DrawRect(x, y, 32000, 32000, color1.Color.ToRGB888(), color1.Alpha, color1.InkEffect, true);
    int ReturnVal = SonicMania::DrawSprite(AnimData, Position, ScreenRelative);
    return ReturnVal;
}

static __declspec(naked) void OnWaterDrawHook()
{
    __asm
    {
        call DrawWater
        jmp OnWaterDrawReturn
    }
}

void PatchInfinityZoneCustomWater()
{
    WriteData<6>((BYTE*)(baseAddress + 0x19B97), (BYTE)0x90);
    WriteJump((void*)(baseAddress + 0x19B97), OnWaterDrawHook);
}



