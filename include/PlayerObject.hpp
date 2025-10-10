#pragma once

#include <cocos2d.h>

class PlayerObject : public cocos2d::CCSprite {
public:
    static PlayerObject* create(int, cocos2d::CCLayer*);
    bool init(void*);

    virtual CCLayer* getGameLayer() const;
};
