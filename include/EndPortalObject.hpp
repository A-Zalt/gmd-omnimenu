#pragma once

#include <cocos2d.h>

class EndPortalObject : public cocos2d::CCSprite {
public:
    static EndPortalObject* create();

    bool init(void*);
    virtual void triggerObject();
};
