#pragma once
#include <cstdint>
#include "Common.h"
#include "IZBinary.h"
struct DataTag
{
    void* Location;
    size_t Size;
};

class IZDataSystem
{
protected:
    unordered_map<string, DataTag> m_DataTags;
    unordered_map<string, u8>      m_u8Tags;
    unordered_map<string, u16>     m_u16Tags;
    unordered_map<string, u32>     m_u32Tags;
    unordered_map<string, string>  m_StringTags;
public:
    void WriteData(IZBinary& writer);
    void LoadData(IZBinary& reader);

    void SetU8(string name, u8 value);
    void SetU16(string name, u16 value);
    void SetU32(string name, u32 value);
    void SetS8(string name, s8 value);
    void SetS16(string name, s16 value);
    void SetS32(string name, s32 value);
    void SetString(string name, string value);
    void SetData(string name, void* value, size_t size);

    u8  GetU8(string name);
    u16 GetU16(string name);
    u32 GetU32(string name);
    s8  GetS8(string name);
    s16 GetS16(string name);
    s32 GetS32(string name);
    string GetString(string name);

    void Reset();
};