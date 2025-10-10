#pragma once

#include <cocos2d.h>

using namespace cocos2d;

class GJGameLevel;

class LevelInfoLayer : public CCLayer
{
public:
    bool init(GJGameLevel*);
    void onClone();
};