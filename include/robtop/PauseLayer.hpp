#pragma once

#include <cocos2d.h>

class PauseLayer : public cocos2d::CCLayerColor {
public:
    static PauseLayer* create(void*);
    void onOpenMenu();
    void createSpeedhack();
    void createEye();
    void toggleVisibility();
};
