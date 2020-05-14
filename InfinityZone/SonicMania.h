#pragma once
#include "Common.h"

#define BitFlag(type, enumType) inline type operator|(type a, type b) { return (type)((enumType)a | (enumType)b); }

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
	
	DataPointer(Scene, CurrentScene, 0x00A535C4);
	DataPointer(GameStates, GameState, 0x00A535E2);
    DataPointer(int, SceneFlags, 0x00A535E3);

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


}