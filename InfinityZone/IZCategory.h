#pragma once
#include "Common.h"

class IZScene;

class IZCategoryGroup
{
protected:
public:
    string GroupName;
    vector<IZScene*> Scenes;
};

class IZCategory
{
protected:
public:
    string CategoryName;
    vector<IZCategoryGroup*> Groups;
};