#pragma once
#include "GameManager.hpp"
#include "UILayer.hpp"
#include "LevelInfoLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "addresses.hpp"
#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

uintptr_t get_address(int offset);

PlayLayer* getPlayLayer();

PlayerObject* getPlayer(PlayLayer* playLayer);
PlayerObject* getPlayer();
GJGameLevel* getPlayLayerLevel(PlayLayer* playLayer);
GJGameLevel* getPlayLayerLevel();
UILayer* getUILayer(PlayLayer* playLayer);
UILayer* getUILayer();

GJLevelType getLevelType(GJGameLevel* level);
void setLevelType(GJGameLevel* level, GJLevelType type);

bool getLevelVerified(GJGameLevel* level);
void setLevelVerified(GJGameLevel* level, bool isVerified);

double getXVelocity(PlayerObject* player);
void setXVelocity(PlayerObject* player, double xVel);
void addXVelocity(PlayerObject* player, double adder);

double getGravity(PlayerObject* player);
void setGravity(PlayerObject* player, double gravity);
void addGravity(PlayerObject* player, double adder);

double getYStart(PlayerObject* player);
void setYStart(PlayerObject* player, double yStart);
void addYStart(PlayerObject* player, double adder);

int getLevelNormalPercent(GJGameLevel* level);
int getLevelPracticePercent(GJGameLevel* level);
void setLevelNormalPercent(GJGameLevel* level, int percent);
void setLevelPracticePercent(GJGameLevel* level, int percent);
int getCurrentPercentage(PlayLayer* playLayer);
int getCurrentPercentage();

int getLevelAttempts(GJGameLevel* level);
void setLevelAttempts(GJGameLevel* level, int attempts);
GJGameLevel* getInfoLayerLevel(LevelInfoLayer* infoLayer);

int getLevelID(GJGameLevel* level);
std::string getLevelName(GJGameLevel* level);
std::string getLevelUsername(GJGameLevel* level);
int getLevelUserID(GJGameLevel* level);

template <typename T>
std::vector<uint8_t> toBytesLE(T value);

cocos2d::CCArray* getPlayLayerHazards(PlayLayer* playLayer);
cocos2d::CCArray* getPlayLayerHazards();
int getObjectCount(LevelEditorLayer* editorLayer);
void setObjectCount(LevelEditorLayer* editorLayer, int objectCount);
LevelEditorLayer* getLevelEditorLayer(void* pauseLayer);

bool getPlayLayerPractice(PlayLayer* playLayer);
bool getPlayLayerPractice();
void setPlayLayerPractice(PlayLayer* playLayer, bool isPractice);
void setPlayLayerPractice(bool isPractice);
cocos2d::CCArray* getPlayLayerCheckpoints(PlayLayer* playLayer);
cocos2d::CCArray* getPlayLayerCheckpoints();
cocos2d::CCPoint getCheckpointPosition(CCNode* checkpoint);
cocos2d::CCPoint getStartPos(PlayLayer* playLayer);
cocos2d::CCPoint getStartPos();
CCLayer* getSelectLayerScroll(CCLayer* selectLayer);
int getCurrentScrollScreen(CCLayer* scrollLayer);
int getScrollPageCount(CCLayer* scrollLayer);
int getCurrentScrollIndex(CCLayer* scrollLayer);
std::string getPlayerName();
void setObjectLimit(int limit);
void setZoomBypass(bool enabled);