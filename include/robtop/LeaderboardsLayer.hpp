#pragma once

#include <cocos2d.h>

typedef enum {
    Top = 1,
    Global = 2,
    Creators = 3,
#if GAME_VERSION >= GV_1_6
    Week = 4
#endif
} LeaderboardState;

class LeaderboardsLayer : public cocos2d::CCLayer {
public:
    static LeaderboardsLayer* create(LeaderboardState);

    void selectLeaderboard(LeaderboardState);
    void onRefresh();
    void onStarFilter(CCObject* sender);
    void onDemonFilter(CCObject* sender);
    void updateFilterButtons();
    void selectLeaderboard(int state);
};
