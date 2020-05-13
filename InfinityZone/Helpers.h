#pragma once
#include "Common.h"

// Functions
extern intptr_t GetAddressFromJump(intptr_t jmp);
extern int LoadFile(char* filename, fileinfo* info, void* unknown);
extern void* LoadAndReadFile(const char* filename, int* length);
extern int ReadBytesFromFile(fileinfo* file, void* buffer, int bytes);
extern int DecryptBytes(fileinfo* file, void* buffer, int bufferSize);
extern void ReplaceString(string& str, string& from, string& to);