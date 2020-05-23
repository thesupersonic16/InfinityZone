#include "IZAPI.h"
#include "windows.h"

extern "C"
{
    namespace IZAPI
    {
        HMODULE InfinityZoneModule;

        // Function Types
        typedef void(__cdecl* RegisterStageLoadEvent_type)(StageLoadEvent);
        typedef void(__cdecl* CStringFunc)(const char*);
        typedef void(__cdecl* Func)();
        typedef StageInfo(__cdecl* GetCurrentStage_type)();

        RegisterStageLoadEvent_type RegisterStageLoadEvent_ptr = nullptr;
        RegisterStageLoadEvent_type RegisterStageUnloadEvent_ptr = nullptr;
        CStringFunc LoadStagesFile_ptr = nullptr;
        CStringFunc ChangeStage_ptr = nullptr;
        Func PerformAssetReset_ptr = nullptr;
        GetCurrentStage_type GetCurrentStage_ptr = nullptr;

        void ShowError(const char* error)
        {
            MessageBoxA(NULL, error, "Fatal InfinityZone Error", MB_OK | MB_ICONERROR);
        }

        void IZInit()
        {
            InfinityZoneModule = LoadLibraryA("InfinityZone.dll");
            if (!InfinityZoneModule)
                return ShowError("Could not detect InfinityZone!\nPlease make sure InfinityZone is installed and enabled.");
            RegisterStageLoadEvent_ptr   = (RegisterStageLoadEvent_type)GetProcAddress(InfinityZoneModule, "RegisterStageLoadEvent");
            RegisterStageUnloadEvent_ptr = (RegisterStageLoadEvent_type)GetProcAddress(InfinityZoneModule, "RegisterStageUnloadEvent");
            LoadStagesFile_ptr           = (CStringFunc)GetProcAddress(InfinityZoneModule, "LoadStagesFile");
            ChangeStage_ptr              = (CStringFunc)GetProcAddress(InfinityZoneModule, "ChangeStage");
            PerformAssetReset_ptr        = (Func)GetProcAddress(InfinityZoneModule, "PerformAssetReset");
            GetCurrentStage_ptr          = (GetCurrentStage_type)GetProcAddress(InfinityZoneModule, "GetCurrentStage");
        }


        // Registers an event for when a stage loads
        void RegisterStageLoadEvent(StageLoadEvent event)
        {
            (*RegisterStageLoadEvent_ptr)(event);
        }

        // Registers an event for when a stage unloads
        void RegisterStageUnloadEvent(StageLoadEvent event)
        {
            (*RegisterStageUnloadEvent_ptr)(event);
        }

        // Loads and registers all stages
        void LoadStagesFile(const char* path)
        {
            (*LoadStagesFile_ptr)(path);
        }

        // Switches custom stages
        void ChangeStage(const char* key)
        {
            (*ChangeStage_ptr)(key);
        }

        // Performs an asset reset
        void PerformAssetReset()
        {
            (*PerformAssetReset_ptr)();
        }

        // Gets the key of the current custom stage
        StageInfo GetCurrentStage()
        {
            return (*GetCurrentStage_ptr)();
        }

    }
}