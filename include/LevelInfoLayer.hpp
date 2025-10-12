#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"

class LevelInfoLayer : public cocos2d::CCLayer {
public:
    static LevelInfoLayer* create(GJGameLevel*);
    bool init(GJGameLevel*);
    void onClone();
};
