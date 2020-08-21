#include "IZGameSession.h"
#include "Helpers.h"

int __cdecl StatusCheck(int status);
int __cdecl ReadStatusCheck(int status);

static char* SaveBuffer = (char*)malloc(IZ_SAVE_FILE_SIZE);
static IZGameSession* CurrentSession = nullptr;

IZGameSession::IZGameSession()
{
    CurrentSession = this;
    Reset();
}
IZGameSession::~IZGameSession()
{

}

void IZGameSession::SaveFile()
{
    // TODO
    LogDebug("InfinityZone::IZGameSession::SaveFile", "Building InfinityZone Save File...");

    SaveStorage.Reset();
    auto writer = IZBinary(SaveBuffer);
    SaveStorage.WriteData(writer);
    LogDebug("InfinityZone::IZGameSession::SaveFile", "Writing InfinityZone Save File (%d bytes) to Steam Cloud...", writer.GetSize());
    SonicMania::TrySaveUserFile("IZSaveData.bin", SaveBuffer, writer.GetSize(), StatusCheck, 0);
}

void IZGameSession::LoadFile()
{
    // TODO
    LogDebug("InfinityZone::IZGameSession::LoadFile", "Reading InfinityZone Save File from Steam Cloud...");
    SonicMania::TryLoadUserFile("IZSaveData.bin", SaveBuffer, IZ_SAVE_FILE_SIZE, ReadStatusCheck);
}

void IZGameSession::Reset()
{
}

int __cdecl ReadStatusCheck(int status)
{
    // TODO
    LogDebug("InfinityZone::IZGameSession::ReadStatusCheck", "Status Check: %d", status);
    LogDebug("InfinityZone::IZGameSession::ReadStatusCheck", "Processing InfinityZone Save File...");
    auto reader = IZBinary(SaveBuffer);
    CurrentSession->SaveStorage.Reset();
    CurrentSession->SaveStorage.LoadData(reader);

    return 0;
}

int __cdecl StatusCheck(int status)
{
    if (status == 200)
        LogDebug("InfinityZone::IZGameSession::StatusCheck", "Saved!", status);
    else
        LogDebug("InfinityZone::IZGameSession::StatusCheck", "Status Check: %d", status);
    return 0;
}