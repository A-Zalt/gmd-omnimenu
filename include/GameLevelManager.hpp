#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"

class GameLevelManager : public cocos2d::CCNode {
public:
    static GameLevelManager* sharedState();

    void onUploadLevelCompleted(cocos2d::CCNode* p1, void* p2);
};
