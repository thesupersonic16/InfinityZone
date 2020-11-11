#include "IZAPI.h"
#include <string>

#define IZAPI_MAJORVERSION 0

extern "C"
{
    namespace IZAPI
    {
        HMODULE InfinityZoneModule;
        char VersionString[64];

        // Function Types
        typedef int(__cdecl* GetIZVersion_type)();
        typedef void(__cdecl* RegisterStageLoadEvent_type)(StageLoadEvent);
        typedef void(__cdecl* CString3Func)(const char*, const char*, const char*);
        typedef void(__cdecl* CString2Func)(const char*, const char*);
        typedef void(__cdecl* CStringFunc)(const char*);
        typedef void(__cdecl* Func)();
        typedef StageInfo(__cdecl* GetCurrentStage_type)();
        typedef void(__cdecl* GetIZScenes_type)(StageInfo* buffer, size_t limit);
        typedef size_t(__cdecl* GetIZStageCount_type)();

        GetIZVersion_type GetIZVersion_ptr = nullptr;
        GetIZVersion_type GetIZAPIMajorVersion_ptr = nullptr;
        RegisterStageLoadEvent_type RegisterStageLoadEvent_ptr = nullptr;
        RegisterStageLoadEvent_type RegisterStageUnloadEvent_ptr = nullptr;
        CStringFunc LoadStagesFile_ptr = nullptr;
        CStringFunc ChangeScene_ptr = nullptr;
        Func PerformAssetReset_ptr = nullptr;
        GetCurrentStage_type GetCurrentStage_ptr = nullptr;
        CString3Func SetStageAsset_ptr = nullptr;
        CString2Func SetGlobalAsset_ptr = nullptr;
        GetIZScenes_type GetIZScenes_ptr = nullptr;
        GetIZStageCount_type GetIZStageCount_ptr = nullptr;

        void ShowError(const char* error)
        {
            char message[255];
            sprintf_s(message, "%s\n\nIZAPI Information:\nIZAPI Version: %d\nIZ Version: %s", error, IZAPI_MAJORVERSION, VersionString);
            MessageBoxA(NULL, message, "Fatal InfinityZone Error", MB_OK | MB_ICONERROR);
        }

        bool IZInit()
        {
            // Write placeholder
            strcpy_s(VersionString, "NOT LOADED");
            
            // Detect InfinityZone
            InfinityZoneModule = DetectIZDLL();
            if (!InfinityZoneModule)
            {
                ShowError("Could not detect InfinityZone!\nPlease make sure InfinityZone is installed and enabled.");
                return false;
            }

            // Pull exports
            GetIZVersion_ptr             = (GetIZVersion_type)GetProcAddress(InfinityZoneModule, "GetIZVersion");
            GetIZAPIMajorVersion_ptr     = (GetIZVersion_type)GetProcAddress(InfinityZoneModule, "GetIZAPIMajorVersion");
            RegisterStageLoadEvent_ptr   = (RegisterStageLoadEvent_type)GetProcAddress(InfinityZoneModule, "RegisterStageLoadEvent");
            RegisterStageUnloadEvent_ptr = (RegisterStageLoadEvent_type)GetProcAddress(InfinityZoneModule, "RegisterStageUnloadEvent");
            LoadStagesFile_ptr           = (CStringFunc)GetProcAddress(InfinityZoneModule, "LoadStagesFile");
            ChangeScene_ptr              = (CStringFunc)GetProcAddress(InfinityZoneModule, "ChangeScene");
            PerformAssetReset_ptr        = (Func)GetProcAddress(InfinityZoneModule, "PerformAssetReset");
            GetCurrentStage_ptr          = (GetCurrentStage_type)GetProcAddress(InfinityZoneModule, "GetCurrentStage");
            SetStageAsset_ptr            = (CString3Func)GetProcAddress(InfinityZoneModule, "SetStageAsset");
            SetGlobalAsset_ptr           = (CString2Func)GetProcAddress(InfinityZoneModule, "SetGlobalAsset");
            GetIZScenes_ptr              = (GetIZScenes_type)GetProcAddress(InfinityZoneModule, "GetIZScenes");
            GetIZStageCount_ptr          = (GetIZStageCount_type)GetProcAddress(InfinityZoneModule, "GetIZStageCount");

            // Read Version from InfinityZone
            if (GetIZVersion_ptr && GetIZAPIMajorVersion_ptr)
                sprintf_s(VersionString, "IZ_VERSION: %d, IZAPI_MAJORVERSION: %d", GetIZVersion(), GetIZAPIMajorVersion());

            // Check IZAPI base version
            if (GetIZAPIMajorVersion_ptr && GetIZAPIMajorVersion() < IZAPI_MAJORVERSION)
            {
                ShowError("The detected version of InfinityZone is out of date!\nPlease make sure you are running the latest version of InfinityZone.\n");
                return false;
            }

        }

        HMODULE DetectIZDLL()
        {
            return LoadLibraryA("InfinityZone.dll");
        }

        // Gets the version of InfinityZone
        int GetIZVersion()
        {
            return (*GetIZVersion_ptr)();
        }

        // Gets the supported version of IZAPI
        int GetIZAPIMajorVersion()
        {
            return (*GetIZAPIMajorVersion_ptr)();
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

        // Loads a custom scene
        void ChangeScene(const char* sceneKey)
        {
            (*ChangeScene_ptr)(sceneKey);
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

        void SetStageAsset(const char* stageKey, const char* basePath, const char* newPath)
        {
            return (*SetStageAsset_ptr)(stageKey, basePath, newPath);
        }

        // Sets a global asset redirect
        void SetGlobalAsset(const char* basePath, const char* newPath)
        {
            return (*SetGlobalAsset_ptr)(basePath, newPath);
        }

        void GetIZScenes(StageInfo* buffer, size_t limit)
        {
            return (*GetIZScenes_ptr)(buffer, limit);
        }

        size_t GetIZSceneCount()
        {
            return (*GetIZStageCount_ptr)();
        }

    }
}