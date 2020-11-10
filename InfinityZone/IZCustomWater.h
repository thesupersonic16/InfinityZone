#pragma once
#include "SonicMania.h"
#include "depends/tinyxml2/tinyxml2.h"
class IZCustomWater
{
protected:
public:
    struct WaterColorDefintion
    {
        SonicMania::InkEffect InkEffect;
        int Alpha;
        SonicMania::Color Color;

        WaterColorDefintion()
        {

        }

        WaterColorDefintion(SonicMania::Color color, int alpha, SonicMania::InkEffect effect) : WaterColorDefintion()
        {
            InkEffect = effect;
            Alpha = alpha;
            Color = color;
        }
    };

    WaterColorDefintion WaterSettings;

    bool LoadXML(tinyxml2::XMLElement* xmlColors);
    WaterColorDefintion LoadCustomWaterFromXML(tinyxml2::XMLElement* xmlColor);

    IZCustomWater();
};

extern void PatchInfinityZoneCustomWater();

