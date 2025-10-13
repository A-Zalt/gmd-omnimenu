#pragma once

#include <cocos2d.h>
#include "EndPortalObject.hpp"

class PlayerObject : public cocos2d::CCSprite {
public:
    static PlayerObject* create(int, cocos2d::CCLayer*);
    bool init(void*);
    void lockPlayer();
    void gravityDown();
    void gravityUp();
    void speedDown();
    void speedUp();
    void yStartDown();
    void yStartUp();
    void logValues();

    virtual CCLayer* getGameLayer() const;
    virtual EndPortalObject* getPortalObject();
};
