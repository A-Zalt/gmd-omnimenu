#pragma once

#include <cocos2d.h>

class PlayLayer : public cocos2d::CCLayer {
public:
    static PlayLayer* create(void*);
    bool init(void*);

    void resetLevel();
    void levelComplete();
};
