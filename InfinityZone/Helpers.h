#pragma once
#include "Common.h"

// Functions
extern intptr_t GetAddressFromCall(intptr_t call);
extern intptr_t GetAddressFromJump(intptr_t jmp);
extern int LoadFile(char* filename, fileinfo* info, void* unknown);
extern void* LoadAndReadFile(const char* filename, int* length);
extern int ReadBytesFromFile(fileinfo* file, void* buffer, int bytes);
extern int DecryptBytes(fileinfo* file, void* buffer, int bufferSize);
extern void ReplaceString(string& str, string& from, string& to);
extern void LogDebug(const char* name, const char* text, ...);
extern void LogError(const char* name, const char* text, ...);
extern void LogWarn(const char* name, const char* text, ...);
extern void LogInfo(const char* name, const char* text, ...);