#include "DevMenu.h"
#include "Common.h"
#include "InfinityZone.h"

using namespace SonicMania;

DataPointer(DWORD, dword_6F0AE4, 0x002FBB4C);
DataPointer(BYTE, Key_Enter, 0x00441754);
DataPointer(BYTE, Controller_A, 0x0044170C);
DataPointer(BYTE, Key_Up, 0x004416D8);
DataPointer(BYTE, Key_Down, 0x004416E4);


static int loc_DrawText = baseAddress + 0x1D58C0;
static __declspec(naked) WORD DevMenu_DrawText_(int XPosition, const char* text, int YPosition, DevMenu_Alignment alignment, int color)
{
    __asm
    {
        mov edx, [ESP + 4]
        mov ecx, [ESP + 8]
        push[ESP + 20]
        push[ESP + 20]
        push[ESP + 20]
        call loc_DrawText
        add esp, 12
        ret
    }
}

void PatchInfinityZoneDevMenu()
{
    WriteCall((void*)(baseAddress + 0x001C25DB), InfinityZone_MainDevMenu);
    WriteJump((void*)(baseAddress + 0x001C25E0), (void*)(baseAddress + 0x001C2A25));
    WriteJump((void*)(baseAddress + 0x001C2A31), (void*)(baseAddress + 0x001C2A8C));
}

int InfinityZone_MainDevMenu()
{
    auto con = PlayerControllers[0];
    char result;
    int centerX = *(_DWORD*)(dword_D3CC00 + 614416);
    int centerY = *(_DWORD*)(dword_D3CC00 + 614420);
    int YPosition = centerY - 84;

    int count = 6;
    int optionColors[6];
    for (int i = 0; i < count; ++i)
        optionColors[i] = 0x808090;
    optionColors[DevMenu_Option - DevMenu_Scroll] = 0xF0F0F0;


    // Title
    YPosition += 6;
    YPosition += 14;
    YPosition += 40;

    DevMenu_DrawRect(centerX - 128, YPosition - 8, 256, 84, 0x00000080, 255, 0, 1);
    DevMenu_DrawText_(centerX, "Resume", YPosition, Alignment_Center, optionColors[0]);            YPosition += 12;
    DevMenu_DrawText_(centerX, "Restart", YPosition, Alignment_Center, optionColors[1]);           YPosition += 12;
    DevMenu_DrawText_(centerX, "Stage Select", YPosition, Alignment_Center, optionColors[2]);      YPosition += 12;
    DevMenu_DrawText_(centerX, "Stage Select+", YPosition, Alignment_Center, optionColors[3]);    YPosition += 12;
    DevMenu_DrawText_(centerX, "Options", YPosition, Alignment_Center, optionColors[4]);           YPosition += 12;
    DevMenu_DrawText_(centerX, "Exit", YPosition, Alignment_Center, optionColors[5]);              YPosition += 12;
    YPosition += 10;


    if (Key_Up)
    {
        if (!dword_6F0AE4)
        {
            --DevMenu_Option;
            if (DevMenu_Option < 0)
            {
                DevMenu_Option = 5;
                DevMenu_Scroll = DevMenu_Option - 5;
            }
        }
        result = (dword_6F0AE4 + 1) & 7;
        dword_6F0AE4 = result;
    }
    else if (Key_Down)
    {
        if (!dword_6F0AE4)
        {
            ++DevMenu_Option;
            if ((DevMenu_Option - DevMenu_Scroll) > 6)
                ++DevMenu_Scroll;
            if (DevMenu_Option > 5)
            {
                DevMenu_Option = 0;
                DevMenu_Scroll = 0;
            }
        }
        result = (dword_6F0AE4 + 1) & 7;
        dword_6F0AE4 = result;
    }
    else
    {
        result = Key_Enter | Controller_A;
        dword_6F0AE4 = 0;

        if ((Key_Enter | Controller_A) == 1)
        {
            switch (DevMenu_Option)
            {
            case 0:
                return GameState = *(GameStates*)(baseAddress + 0x1FBB54); // TODO Doesn't work for whatever reason
            case 1:
                GameState = GameState_NotRunning;
                break;
            case 2:
                DevMenu_Address = DevMenu_StageSelect;
                DevMenu_Option = 0;
                DevMenu_Scroll = 0;
                break;
            case 3:
                DevMenu_Address = InfinityZone_StageSelect;
                DevMenu_Option = 0;
                DevMenu_Scroll = 0;
                break;
            case 4:
                DevMenu_Address = DevMenu_Options;
                DevMenu_Option = 0;
                DevMenu_Scroll = 0;
                break;
            case 5:
                *(bool*)(baseAddress + 0x2681D8) = false;
                break;
            default:
                break;
            }
        }
    }
    return result;
}

int InfinityZone_StageSelect()
{
    char result;

    // Centre of the Screen

    int centerX = *(_DWORD*)(dword_D3CC00 + 614416);
    int centerY = *(_DWORD*)(dword_D3CC00 + 614420);
    int YPosition = centerY - 84;

    // Title
    YPosition += 6;
    YPosition += 14;

    YPosition += 40;

    // Bottom Panel
    DevMenu_DrawRect(centerX - 128, YPosition - 8, 256, 84, 0x00000080, 255, 0, 1);



    int optionColours[32];
    for (int i = 0; i < sizeof optionColours / sizeof(int); ++i)
        optionColours[i] = 0x808090;

    // Give selected Option a lighter colour
    if ((DevMenu_Option - DevMenu_Scroll) < 0)
        DevMenu_Scroll = 0;
    optionColours[DevMenu_Option - DevMenu_Scroll] = 0xF0F0F0;
    YPosition -= 10;
//    for (int i = DevMenu_Scroll; i < IZInstance->registeredStages.size(); ++i)
    std::string key = "";
    auto it = IZInstance->registeredStages.begin();
    unsigned int i = DevMenu_Scroll;
    while (it != IZInstance->registeredStages.end())
    {
        YPosition += 12;
        if (i - DevMenu_Scroll > 5)
            break;
        DevMenu_DrawText_(centerX, it->second->StageName.c_str(), YPosition, Alignment_Center, optionColours[i - DevMenu_Scroll]);
        if (DevMenu_Option - DevMenu_Scroll == i) key = it->first;
        ++it;
        ++i;
    }
    if (i == IZInstance->registeredStages.size())
    {
        YPosition += 12;
        DevMenu_DrawText_(centerX, "Back", YPosition, Alignment_Center, optionColours[i - DevMenu_Scroll]);
    }

    if (Key_Up)
    {
        if (!dword_6F0AE4)
        {
            --DevMenu_Option;
            if (DevMenu_Option < 0)
            {
                DevMenu_Option = IZInstance->registeredStages.size();
                DevMenu_Scroll = DevMenu_Option - 5;
                if (DevMenu_Scroll < 0) DevMenu_Scroll = 0;
            }
        }
        result = (dword_6F0AE4 + 1) & 7;
        dword_6F0AE4 = result;
    }
    else if (Key_Down)
    {
        if (!dword_6F0AE4)
        {
            ++DevMenu_Option;
            if ((DevMenu_Option - DevMenu_Scroll) > 5)
                ++DevMenu_Scroll;
            if (DevMenu_Option > static_cast<int>(IZInstance->registeredStages.size()))
                DevMenu_Option = 0;
        }
        result = (dword_6F0AE4 + 1) & 7;
        dword_6F0AE4 = result;
    }
    else
    {
        result = Key_Enter | Controller_A;
        dword_6F0AE4 = 0;
        if ((Key_Enter | Controller_A) == 1)
        {
            if (DevMenu_Option == IZInstance->registeredStages.size())
            {
                DevMenu_Address = InfinityZone_MainDevMenu;
                DevMenu_Option = 3;
                DevMenu_Scroll = 0;
            }
            else
            {
                IZInstance->ChangeStage(key);
            }
        }
    }
    return result;
}