#pragma once

#include <cocos2d.h>
#include "EndPortalObject.hpp"

class PlayerObject : public cocos2d::CCSprite {
public:
    static PlayerObject* create(int, cocos2d::CCLayer*);
    bool init(void*);
    void lockPlayer();

    virtual CCLayer* getGameLayer() const;
    virtual EndPortalObject* getPortalObject();
};
