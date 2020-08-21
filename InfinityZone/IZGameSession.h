#pragma once
#include <cstdint>
#include "Common.h"
#include "IZBinary.h"
#include "IZDataSystem.h"

class IZGameSession
{
protected:
public:
    IZDataSystem SaveStorage;

    IZGameSession();
    ~IZGameSession();

    void SaveFile();
    void LoadFile();
    // Cleans the session
    void Reset();
};