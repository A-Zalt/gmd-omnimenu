#pragma once

#include <cocos2d.h>
#include "PlayerObject.hpp"

class GJGameLevel : cocos2d::CCNode {
public:

    static GJGameLevel* create();
    static GJGameLevel* create(cocos2d::CCDictionary dict);
    virtual bool init();
    virtual void setIsVerified(bool isVerified);
};
