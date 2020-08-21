#pragma once
#include "Common.h"

typedef char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef short s16;
typedef int s32;

class IZBinary
{
protected:
    char* m_oldBuffer;
    char* m_buffer;
    unordered_map<string, char*> m_offsets;
public:
    IZBinary(char* buffer);

    int GetSize();

    // Writing
    void WriteU8(u8 value);
    void WriteS8(s8 value);
    void WriteU16(u16 value);
    void WriteS16(s16 value);
    void WriteU32(u32 value);
    void WriteS32(s32 value);
    void WriteSig(string text);
    void WriteString(string text);
    void WriteData(void* data, int size);
    void WriteOffset(string name);
    void FillOffset(string name);

    // Reading
    u8     ReadU8();
    s8     ReadS8();
    u16    ReadU16();
    s16    ReadS16();
    u32    ReadU32();
    s32    ReadS32();
    string ReadSig(int size);
    string ReadString();
    string ReadStringOffset();
};
