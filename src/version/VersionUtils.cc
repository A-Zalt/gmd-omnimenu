#include "GameManager.hpp"
#include "UILayer.hpp"
#include "LevelInfoLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "addresses.hpp"
#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

uintptr_t get_address(int offset) {
    void* handle = dlopen(MAIN_LIBRARY, RTLD_NOW);
    void* addr = dlsym(handle, "JNI_OnLoad"); // this symbol is present in every GD version according to akqanile/Adelfa

    Dl_info info;
    dladdr(addr, &info);
    return reinterpret_cast<uintptr_t>(info.dli_fbase) + offset;
}

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

int getLevelAttempts(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(int, level, GJGameLevel__m_attempts);
}
void setLevelAttempts(GJGameLevel* level, int attempts) {
    MEMBER_BY_OFFSET(int, level, GJGameLevel__m_attempts) = attempts;
}

GJGameLevel* getInfoLayerLevel(LevelInfoLayer* infoLayer) {
    return MEMBER_BY_OFFSET(GJGameLevel*, infoLayer, LevelInfoLayer__m_level);
}

int getLevelID(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(int, level, GJGameLevel__m_levelID);
}
std::string getLevelName(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(std::string, level, GJGameLevel__m_levelName);
}
std::string getLevelUsername(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(std::string, level, GJGameLevel__m_username);
}
int getLevelUserID(GJGameLevel* level) {
    return MEMBER_BY_OFFSET(int, level, GJGameLevel__m_userID);
}

template <typename T>
std::vector<uint8_t> toBytesLE(T value) {
    std::vector<uint8_t> bytes(sizeof(T));
    for (size_t i = 0; i < sizeof(T); ++i)
        bytes[i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
    return bytes;
}
void setObjectLimit(int limit) {
    DobbyCodePatch(
        reinterpret_cast<void*>(get_address(object_limit)),
        toBytesLE(limit).data(), 2
    );
}

cocos2d::CCArray* getPlayLayerHazards(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(cocos2d::CCArray*, playLayer, PlayLayer__m_hazards);
}
cocos2d::CCArray* getPlayLayerHazards() {
    return getPlayLayerHazards(getPlayLayer());
}
int getObjectCount(LevelEditorLayer* editorLayer) {
    return MEMBER_BY_OFFSET(int, editorLayer, LevelEditorLayer__m_objectCount);
}
void setObjectCount(LevelEditorLayer* editorLayer, int objectCount) {
    MEMBER_BY_OFFSET(int, editorLayer, LevelEditorLayer__m_objectCount) = objectCount;
}
LevelEditorLayer* getLevelEditorLayer(void* pauseLayer) {
    return MEMBER_BY_OFFSET(LevelEditorLayer*, pauseLayer, EditorPauseLayer__m_editorLayer);
}

bool getPlayLayerPractice(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(bool, playLayer, PlayLayer__m_isPractice);
}
bool getPlayLayerPractice() {
    return getPlayLayerPractice(getPlayLayer());
}
void setPlayLayerPractice(PlayLayer* playLayer, bool isPractice) {
    MEMBER_BY_OFFSET(bool, playLayer, PlayLayer__m_isPractice) = isPractice;
}
void setPlayLayerPractice(bool isPractice) {
    return setPlayLayerPractice(getPlayLayer(), isPractice);
}
cocos2d::CCArray* getPlayLayerCheckpoints(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(cocos2d::CCArray*, playLayer, PlayLayer__m_checkpoints);
}
cocos2d::CCArray* getPlayLayerCheckpoints() {
    return getPlayLayerCheckpoints(getPlayLayer());
}