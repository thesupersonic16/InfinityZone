#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <ManiaModLoader.h>
#include "SonicMania.h"

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
