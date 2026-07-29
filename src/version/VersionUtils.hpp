#pragma once
#include "GameManager.hpp"
#include "UILayer.hpp"
#include "LevelInfoLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "addresses.hpp"
#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook
#include "EditorUI.hpp"
#include "LocalLevelManager.hpp"
#include "GameObject.hpp"
#include "DrawGridLayer.hpp"
#include "EditButtonBar.hpp"
#include "CCTextInputNode.hpp"
#include "ColorSelectPopup.hpp"
#include "LevelBrowserLayer.hpp"
#include "LevelSettingsObject.hpp"
#include "GameLevelManager.hpp"
#include "GameStatsManager.hpp"
#include "LeaderboardsLayer.hpp"
#include "Slider.hpp"

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

double getXVelocity(PlayerObject* player);
void setXVelocity(PlayerObject* player, double xVel);
void addXVelocity(PlayerObject* player, double adder);

double getGravity(PlayerObject* player);
void setGravity(PlayerObject* player, double gravity);
void addGravity(PlayerObject* player, double adder);

double getYStart(PlayerObject* player);
void setYStart(PlayerObject* player, double yStart);
void addYStart(PlayerObject* player, double adder);

float getCurrentPercentageF(PlayLayer* playLayer);
float getCurrentPercentageF();
int getCurrentPercentage(PlayLayer* playLayer);
int getCurrentPercentage();

GJGameLevel* getInfoLayerLevel(LevelInfoLayer* infoLayer);

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
cocos2d::CCLayer* getEditorGameLayer(LevelEditorLayer* editorLayer);
LevelEditorLayer* getUIEditorLayer(EditorUI* uiLayer);
GJGameLevel* getCellLevel(CCNode* cell);
#if GAME_VERSION > GV_1_0
int getCommentID(CCNode* comment);
#endif
GJGameLevel* getEditLayerLevel(CCLayer* editLayer);

CCParticleSystem* getShipLiftParticles(PlayerObject* player);
CCParticleSystem* getShipGroundParticles(PlayerObject* player);
CCParticleSystem* getGroundParticles(PlayerObject* player);
CCParticleSystem* getLandingParticles(PlayerObject* player);
CCParticleSystem* getLandingParticles2(PlayerObject* player);
CCParticleSystem* getShipFireParticles(PlayerObject* player);
CCParticleSystem* getBGParticles(PlayLayer* playLayer);
CCParticleSystem* getObjectParticles(void* object);
#if GAME_VERSION > GV_1_0
CCArray* getLocalLevels(LocalLevelManager* lolman);
#else
CCArray* getLocalLevels(GameLevelManager* glman);
#endif
CCArray* getLocalLevels();
bool getObjectUseAudioScale(void* object);
void setObjectUseAudioScale(void* object, bool uas);
void setFreeBuild(bool enable);

CCArray* getSelectedObjects(EditorUI* uiLayer);
void setSelectedObjects(EditorUI* uiLayer, CCArray* arr);
DrawGridLayer* getGridLayer(LevelEditorLayer* editLayer);
int getObjectType(GameObject* object);
CCArray* getRedoArray(LevelEditorLayer* editLayer);
GameObject* getSelectedObject(EditorUI* uiLayer);
CCSpriteBatchNode* getEditorBatchNode(LevelEditorLayer* editLayer);
bool getShouldSpawn(GameObject* object);

CCMenu* getEditorUIButtonMenu(EditorUI* uiLayer);

bool getOnGround(PlayerObject* player);
void setOnGround(PlayerObject* player, bool onGround);

EditButtonBar* getCreateButtonBar(EditorUI* uiLayer);
void setCreateButtonBar(EditorUI* uiLayer, EditButtonBar* bar);
CCArray* getCreateButtons(EditorUI* uiLayer);
void setCreateButtons(EditorUI* uiLayer, CCArray* array);
#if GAME_VERSION >= GV_1_7
CCArray* getCreateButtonBars(EditorUI* uiLayer);
void setCreateButtonBars(EditorUI* uiLayer, CCArray* bars);
#endif
float getScreenBottom();
float getUnkFloat(EditorUI* uiLayer);

CCArray* getEditorSections(LevelEditorLayer* editLayer);
int getObjectKey(GameObject* obj);

int getCurrentAttempts(PlayLayer* playLayer);
int getCurrentAttempts();
int getCurrentJumps(PlayLayer* playLayer);
int getCurrentJumps();
float getClkTimer(PlayLayer* playLayer);
float getClkTimer();
std::string getPlayerUDID();

CCLabelBMFont* getAttemptLabel(PlayLayer* layer);
CCLabelBMFont* getAttemptLabel();
CCArray* getPlaySections(PlayLayer* playLayer);
CCArray* getPlaySections();
bool compareXes2(CCObject* p1, CCObject* p2);
CCArray* getStartPositions_(PlayLayer* playLayer);
CCArray* getStartPositions_();
void setStartPos(PlayLayer* playLayer, CCPoint point);
void setStartPos(CCPoint point);
CCPoint getRealPosition(GameObject* object);

TextInputDelegate* getTextInputDelegate(CCTextInputNode* node);
void setTextInputDelegate(CCTextInputNode* node, TextInputDelegate* delegate);

cocos2d::extension::CCControlColourPicker* getColorPicker(ColorSelectPopup* popup);
cocos2d::extension::CCControlColourPicker* getColorPicker(ColorPickerPopup* popup);
int getCharLimit(CCTextInputNode* node);
void setCharLimit(CCTextInputNode* node, int limit);

void setEditButton(bool enable);
void setRestartButton(bool enable);

GJSearchObject* getSearchObject(LevelBrowserLayer* browser);
int getSearchType(GJSearchObject* searcher);
int& getSearchPage(GJSearchObject* searcher);
#if GAME_VERSION >= GV_1_4
void setIconHack(bool enable);
#endif

EditButtonBar* getEditButtonBar(EditorUI* uiLayer);
void setEditButtonBar(EditorUI* uiLayer, EditButtonBar* bar);
#if GAME_VERSION < GV_1_7
CCArray* getBarButtons(EditButtonBar* bar);
#else
CCArray* getBarPages(EditButtonBar* bar);
#endif
void setOriginalScale(CCMenuItemSpriteExtra* btn, float scale);

#if GAME_VERSION < GV_1_4
void setEditObjectButton(EditorUI* self, CCMenuItemSpriteExtra* btn);
#endif

int getGlobalOrderOfArrival();
LevelSettingsObject* getEditorSettingsObject(LevelEditorLayer* lel);
void setEditorSettingsObject(LevelEditorLayer* lel, LevelSettingsObject* settings);
int getAudioTrack(LevelSettingsObject* obj);

#if GAME_VERSION < GV_1_2
std::string getAllowedChars(CCTextInputNode* input);
#endif

#if GAME_VERSION >= GV_1_5
void setCharLimitBypass(bool enable);
void setProfanityBypass(bool enable);
void setBlockVerify(bool enable);
std::vector<uint8_t> uintptrToBytes(uintptr_t value);
bool getShowProgressBar();
CCSprite* getProgressBar(PlayLayer* playLayer);
CCSprite* getProgressBar();
CCMenuItemSpriteExtra* getZoomOutButton(EditorUI* uiLayer);
#endif

#if GAME_VERSION >= GV_1_6
void setClubstepBypass(bool enable);
#endif
const char* intToReadableGV(int gv);
#if GAME_VERSION < GV_1_4
void setNoShipTint(bool enable);
#endif
int getSectionIdx(GameObject* obj);

#if GAME_VERSION < GV_1_1
CCDictionary* getKeyTimers();
#else
bool getAutoRetry();
#if GAME_VERSION < GV_1_6
void setShouldRunDelayedReset(PlayLayer* playLayer, bool value);
#endif
#endif
GJGameLevel* getLELLevel(LevelEditorLayer* lel);
#if GAME_VERSION >= GV_1_7
CCArray* getSpeedObjects(DrawGridLayer* gridLayer);
int getStartSpeed(LevelSettingsObject* settings);
// float getLevelDistance(LevelEditorLayer* lel);
#endif
CCMenu* getCheckpointMenu(UILayer* self);
#if GAME_VERSION >= GV_1_3
LeaderboardState getLeaderboardState(LeaderboardsLayer* self);
void setLeaderboardState(LeaderboardsLayer* self, int value);
#endif
CCPoint getCameraPos(PlayLayer* self);
CCPoint getCameraPos();
#if GAME_VERSION >= GV_1_4
float getRadius(GameObject* obj);
#endif
bool getIsFlipped(PlayLayer* layer);
bool getIsFlipped();
float getDuration(ColorSelectPopup* popup);
void setDuration(ColorSelectPopup* popup, float value);
CCLabelBMFont* getDurationLabel(ColorSelectPopup* popup);
Slider* getCSPSlider(ColorSelectPopup* popup);
void setDecimals(char val);
#if GAME_VERSION == GV_1_4
bool getFlyMode(PlayerObject* player);
bool getGravityFlipped(PlayerObject* player);
float getPlayerScale(PlayerObject* player);
#endif
bool getCheckpointFlipped(CCNode* checkpoint);

#if GDPS == GDPS_NEOPOINTFOUR
CCDictionary* getSettingsDict(GameLevelManager* glman);
CCDictionary* getCompletedDict(GameStatsManager* gsman);
CCDictionary* getStatsDict(GameStatsManager* gsman);
int getPlayerShip(GameManager* gm);
CCLayer* getMainLayer(CCLayer* infoLayer);
#endif
int getTotalLevels(LevelBrowserLayer* browser);
const char* keyToFrame(int objectID);
CCPoint getTouchLocation(CCTouch* touch);
CCPoint getPreviousTouchLocation(CCTouch* touch);