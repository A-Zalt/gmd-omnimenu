#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"

class LevelEditorLayer : public cocos2d::CCLayer
{
public:
    static LevelEditorLayer* create(GJGameLevel* level);
    bool init(GJGameLevel* level);

    virtual int getObjectCount();
};
