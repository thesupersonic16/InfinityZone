#include "IZBinary.h"

IZBinary::IZBinary(char* buffer)
{
    m_buffer = m_oldBuffer = buffer;
}

int IZBinary::GetSize()
{
    return m_buffer - m_oldBuffer;
}

void IZBinary::WriteU8(u8 value)
{
    m_buffer[0] = value;
    ++m_buffer;
}

void IZBinary::WriteS8(s8 value)
{
    m_buffer[0] = value;
    ++m_buffer;
}

void IZBinary::WriteU16(u16 value)
{
    *(u16*)m_buffer = value;
    m_buffer += sizeof(u16);
}

void IZBinary::WriteS16(s16 value)
{
    *(s16*)m_buffer = value;
    m_buffer += sizeof(s16);
}

void IZBinary::WriteU32(u32 value)
{
    *(u32*)m_buffer = value;
    m_buffer += sizeof(u32);
}

void IZBinary::WriteS32(s32 value)
{
    *(s32*)m_buffer = value;
    m_buffer += sizeof(s32);
}

void IZBinary::WriteSig(string text)
{
    memcpy(m_buffer, text.c_str(), text.length());
    m_buffer += text.length();
}

void IZBinary::WriteString(string text)
{
    int length = text.length();
    m_buffer[0] = (char)length;
    memcpy(m_buffer + 1, text.c_str(), length);
    m_buffer += length + 1;
}

void IZBinary::WriteData(void* data, int size)
{
    memcpy(m_buffer, data, size);
    m_buffer += size;
}

void IZBinary::WriteOffset(string name)
{
    m_offsets[name] = m_buffer;
    m_buffer += sizeof(char*);
}

void IZBinary::FillOffset(string name)
{
    *(size_t*)(m_offsets[name]) = (size_t)(m_buffer - m_oldBuffer);
    m_offsets.erase(name);
}


u8 IZBinary::ReadU8()
{
    u8 value = m_buffer[0];
    ++m_buffer;
    return value;
}

s8 IZBinary::ReadS8()
{
    s8 value = m_buffer[0];
        ++m_buffer;
    return value;
}

u16 IZBinary::ReadU16()
{
    u16 value = *(u16*)m_buffer;
    m_buffer += sizeof(u16);
    return value;
}

s16 IZBinary::ReadS16()
{
    s16 value = *(s16*)m_buffer;
    m_buffer += sizeof(s16);
    return value;
}

u32 IZBinary::ReadU32()
{
    u32 value = *(u32*)m_buffer;
    m_buffer += sizeof(u32);
    return value;
}

s32 IZBinary::ReadS32()
{
    s32 value = *(s32*)m_buffer;
    m_buffer += sizeof(s32);
    return value;
}

string IZBinary::ReadSig(int size)
{
    string value = string(m_buffer, size);
    m_buffer += size;
    return value;
}

string IZBinary::ReadString()
{
    int length = m_buffer[0];
    string value = string(m_buffer + 1, length);
    m_buffer += length + 1;
    return value;
}
string IZBinary::ReadStringOffset()
{
    char* newBuffer = (char*)(m_oldBuffer + ReadU32());
    return string(newBuffer + 1, (int)newBuffer[0]);
}

