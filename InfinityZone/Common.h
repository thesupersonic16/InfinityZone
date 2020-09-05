#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <ManiaModLoader.h>
#include "SonicMania.h"


// TODO: Add this to ManiaModLoader.h
struct HelperFunctions
{
    // The version of the structure.
    int Version;
    // Registers a start position for a character.
    int(__cdecl* ReadBytesFromFile)(fileinfo* file, void* buffer, int bytes);
    // Replaces the source file with the destination file.
    void(__cdecl* AddReplaceFile)(const char* src, const char* dst);
    // Gets the real path to a replaceable file.
    const char* (__cdecl* GetReplaceablePath)(const char* path);
    // Checks if a replaceable file exist.
    bool(__cdecl* CheckFile)(const char* path);
};
// ----

using std::string;
using std::vector;
using std::unordered_map;

// Macros
#define IZ_EXPORT __declspec(dllexport)
#define IZ_APPLICATIONNAME "InfinityZone"
#define IZ_VERSION 0
#define IZ_MAX_SAVES 32
#define IZ_SAVE_FILE_SIZE 0x007FD70A // 7.99 MB (Mania has a 8 MB file limit)
#define IZAPI_MAJORVERSION 0
