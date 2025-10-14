#include "GameManager.hpp"
#include "GJGameLevel.hpp"
#include "UILayer.hpp"

#define GAME_VERSION 3
#define READABLE_GAME_VERSION "1.2"
#define READABLE_GAME_VERSION_FULL "1.22"
#define MENU_VERSION "0.41"
#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

/****************************************************/
/*/                    OFFSETS                     /*/
/**/ #define GJGameLevel__m_isVerified      0x159 /**/
/**/ #define GJGameLevel__m_levelType       0x1a0 /**/
/**/ #define GJGameLevel__m_normalPercent   0x168 /**/
/**/ #define GJGameLevel__m_practicePercent 0x16c /**/
/**/ #define GameManager__m_playLayer       0x150 /**/
/**/ #define PlayLayer__m_lastX             0x1d8 /**/
/**/ #define PlayLayer__m_level             0x230 /**/
/**/ #define PlayLayer__m_playerObject      0x22c /**/
/**/ #define PlayLayer__m_uiLayer           0x228 /**/
/**/ #define PlayerObject__m_gravity        0x350 /**/
/**/ #define PlayerObject__m_xVelocity      0x340 /**/
/**/ #define PlayerObject__m_yStart         0x348 /**/
/*/                                                /*/
/****************************************************/

PlayLayer* getPlayLayer() {
    GameManager* gman = GameManager::sharedState();
    return MEMBER_BY_OFFSET(PlayLayer*, gman, GameManager__m_playLayer); 
}

PlayerObject* getPlayer(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(PlayerObject*, playLayer, PlayLayer__m_playerObject);
}
PlayerObject* getPlayer() {
    return getPlayer(getPlayLayer());
}

GJGameLevel* getPlayLayerLevel(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(GJGameLevel*, playLayer, PlayLayer__m_level);
}
GJGameLevel* getPlayLayerLevel() {
    return getPlayLayerLevel(getPlayLayer());
}

UILayer* getUILayer(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(UILayer*, playLayer, PlayLayer__m_uiLayer);
}
UILayer* getUILayer() {
    return getUILayer(getPlayLayer());
}

GJLevelType getLevelType(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(GJLevelType, level, GJGameLevel__m_levelType);
}
void setLevelType(GJGameLevel* level, GJLevelType type) {
    MEMBER_BY_OFFSET(GJLevelType, level, GJGameLevel__m_levelType) = type;
}

bool getLevelVerified(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(bool, level, GJGameLevel__m_isVerified);
}
void setLevelVerified(GJGameLevel* level, bool isVerified) {
    MEMBER_BY_OFFSET(bool, level, GJGameLevel__m_isVerified) = isVerified;
}

double getXVelocity(PlayerObject* player) {
    return MEMBER_BY_OFFSET(double, player, PlayerObject__m_xVelocity);
}
void setXVelocity(PlayerObject* player, double xVel) {
    MEMBER_BY_OFFSET(double, player, PlayerObject__m_xVelocity) = xVel;
}
void addXVelocity(PlayerObject* player, double adder) {
    MEMBER_BY_OFFSET(double, player, PlayerObject__m_xVelocity) += adder;
}

double getGravity(PlayerObject* player) {
    return MEMBER_BY_OFFSET(double, player, PlayerObject__m_gravity);
}
void setGravity(PlayerObject* player, double gravity) {
    MEMBER_BY_OFFSET(double, player, PlayerObject__m_gravity) = gravity;
}
void addGravity(PlayerObject* player, double adder) {
    MEMBER_BY_OFFSET(double, player, PlayerObject__m_gravity) += adder;
}

double getYStart(PlayerObject* player) {
    return MEMBER_BY_OFFSET(double, player, PlayerObject__m_yStart);
}
void setYStart(PlayerObject* player, double yStart) {
    MEMBER_BY_OFFSET(double, player, PlayerObject__m_yStart) = yStart;
}
void addYStart(PlayerObject* player, double adder) {
    MEMBER_BY_OFFSET(double, player, PlayerObject__m_yStart) += adder;
}

int getLevelNormalPercent(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(int, level, GJGameLevel__m_normalPercent);
}
int getLevelPracticePercent(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(int, level, GJGameLevel__m_practicePercent);
}
void setLevelNormalPercent(GJGameLevel* level, int percent) {
    MEMBER_BY_OFFSET(int, level, GJGameLevel__m_normalPercent) = percent;
}
void setLevelPracticePercent(GJGameLevel* level, int percent) {
    MEMBER_BY_OFFSET(int, level, GJGameLevel__m_practicePercent) = percent;
}
int getCurrentPercentage(PlayLayer* playLayer) {
    PlayerObject* player = getPlayer(playLayer);
    int percent = floorf((player->getPositionX() / MEMBER_BY_OFFSET(float, playLayer, PlayLayer__m_lastX)) * 100.0); // from destroyPlayer
    if (percent < 0) return 0;
    if (percent > 100) return 100;
    return percent;
}
int getCurrentPercentage() {
    return getCurrentPercentage(getPlayLayer());
}