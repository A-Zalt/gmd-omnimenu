#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"

class LevelSelectLayer : public cocos2d::CCLayer {
public:
    static LevelInfoLayer* create(int);
    bool init(int);

    void onViewLevelInfo();
};
