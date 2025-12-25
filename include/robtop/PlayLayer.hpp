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
    void resume();
#if GAME_VERSION > GV_1_0
    void delayedResetLevel();
#else
    static void resetLevelLogic(PlayLayer* self);
    void turnOffTheThing();
#endif
#if GAME_VERSION < GV_1_6
    void customResetLevel();
#endif
#if GAME_VERSION >= GV_1_7
    float timeForXPos(float xPos, bool something);
#endif

#if GAME_VERSION > GV_1_3
    void tintObjects(cocos2d::ccColor3B color, float duration);
#endif
};
