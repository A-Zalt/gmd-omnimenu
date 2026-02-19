#pragma once

#include <cocos2d.h>

class EndLevelLayer : public cocos2d::CCLayer {
public:
    static EndLevelLayer* create();
    void toggleVisibility();
};
