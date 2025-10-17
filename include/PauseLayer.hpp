#pragma once

#include <cocos2d.h>

class PauseLayer : public cocos2d::CCLayer {
public:
    static PauseLayer* create(void*);
    void onOpenMenu();
};
