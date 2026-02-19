#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"

class ObjectToolbox : public cocos2d::CCNode {
public:
    static ObjectToolbox* sharedState();
    const char* keyToFrame(char const* key);
};
