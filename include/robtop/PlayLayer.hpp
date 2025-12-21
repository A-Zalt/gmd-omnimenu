#pragma once

#include <cocos2d.h>
#include "PlayerObject.hpp"

class PlayLayer : public cocos2d::CCLayer {
public:
    static PlayLayer* create(void*);
    bool init(void*);

    void resetLevel();
    void levelComplete();
    void removeLastCheckpoint();
    CCNode* getLastCheckpoint();
    virtual PlayerObject* getPlayer();

#if GAME_VERSION > GV_1_3
    void tintObjects(cocos2d::ccColor3B color, float duration);
#endif
};
