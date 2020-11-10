#pragma once
#include "Common.h"

#define BitFlag(type, enumType) inline type operator|(type a, type b) { return (type)((enumType)a | (enumType)b); }

typedef unsigned char byte;
typedef unsigned short ushort;

namespace SonicMania
{

    enum GameStates : char
    {
        GameState_NotRunning = 0b0000, // 0
        GameState_Running = 0b0001, // 1
        GameState_SoftPause = 0b0010, // 2
        GameState_HardPause = 0b0100, // 4
        GameState_DevMenu = 0b1000  // 8
    };
    BitFlag(GameStates, char)

    enum Scene : short
    {
        Scene_NONE = -1,
        // Presentation 
        Scene_Logo,
        Scene_Title,
        Scene_MainMenu,
        Scene_ThanksForPlaying,
        Scene_LevelSelect,
        Scene_Credits,
        Scene_Continue,
        Scene_Summary,
        Scene_Summary_e,
        // Mania Mode
        Scene_GHZ1,
        Scene_GHZ2,
        Scene_CPZ1,
        Scene_CPZ2,
        Scene_SPZ1,
        Scene_SPZ2,
        Scene_FBZ1,
        Scene_FBZ2,
        Scene_PSZ1,
        Scene_PSZ2,
        Scene_SSZ1,
        Scene_SSZ2,
        Scene_SSZ2M,
        Scene_HCZ1,
        Scene_HCZ2,
        Scene_MSZ1,
        Scene_MSZ1K,
        Scene_MSZ2,
        Scene_OOZ1,
        Scene_OOZ2,
        Scene_LRZ1,
        Scene_LRZ2,
        Scene_LRZ3,
        Scene_MMZ1,
        Scene_MMZ2,
        Scene_TMZ1,
        Scene_TMZ2,
        Scene_TMZ3,
        Scene_ERZ,
        // Encore Mode
        Scene_GHZ1_e,
        Scene_GHZ2_e,
        Scene_CPZ1_e,
        Scene_CPZ2_e,
        Scene_SPZ1_e,
        Scene_SPZ2_e,
        Scene_FBZ1_e,
        Scene_FBZ2_e,
        Scene_PSZ1_e,
        Scene_PSZ2_e,
        Scene_SSZ1_e,
        Scene_SSZ2_e,
        Scene_SSZ2M_e,
        Scene_HCZ1_e,
        Scene_HCZ2_e,
        Scene_MSZ1_e,
        Scene_MSZ2_e,
        Scene_OOZ1_e,
        Scene_OOZ2_e,
        Scene_LRZ1_e,
        Scene_LRZ2_e,
        Scene_LRZ3_e,
        Scene_MMZ1_e,
        Scene_MMZ2_e,
        Scene_TMZ1_e,
        Scene_TMZ2_e,
        Scene_TMZ3_e,
        // Special Stage
        Scene_SpecialStage1,
        Scene_SpecialStage2,
        Scene_SpecialStage3,
        Scene_SpecialStage4,
        Scene_SpecialStage5,
        Scene_SpecialStage6,
        Scene_SpecialStage7,
        Scene_SpecialStage1_e,
        Scene_SpecialStage2_e,
        Scene_SpecialStage3_e,
        Scene_SpecialStage4_e,
        Scene_SpecialStage5_e,
        Scene_SpecialStage6_e,
        Scene_SpecialStage7_e,
        // Blue Sphere (+0 - +31)
        Scene_BlueSphere,
        Scene_BlueSphere_Random = Scene_BlueSphere + 32,
        Scene_BlueSphere_Random2,
        // Pinball
        Scene_Pinball,
        // Extras
        Scene_PuyoPuyo,
        Scene_DAGarden,
        Scene_DAGarden_e,
        // Cutscene
        Scene_Cutscene_AngelIsland,
        Scene_Cutscene_AngelIsland_e,
        Scene_Cutscene_GreenHillZone,
        Scene_Cutscene_GreenHillZone2,
        Scene_Cutscene_MirageSaloonKnucklesIntro,
        Scene_Cutscene_StardustSpeedwayTimeTravel,
        Scene_TryAgain,
        Scene_Cutscene_MirageSaloonEnd_e,
        Scene_TryAgain_e,
        // Video
        Scene_BadEnd_Sonic,
        Scene_BadEnd_Sonic2,
        Scene_BadEnd_Tails,
        Scene_BadEnd_Knux,
        Scene_BadEnd_Mighty,
        Scene_BadEnd_Ray,
        Scene_GoodEnd,
        Scene_TrueEnd
    };

    enum Filter : char
    {
        Filter_None = 0b0000,   // 0
        Filter_Common = 0b0001, // 1
        Filter_Mania = 0b0010,  // 2
        Filter_Encore = 0b0100  // 4
    };
    BitFlag(Filter, char)
    
    DataPointer(Scene, CurrentScene, 0x00A535C4);
    DataPointer(GameStates, GameState, 0x00A535E2);
    DataPointer(Filter, SceneFlags, 0x00A535E3);
    DataPointer(bool, ConsoleEnabled, 0x002FC867);

    extern char* CurrentSceneName;

    enum DevMenu_Alignment : int
    {
        Alignment_Left,
        Alignment_Center,
        Alignment_Right
    };

    struct ControllerInput
    {
        /* 0x00000000 */ BOOL Down;
        /* 0x00000004 */ BOOL Press;
        /* 0x00000008 */ BOOL Release;
    };
    struct Controller
    {
        /* 0x00000000 */ ControllerInput Up;
        /* 0x0000000C */ ControllerInput Down;
        /* 0x00000018 */ ControllerInput Left;
        /* 0x00000024 */ ControllerInput Right;
        /* 0x00000030 */ ControllerInput A;
        /* 0x0000003C */ ControllerInput B;
        /* 0x00000048 */ ControllerInput C;
        /* 0x00000054 */ ControllerInput X;
        /* 0x00000060 */ ControllerInput Y;
        /* 0x0000006C */ ControllerInput Z;
        /* 0x00000078 */ ControllerInput Start;
        /* 0x00000084 */ ControllerInput Select;
    };

    struct Color
    {
        byte Red = 0;
        byte Green = 0;
        byte Blue = 0;

        Color()
        {

        }

        Color(int r, int g, int b)
        {
            Red = r & 0xFF;
            Green = g & 0xFF;
            Blue = b & 0xFF;
        }

        Color(int rgb)
        {
            Red = (rgb >> 16) & 0xFF;
            Green = (rgb >> 8) & 0xFF;
            Blue = rgb & 0xFF;
        }

        inline SHORT ToRGB565()
        {
            //return ((Red & 0b11111000) << 8) | ((Green & 0b11111100) << 3) | (Blue >> 3);
            return (Blue >> 3) | 32 * (Green >> 2) | ((Red >> 3) << 11);
        }

        inline int ToRGB888()
        {
            return (Red & 0xFF) << 16 | (Green & 0xFF) << 8 | Blue & 0xFF;
        }

        inline void FromRGB565(SHORT RGB565)
        {
            Red = (RGB565 & 0b1111100000000000) >> 8;
            Green = (RGB565 & 0b0000011111100000) >> 3;
            Blue = (RGB565 & 0b0000000000011111) << 3;
        }

        inline void FromINT(int rgb)
        {
            Red = (rgb >> 16) & 0xFF;
            Green = (rgb >> 8) & 0xFF;
            Blue = rgb & 0xFF;
        }

        inline void Tint(float tint)
        {
            Red = (byte)(Red * tint);
            Green = (byte)(Green * tint);
            Blue = (byte)(Blue * tint);
        }

        inline void Blend(byte r, byte g, byte b, float a)
        {
            Red = (byte)(a * r + (1 - a) * Red);
            Green = (byte)(a * g + (1 - a) * Green);
            Blue = (byte)(a * b + (1 - a) * Blue);
        }

        inline void Blend(Color color, float a)
        {
            Red = (byte)(a * color.Red + (1 - a) * Red);
            Green = (byte)(a * color.Green + (1 - a) * Green);
            Blue = (byte)(a * color.Blue + (1 - a) * Blue);
        }



    };

    struct Vector2
    {
        Vector2()
        {

        }

        Vector2(int x, int y) : Vector2()
        {
            X = x;
            Y = y;
        }

        Vector2(float x, float y) : Vector2()
        {
            X = (short)x;
            Y = (short)y;
            SubX = (ushort)((x - (ushort)(x)) * 0x10000);
            SubX = (ushort)((y - (ushort)(y)) * 0x10000);
        }

        Vector2(int x) : Vector2()
        {
            X = x;
            Y = x;
        }

        Vector2* Add(Vector2& vec)
        {
            X += vec.X;
            Y += vec.Y;
            return this;
        }

        Vector2* Sub(Vector2& vec)
        {
            X -= vec.X;
            Y -= vec.Y;
            return this;
        }

        Vector2* Mul(Vector2& vec)
        {
            X *= vec.X;
            Y *= vec.Y;
            return this;
        }

        Vector2* Div(Vector2& vec)
        {
            X /= vec.X;
            Y /= vec.Y;
            return this;
        }

        Vector2* Abs()
        {
            if (X < 0)
                X *= -1;
            if (Y < 0)
                Y *= -1;
        }

        Vector2 operator+(const Vector2& vec2)
        {
            Vector2 vec;
            vec.Add(*this);
            vec.Add(*(Vector2*)&vec2);
            return vec;
        }

        Vector2 operator+=(const Vector2& vec2)
        {
            Add(*(Vector2*)&vec2);
            return *this;
        }

        Vector2 operator-(const Vector2& vec2)
        {
            Vector2 vec;
            vec.Add(*this);
            vec.Sub(*(Vector2*)&vec2);
            return vec;
        }

        Vector2 operator-=(const Vector2& vec2)
        {
            Sub(*(Vector2*)&vec2);
            return *this;
        }

        Vector2 operator*(const Vector2& vec2)
        {
            Vector2 vec;
            vec.Add(*this);
            vec.Mul(*(Vector2*)&vec2);
            return vec;
        }

        Vector2 operator*=(const Vector2& vec2)
        {
            Mul(*(Vector2*)&vec2);
            return *this;
        }

        Vector2 operator/(const Vector2& vec2)
        {
            Vector2 vec;
            vec.Add(*this);
            vec.Div(*(Vector2*)&vec2);
            return vec;
        }

        Vector2 operator/=(const Vector2& vec2)
        {
            Div(*(Vector2*)&vec2);
            return *this;
        }

        /** Calculates the Angle to the target vector
            Vector2 target: Location to the target.
            double Return: Angle between the main and target in Radians.
        **/
        double CalculateAngle(Vector2 target)
        {
            double theta = atan2(target.X - X, Y - target.Y);
            if (theta < 0.0)
                theta += 6.2831853071795865;
            return theta;
        }

        /** Calculates the Distance between to the target vector and the main vector
            Vector2 target: Location to the target.
            double Return: The Distance between the main and target.
        **/
        double inline CalculateDistance(Vector2 target)
        {
            return sqrt(pow(X - target.X, 2) + pow(Y - target.Y, 2));
        }

        int GetFullX()
        {
            return (X << 16) + SubX;
        }

        int GetFullY()
        {
            return (Y << 16) + SubY;
        }

        float GetFullX_F()
        {
            return (float)X + (float)(SubX / 0x10000);
        }

        float GetFullY_F()
        {
            return (float)Y + (float)(SubY / 0x10000);
        }

        void SetFullX(int x)
        {
            SubX = (short)x;
            X = x >> 16;
        }

        void SetFullY(int y)
        {
            SubY = (short)y;
            Y = y >> 16;
        }

        void SetFullX(float x)
        {
            X = (short)x;
            SubX = (ushort)((x - (ushort)(x)) * 0x10000);
        }

        void SetFullY(float y)
        {
            Y = (short)y;
            SubY = (ushort)((y - (ushort)(y)) * 0x10000);
        }

        bool inline IsNull()
        {
            return X == 0 && Y == 0;
        }

    public:
        ushort SubX = 0;
        short X = 0;

        ushort SubY = 0;
        short Y = 0;
    };

    enum InkEffect : byte
    {
        Ink_None,
        Ink_Blend,
        Ink_Alpha,
        Ink_Add,
        Ink_Subtract,
        Ink_Distort,
        Ink_Masked,
        Ink_Unmasked
    };

    struct __declspec(align(4)) EntityAnimationData
    {
        /* 0x00000000 */ void* Animationptr;
        /* 0x00000004 */ int CurrentFrame;
        /* 0x00000008 */ short CurrentAnimation;
        /* 0x0000000A */ short LastAnimation;
        /* 0x0000000C */ short Speed;
        /* 0x0000000E */ short Unknown0E;
        /* 0x00000010 */ short Duration;
        /* 0x00000012 */ short FrameCount; // Frame count + 1?
        /* 0x00000014 */ BYTE unknown14;
        /* 0x00000015 */ BYTE UserFrameCount; // I Added This Normally Unknown
    };
    
    FunctionPointer(void, DrawRect, (int Xpos, int Ypos, int Width, int Height, int Colour, signed int Alpha, SonicMania::InkEffect InkEffect, BOOL ScreenRelative), 0x001D8870);
    FunctionPointer(int, DrawSprite, (SonicMania::EntityAnimationData* AnimData, SonicMania::Vector2* Position, BOOL ScreenRelative), 0x001B3B00);

    DataPointer(HWND, MainWindowHandle, 0x00A53C10);
    DataPointer(bool, DevMenu_Enabled, 0x002FC867);
    DataPointer(void*, DevMenu_Address, 0x002FBB40);
    DataPointer(int, DevMenu_Option, 0x002FBB44);
    DataPointer(int, DevMenu_Scroll, 0x002FBB48);
    FunctionPointer(int, DevMenu_DrawRect, (int x, int y, signed int width, int height, int color, signed int a6, int a7, int a8), 0x001D8870);
    FunctionPointer(int, DevMenu_DrawText, (int x, const char* text, int y, DevMenu_Alignment CenterText, int color), 0x1D58C0);
    DataPointer(DWORD, dword_D3CC00, 0x947C6C);
    DataArray(Controller, PlayerControllers, 0x004416D8, 5);
    FunctionPointer(char, DevMenu_MainMenu, (), 0x001C2500);
    FunctionPointer(char, DevMenu_StageSelect, (), 0x001C2B60);
    FunctionPointer(char, DevMenu_SceneSelect, (), 0x001C2DB0);
    FunctionPointer(char, DevMenu_Options, (), 0x001C3090);
    FunctionPointer(int, TrySaveUserFile, (const char* filename, void* buffer, unsigned int bufSize, int(__cdecl* setStatus)(int), unsigned int a5), 0x001BE010);
    FunctionPointer(int, TryLoadUserFile, (const char* filename, void* buffer, unsigned int bufSize, int(__cdecl* setStatus)(int)), 0x001BDFF0);
    FunctionPointer(char, LoadGif, (int a1, char* filepath, int buffer), 0x001CBA90);

    inline int* GetAddress(int baseAddress, int offset1)
    {
        return (int*)((*(int*)baseAddress) + offset1);
    }

    inline int* GetAddress(int baseAddress, int offset1, int offset2)
    {
        return (int*)(*(int*)GetAddress(baseAddress, offset1) + offset2);
    }
}