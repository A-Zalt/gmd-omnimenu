#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"

#if GAME_VERSION > GV_1_0
typedef enum {
    Level = 1,
    Comment = 2
} LikeItemType;
#endif

class GameLevelManager : public cocos2d::CCNode {
public:
    static GameLevelManager* sharedState();

    void onUploadLevelCompleted(cocos2d::CCNode* p1, void* p2);
    GJGameLevel* getMainLevel(int id);
    GJGameLevel* createNewLevel();
    void saveLevel(GJGameLevel*);
    bool getBoolForKey(const char* key);
    void setBoolForKey(bool, const char* key);
    void updateUserScore();
    void getLeaderboardScores(char const*);
    cocos2d::CCArray* getStoredOnlineLevels(char const*);
#if GAME_VERSION > GV_1_0
    void resetTimerForKey(const char* key);
    bool hasLikedItem(LikeItemType, int, bool);
#endif
};
