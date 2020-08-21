#include "Helpers.h"
#include "IZDataSystem.h"

void IZDataSystem::WriteData(IZBinary& writer)
{
    // Header
    writer.WriteSig("IZSF"); // Signature
    
    // Location Table
    writer.WriteU32(m_DataTags.size());   //   Data Tag Count
    writer.WriteU32(m_u8Tags.size());     //  8 Bit Tag Count
    writer.WriteU32(m_u16Tags.size());    // 16 Bit Tag Count
    writer.WriteU32(m_u32Tags.size());    // 32 Bit Tag Count
    writer.WriteU32(m_StringTags.size()); // String Tag Count
    for (std::pair<string, DataTag> tag : m_DataTags)
    {
        writer.WriteOffset(string("nl_") + tag.first); // Name Location
        writer.WriteOffset(string("dl_") + tag.first); // Data Location
        writer.WriteU32(tag.second.Size);              // Data Size
    }
    for (std::pair<string, u8> tag : m_u8Tags)
    {
        writer.WriteOffset(string("nl_") + tag.first); // Name Location
        writer.WriteU32(tag.second);                   // Data Location
    }
    for (std::pair<string, u16> tag : m_u16Tags)
    {
        writer.WriteOffset(string("nl_") + tag.first); // Name Location
        writer.WriteU32(tag.second);                   // Data Location
    }
    for (std::pair<string, u32> tag : m_u32Tags)
    {
        writer.WriteOffset(string("nl_") + tag.first); // Name Location
        writer.WriteU32(tag.second);                   // Data Location
    }
    for (std::pair<string, string> tag : m_StringTags)
    {
        writer.WriteOffset(string("nl_") + tag.first); // Name Location
        writer.WriteOffset(string("dl_") + tag.first); // Data Location
    }
    // Name Table
    for (std::pair<string, DataTag> tag : m_DataTags)
    {
        writer.FillOffset(string("nl_") + tag.first); // Name Location
        writer.WriteString(tag.first);                // Data Location
    }
    for (std::pair<string, u8> tag : m_u8Tags)
    {
        writer.FillOffset(string("nl_") + tag.first); // Name Location
        writer.WriteString(tag.first);                // Data Location
    }
    for (std::pair<string, u16> tag : m_u16Tags)
    {
        writer.FillOffset(string("nl_") + tag.first); // Name Location
        writer.WriteString(tag.first);                // Data Location
    }
    for (std::pair<string, u32> tag : m_u32Tags)
    {
        writer.FillOffset(string("nl_") + tag.first); // Name Location
        writer.WriteString(tag.first);                // Data Location
    }
    for (std::pair<string, string> tag : m_StringTags)
    {
        writer.FillOffset(string("nl_") + tag.first); // Name Location
        writer.WriteString(tag.first);                // Data Location
    }

    // Data
    for (std::pair<string, DataTag> tag : m_DataTags)
    {
        writer.FillOffset(string("dl_") + tag.first);
        writer.WriteData(tag.second.Location, tag.second.Size);
    }
    for (std::pair<string, string> tag : m_StringTags)
    {
        writer.FillOffset(string("dl_") + tag.first);
        writer.WriteString(tag.second);
    }
    LogDebug("InfinityZone::IZDataSystem::WriteData", "Written all tags to buffer (%d bytes)!", writer.GetSize());
}
void IZDataSystem::LoadData(IZBinary& reader)
{
    // Header
    string sig = reader.ReadSig(4);
    if (sig != "IZSF")
    {
        LogError("InfinityZone::IZDataSystem::LoadData", "Failed to read Data File! Signature does not match!");
        return;
    }

    // Location Table
    int dataTags   = reader.ReadU32();
    int u8Tags     = reader.ReadU32();
    int u16Tags    = reader.ReadU32();
    int u32Tags    = reader.ReadU32();
    int stringTags = reader.ReadU32();
    for (int i = 0; i < dataTags; ++i)
    {
        string name = reader.ReadStringOffset();
        void* location = (void*)reader.ReadU32();
        size_t size = reader.ReadU32();
        void* buffer = malloc(size);
        memcpy(buffer, location, size);
        m_DataTags[name] = { malloc(size), size };
    }
    for (int i = 0; i < u8Tags; ++i)
    {
        string name = reader.ReadStringOffset();
        m_u8Tags[name] = reader.ReadU8();
    }
    for (int i = 0; i < u16Tags; ++i)
    {
        string name = reader.ReadStringOffset();
        m_u16Tags[name] = reader.ReadU16();
    }
    for (int i = 0; i < u32Tags; ++i)
    {
        string name = reader.ReadStringOffset();
        m_u32Tags[name] = reader.ReadU32();
    }
    for (int i = 0; i < stringTags; ++i)
    {
        string name = reader.ReadStringOffset();
        m_StringTags[name] = reader.ReadStringOffset();
    }
    LogDebug("InfinityZone::IZDataSystem::LoadData", "Read all tags from buffer!");
}
void IZDataSystem::Reset()
{
    m_u8Tags.clear();
    m_u16Tags.clear();
    m_u32Tags.clear();
    m_StringTags.clear();
}

void IZDataSystem::SetU8(string name, u8 value)
{
    m_u8Tags[name] = value;
}
void IZDataSystem::SetU16(string name, u16 value)
{
    m_u16Tags[name] = value;
}
void IZDataSystem::SetU32(string name, u32 value)
{
    m_u32Tags[name] = value;
}
void IZDataSystem::SetS8(string name, s8 value)
{
    m_u8Tags[name] = value;
}
void IZDataSystem::SetS16(string name, s16 value)
{
    m_u16Tags[name] = value;
}
void IZDataSystem::SetS32(string name, s32 value)
{
    m_u32Tags[name] = value;
}
void IZDataSystem::SetString(string name, string value)
{
    m_StringTags[name] = value;
}
void IZDataSystem::SetData(string name, void* value, size_t size)
{
    auto data = malloc(size);
    memcpy(data, value, size);
    m_DataTags[name] = {data, size};
}

u8 IZDataSystem::GetU8(string name)
{
    return m_u8Tags[name];
}
u16 IZDataSystem::GetU16(string name)
{
    return m_u16Tags[name];
}
u32 IZDataSystem::GetU32(string name)
{
    return m_u32Tags[name];
}
s8 IZDataSystem::GetS8(string name)
{
    return (s8)m_u8Tags[name];
}
s16 IZDataSystem::GetS16(string name)
{
    return (s16)m_u16Tags[name];
}
s32 IZDataSystem::GetS32(string name)
{
    return (s32)m_u32Tags[name];
}
string IZDataSystem::GetString(string name)
{
    return m_StringTags[name];
}