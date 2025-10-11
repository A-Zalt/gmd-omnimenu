#pragma once

#include <cocos2d.h>
#include "PlayerObject.hpp"

class PlayLayer : public cocos2d::CCLayer {
public:
    static PlayLayer* create(void*);
    bool init(void*);

    void resetLevel();
    void levelComplete();
    virtual PlayerObject* getPlayer();
};
