#pragma once

#include <cocos2d.h>

class GameStatsManager : public cocos2d::CCNode {
public:
    static GameStatsManager* sharedState();

    int getStat(const char* stat);
    int incrementStat(const char* key, int value);
};
