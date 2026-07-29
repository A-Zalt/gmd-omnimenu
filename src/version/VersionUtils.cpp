#include "VersionUtils.hpp"
#include "GameManager.hpp"
#include "UILayer.hpp"
#include "LevelInfoLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "addresses.hpp"
#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook
#include "EditorUI.hpp"
#include "LocalLevelManager.hpp"
#include "DrawGridLayer.hpp"
#include "EditButtonBar.hpp"
#include "GameObject.hpp"
#include <algorithm>
#include "CCTextInputNode.hpp"
#include "LevelBrowserLayer.hpp"
#include "LeaderboardsLayer.hpp"
#if GAME_VERSION > GV_1_2
#include "ObjectToolbox.hpp"
#endif
#include <fmt/format.h>

#define ARM_NOP {0x00, 0xbf}
#define ARM_FLOAT_INF {0x00, 0x00, 0x80, 0x7f}
#define ARM_FLOAT_MINUS_INF {0x00, 0x00, 0x80, 0xff}
#define ARM_BNE {0x00, 0xd1}
    
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

float getCurrentPercentageF(PlayLayer* playLayer) {
    PlayerObject* player = getPlayer(playLayer);
    float percent = (player->getPositionX() / MEMBER_BY_OFFSET(float, playLayer, PlayLayer__m_lastX)) * 100.0; // from destroyPlayer
    if (percent < 0) return 0;
    if (percent > 100) return 100;
    return percent;
}
float getCurrentPercentageF() {
    return getCurrentPercentageF(getPlayLayer());
}
int getCurrentPercentage(PlayLayer* playLayer) {
    return floorf(getCurrentPercentageF(playLayer));
}
int getCurrentPercentage() {
    return getCurrentPercentage(getPlayLayer());
}

GJGameLevel* getInfoLayerLevel(LevelInfoLayer* infoLayer) {
    return MEMBER_BY_OFFSET(GJGameLevel*, infoLayer, LevelInfoLayer__m_level);
}

template <typename T>
std::vector<uint8_t> toBytesLE(T value) {
    std::vector<uint8_t> bytes(sizeof(T));
    for (size_t i = 0; i < sizeof(T); ++i)
        bytes[i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
    return bytes;
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
cocos2d::CCPoint getCheckpointPosition(CCNode* checkpoint) {
    return MEMBER_BY_OFFSET(cocos2d::CCPoint, checkpoint, CheckpointObject__m_playerPos);
}
cocos2d::CCPoint getStartPos(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(cocos2d::CCPoint, playLayer, PlayLayer__m_startPos);
}
cocos2d::CCPoint getStartPos() {
    return getStartPos(getPlayLayer());
}
CCLayer* getSelectLayerScroll(CCLayer* selectLayer) {
    return MEMBER_BY_OFFSET(CCLayer*, selectLayer, LevelSelectLayer__m_scrollLayer);
}
int getCurrentScrollScreen(CCLayer* scrollLayer) {
    return MEMBER_BY_OFFSET(int, scrollLayer, BoomScrollLayer__m_currentScreen);
}
int getScrollPageCount(CCLayer* scrollLayer) {
    bool dynamic = MEMBER_BY_OFFSET(bool, scrollLayer, BoomScrollLayer__m_dynamic);
    if (dynamic) {
        CCArray* dynamicPages = MEMBER_BY_OFFSET(CCArray*, scrollLayer, BoomScrollLayer__m_dynamicPages);
        return dynamicPages->count();
    } else {
        CCArray* pages = MEMBER_BY_OFFSET(CCArray*, scrollLayer, BoomScrollLayer__m_pages);
        return pages->count();
    }
}
int getCurrentScrollIndex(CCLayer* scrollLayer) {
    int pageCount = getScrollPageCount(scrollLayer);
    int screen = getCurrentScrollScreen(scrollLayer);
    return ((screen % pageCount) + pageCount) % pageCount;
}
std::string getPlayerName() {
    GameManager* gman = GameManager::sharedState();
    return MEMBER_BY_OFFSET(std::string, gman, GameManager__m_playerName);
}
void setObjectLimit(int limit) {
#if GAME_VERSION < GV_1_5
    DobbyCodePatch(
        reinterpret_cast<void*>(get_address(object_limit)),
        toBytesLE(limit).data(), 4
    );
#else
    if (limit > OBJECT_LIMIT) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(object_limit)),
            std::vector<uint8_t>({0xe0}).data(), 1
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(object_limit_duplicate)),
            std::vector<uint8_t>({0xe7}).data(), 1
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(object_limit)),
#if GAME_VERSION == GV_1_5 || GAME_VERSION == GV_1_7
            std::vector<uint8_t>({0xdd}).data(), 1
#elif GAME_VERSION == GV_1_6
            std::vector<uint8_t>({0xdb}).data(), 1
#endif
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(object_limit_duplicate)),
#if GAME_VERSION == GV_1_5 || GAME_VERSION == GV_1_7
            std::vector<uint8_t>({0xdd}).data(), 1
#elif GAME_VERSION == GV_1_6
            std::vector<uint8_t>({0xdb}).data(), 1
#endif
        );
    }
#endif
}
void setFreeBuild(bool enable) {
    if (!enable) {
#if GAME_VERSION >= GV_1_6
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(move_x_min)),
            std::vector<uint8_t>({0x00, 0x80, 0x96, 0x43}).data(), 4 // 301
        );
#endif
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(move_x_max)),
            std::vector<uint8_t>({0x00, 0x60, 0xEA, 0x46}).data(), 4 // 30000
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(move_y_min)),
            std::vector<uint8_t>({0x00, 0x00, 0xB6, 0x42}).data(), 4 // 91
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(move_y_max)),
            std::vector<uint8_t>({0x00, 0x40, 0xA1, 0x44}).data(), 4 // 1290
        );

        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(place_x_max)),
            std::vector<uint8_t>({0x00, 0x42, 0xEA, 0x46}).data(), 4 // 29985
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(place_x_min)),
            std::vector<uint8_t>({0x00, 0x80, 0x9D, 0x43}).data(), 4 // 315
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(place_y_max)),
            std::vector<uint8_t>({0x00, 0x60, 0x9F, 0x44}).data(), 4 // 1275
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(place_y_min)),
            std::vector<uint8_t>({0x00, 0x00, 0xD2, 0x42}).data(), 4 // 105
        );
    } else {
#if GAME_VERSION >= GV_1_6
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(move_x_min)),
            std::vector<uint8_t>({0x00, 0x00, 0x80, 0x3f}).data(), 4 // 1
        );
#endif
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(move_x_max)),
            std::vector<uint8_t>(ARM_FLOAT_INF).data(), 4
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(move_y_min)),
            std::vector<uint8_t>(ARM_FLOAT_MINUS_INF).data(), 4
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(move_y_max)),
            std::vector<uint8_t>(ARM_FLOAT_INF).data(), 4
        );

        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(place_x_max)),
            std::vector<uint8_t>(ARM_FLOAT_INF).data(), 4
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(place_x_min)),
            std::vector<uint8_t>({0x00, 0x00, 0x80, 0x3F}).data(), 4 // 1
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(place_y_max)),
            std::vector<uint8_t>(ARM_FLOAT_INF).data(), 4
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(place_y_min)),
            std::vector<uint8_t>(ARM_FLOAT_MINUS_INF).data(), 4
        );
    }
}
#if GAME_VERSION < GV_1_5
void setZoomBypass(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_max_1)),
            std::vector<uint8_t>(ARM_NOP).data(), 2
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_max_2)),
            std::vector<uint8_t>(ARM_NOP).data(), 2
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_min)),
            std::vector<uint8_t>(ARM_NOP).data(), 2
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_max_1)),
            std::vector<uint8_t>(ARM_BNE).data(), 2
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_max_2)),
#if GAME_VERSION < GV_1_4
            std::vector<uint8_t>({0x63, 0xe7}).data(),
#else
            std::vector<uint8_t>({0x5e, 0xe7}).data(),
#endif
            2 // B
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_min)),
            std::vector<uint8_t>(ARM_BNE).data(), 2
        );
    }
}
#else
void setZoomBypass(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_max)),
            std::vector<uint8_t>({0x00, 0xBF, 0x00, 0xBF}).data(), 4
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_min)),
            std::vector<uint8_t>({0x00, 0xBF, 0x00, 0xBF}).data(), 4
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_max)),
            std::vector<uint8_t>({0xB0, 0xEE, 0x67, 0x8A}).data(), 4
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(zoom_bypass_min)),
            std::vector<uint8_t>({0xB0, 0xEE, 0x67, 0x8A}).data(), 4
        );
    }
}
#endif
void setEditButton(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(pause_edit_button)),
            std::vector<uint8_t>(ARM_NOP).data(), 2
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(pause_edit_button)),
#if GAME_VERSION < GV_1_5
            std::vector<uint8_t>(ARM_BNE).data(), 2
#else
            std::vector<uint8_t>({0x34, 0xd1}).data(), 2
#endif
        );
    }
}
void setRestartButton(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(pause_restart_button)),
            std::vector<uint8_t>(ARM_NOP).data(), 2
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(pause_restart_button)),
#if GAME_VERSION < GV_1_5
            std::vector<uint8_t>(ARM_BNE).data(), 2
#else
            std::vector<uint8_t>({0x12, 0xd1}).data(), 2
#endif
        );
    }
}
cocos2d::CCLayer* getEditorGameLayer(LevelEditorLayer* editorLayer) {
    return MEMBER_BY_OFFSET(cocos2d::CCLayer*, editorLayer, LevelEditorLayer__m_gameLayer);
}
LevelEditorLayer* getUIEditorLayer(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(LevelEditorLayer*, uiLayer, EditorUI__m_editorLayer);
}
GJGameLevel* getCellLevel(CCNode* cell) {
    return MEMBER_BY_OFFSET(GJGameLevel*, cell, LevelCell__m_level);
}
#if GAME_VERSION > GV_1_0
int getCommentID(CCNode* comment) {
    return MEMBER_BY_OFFSET(int, comment, GJComment__m_commentID);
}
#endif
GJGameLevel* getEditLayerLevel(CCLayer* editLayer) {
    return MEMBER_BY_OFFSET(GJGameLevel*, editLayer, EditLevelLayer__m_level);
}

CCParticleSystem* getShipLiftParticles(PlayerObject* player) {
    return MEMBER_BY_OFFSET(CCParticleSystem*, player, PlayerObject__m_pShipLift);
}
CCParticleSystem* getShipGroundParticles(PlayerObject* player) {
    return MEMBER_BY_OFFSET(CCParticleSystem*, player, PlayerObject__m_pShipGround);
}
CCParticleSystem* getGroundParticles(PlayerObject* player) {
    return MEMBER_BY_OFFSET(CCParticleSystem*, player, PlayerObject__m_pGround);
}
CCParticleSystem* getLandingParticles(PlayerObject* player) {
    return MEMBER_BY_OFFSET(CCParticleSystem*, player, PlayerObject__m_pLanding);
}
CCParticleSystem* getLandingParticles2(PlayerObject* player) {
    return MEMBER_BY_OFFSET(CCParticleSystem*, player, PlayerObject__m_pLanding2);
}
CCParticleSystem* getShipFireParticles(PlayerObject* player) {
    return MEMBER_BY_OFFSET(CCParticleSystem*, player, PlayerObject__m_pShipFire);
}
CCParticleSystem* getBGParticles(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(CCParticleSystem*, playLayer, PlayLayer__m_pGlitter);
}
CCParticleSystem* getObjectParticles(void* object) {
    return MEMBER_BY_OFFSET(CCParticleSystem*, object, GameObject__m_particles);
}
#if GAME_VERSION > GV_1_0
CCArray* getLocalLevels(LocalLevelManager* lolman) {
    return MEMBER_BY_OFFSET(CCArray*, lolman, LocalLevelManager__m_localLevels);
}
CCArray* getLocalLevels() {
    return getLocalLevels(LocalLevelManager::sharedState());
}
#else
CCArray* getLocalLevels(GameLevelManager* glman) {
    return MEMBER_BY_OFFSET(CCArray*, glman, GameLevelManager__m_localLevels);
}
CCArray* getLocalLevels() {
    return getLocalLevels(GameLevelManager::sharedState());
}
#endif
bool getObjectUseAudioScale(void* object) {
    return MEMBER_BY_OFFSET(bool, object, GameObject__m_useAudioScale);
}
void setObjectUseAudioScale(void* object, bool uas) {
    MEMBER_BY_OFFSET(bool, object, GameObject__m_useAudioScale) = uas;
}
CCArray* getSelectedObjects(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(CCArray*, uiLayer, EditorUI__m_selectedObjects);
}
void setSelectedObjects(EditorUI* uiLayer, CCArray* arr) {
    MEMBER_BY_OFFSET(CCArray*, uiLayer, EditorUI__m_selectedObjects) = arr;
}
DrawGridLayer* getGridLayer(LevelEditorLayer* editLayer) {
    return MEMBER_BY_OFFSET(DrawGridLayer*, editLayer, LevelEditorLayer__m_gridLayer);
}
int getObjectType(GameObject* object) {
    return MEMBER_BY_OFFSET(int, object, GameObject__m_type);
}
GameObject* getSelectedObject(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(GameObject*, uiLayer, EditorUI__m_selectedObject);
}
CCArray* getRedoArray(LevelEditorLayer* editLayer) {
    return MEMBER_BY_OFFSET(CCArray*, editLayer, LevelEditorLayer__m_redoArray);
}
CCSpriteBatchNode* getEditorBatchNode(LevelEditorLayer* editLayer) {
    return MEMBER_BY_OFFSET(CCSpriteBatchNode*, editLayer, LevelEditorLayer__m_batchNode);
}
bool getShouldSpawn(GameObject* object) {
    return MEMBER_BY_OFFSET(bool, object, GameObject__m_shouldSpawn);
}
CCMenu* getEditorUIButtonMenu(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(CCMenu*, uiLayer, EditorUI__m_buttonMenu);
}
bool getOnGround(PlayerObject* player) {
    return MEMBER_BY_OFFSET(bool, player, PlayerObject__m_onGround);
}
void setOnGround(PlayerObject* player, bool onGround) {
    MEMBER_BY_OFFSET(bool, player, PlayerObject__m_onGround) = onGround;
}
EditButtonBar* getCreateButtonBar(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(EditButtonBar*, uiLayer, EditorUI__m_createButtonBar);
}
void setCreateButtonBar(EditorUI* uiLayer, EditButtonBar* bar) {
    MEMBER_BY_OFFSET(EditButtonBar*, uiLayer, EditorUI__m_createButtonBar) = bar;
}
CCArray* getCreateButtons(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(CCArray*, uiLayer, EditorUI__m_createButtons);
}
void setCreateButtons(EditorUI* uiLayer, CCArray* array) {
    MEMBER_BY_OFFSET(CCArray*, uiLayer, EditorUI__m_createButtons) = array;
}
#if GAME_VERSION >= GV_1_7
CCArray* getCreateButtonBars(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(CCArray*, uiLayer, EditorUI__m_createButtonBars);
}
void setCreateButtonBars(EditorUI* uiLayer, CCArray* bars) {
    MEMBER_BY_OFFSET(CCArray*, uiLayer, EditorUI__m_createButtonBars) = bars;
}
#endif
float getScreenBottom() {
    CCDirector* director = CCDirector::sharedDirector();
#if GAME_VERSION < GV_1_7
    return MEMBER_BY_OFFSET(float, director->getOpenGLView(), CCEGLViewProtocol__m_screenBottom);
#else
    return director->getScreenBottom();
#endif
}
float getUnkFloat(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(float, uiLayer, EditorUI__m_unkFloat);
}
CCArray* getEditorSections(LevelEditorLayer* editLayer) {
    return MEMBER_BY_OFFSET(CCArray*, editLayer, LevelEditorLayer__m_sections);
}
int getObjectKey(GameObject* obj) {
    return MEMBER_BY_OFFSET(int, obj, GameObject__m_objectKey);
}
int getCurrentAttempts(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(int, playLayer, PlayLayer__m_attempts);
}
int getCurrentAttempts() {
    return getCurrentAttempts(getPlayLayer());
}
int getCurrentJumps(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(int, playLayer, PlayLayer__m_jumps);
}
int getCurrentJumps() {
    return getCurrentJumps(getPlayLayer());
}
float getClkTimer(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(float, playLayer, PlayLayer__m_clkTimer);
}
float getClkTimer() {
    return getClkTimer(getPlayLayer());
}
std::string getPlayerUDID() {
    return MEMBER_BY_OFFSET(std::string, GameManager::sharedState(), GameManager__m_playerUDID);
}
CCLabelBMFont* getAttemptLabel(PlayLayer* layer) {
    return MEMBER_BY_OFFSET(CCLabelBMFont*, layer, PlayLayer__m_attemptLabel);
}
CCLabelBMFont* getAttemptLabel() {
    return getAttemptLabel(getPlayLayer());
}

CCArray* getPlaySections(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(CCArray*, playLayer, PlayLayer__m_sections);
}
CCArray* getPlaySections() {
    return getPlaySections(getPlayLayer());
}

bool compareXes(CCObject* p1, CCObject* p2) {
  return getRealPosition(static_cast<GameObject*>(p1)).x < getRealPosition(static_cast<GameObject*>(p2)).x;
}
bool compareXes2(CCObject* p1, CCObject* p2) {
  return static_cast<GameObject*>(p1)->getPositionX() < static_cast<GameObject*>(p2)->getPositionX();
}

CCArray* getStartPositions_(PlayLayer* playLayer) {
    auto sections = getPlaySections(playLayer);
    CCArray* arr = CCArray::create();
    for (int i = 0; i < sections->count(); i++) {
        auto currSec = static_cast<CCArray*>(sections->objectAtIndex(i));
        for (int j = 0; j < currSec->count(); j++) {
            auto currObj = static_cast<GameObject*>(currSec->objectAtIndex(j));
            if (getObjectKey(currObj) == 31) {
                arr->addObject(currObj);
            }
        }
    }
    std::sort(arr->data->arr, arr->data->arr + arr->data->num, compareXes);
    return arr;
}
CCArray* getStartPositions_() {
    return getStartPositions_(getPlayLayer());
}

void setStartPos(PlayLayer* playLayer, cocos2d::CCPoint point) {
    MEMBER_BY_OFFSET(cocos2d::CCPoint, playLayer, PlayLayer__m_startPos) = point;
}
void setStartPos(cocos2d::CCPoint point) {
    setStartPos(getPlayLayer(), point);
}

CCPoint getRealPosition(GameObject* object) {
    return MEMBER_BY_OFFSET(CCPoint, object, GameObject__m_realPosition);
}

TextInputDelegate* getTextInputDelegate(CCTextInputNode* node) {
    return MEMBER_BY_OFFSET(TextInputDelegate*, node, CCTextInputNode__m_inputDelegate);
}
void setTextInputDelegate(CCTextInputNode* node, TextInputDelegate* delegate) {
    MEMBER_BY_OFFSET(TextInputDelegate*, node, CCTextInputNode__m_inputDelegate) = delegate;
}

cocos2d::extension::CCControlColourPicker* getColorPicker(ColorSelectPopup* popup) {
    return MEMBER_BY_OFFSET(cocos2d::extension::CCControlColourPicker*, popup, ColorSelectPopup__m_colorWheel);
}
cocos2d::extension::CCControlColourPicker* getColorPicker(ColorPickerPopup* popup) {
    return MEMBER_BY_OFFSET(cocos2d::extension::CCControlColourPicker*, popup, ColorPickerPopup__m_colorWheel);
}

int getCharLimit(CCTextInputNode* node) {
    return MEMBER_BY_OFFSET(int, node, CCTextInputNode__m_charLimit);
}
void setCharLimit(CCTextInputNode* node, int limit) {
    MEMBER_BY_OFFSET(int, node, CCTextInputNode__m_charLimit) = limit;
}

GJSearchObject* getSearchObject(LevelBrowserLayer* browser) {
    return MEMBER_BY_OFFSET(GJSearchObject*, browser, LevelBrowserLayer__m_searchObject);
}
int getSearchType(GJSearchObject* searcher) {
    return MEMBER_BY_OFFSET(int, searcher, GJSearchObject__m_type);
}
int& getSearchPage(GJSearchObject* searcher) {
    return MEMBER_BY_OFFSET(int, searcher, GJSearchObject__m_page);
}
#if GAME_VERSION >= GV_1_4
void setIconHack(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__GameManager_isIconUnlocked)),
            std::vector<uint8_t>({0x01, 0x20, 0x70, 0x47}).data(), 4
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__GameManager_isColorUnlocked)),
            std::vector<uint8_t>({0x01, 0x20, 0x70, 0x47}).data(), 4
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__GameManager_isIconUnlocked)),
#if GAME_VERSION == GV_1_4
            std::vector<uint8_t>({0x30, 0xb5, 0x85, 0xb0}).data(), 4
#elif GAME_VERSION == GV_1_5
            std::vector<uint8_t>({0x13, 0xb5, 0x12, 0xb9}).data(), 4
#elif GAME_VERSION >= GV_1_6
            std::vector<uint8_t>({0x13, 0xb5, 0x0a, 0xb9}).data(), 4
#endif
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__GameManager_isColorUnlocked)),
#if GAME_VERSION == GV_1_4
            std::vector<uint8_t>({0x30, 0xb5, 0x85, 0xb0}).data(), 4
#elif GAME_VERSION >= GV_1_5
            std::vector<uint8_t>({0x13, 0xb5, 0x01, 0x24}).data(), 4
#endif
        );
    }
}
#endif
EditButtonBar* getEditButtonBar(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(EditButtonBar*, uiLayer, EditorUI__m_editButtonBar);
}
void setEditButtonBar(EditorUI* uiLayer, EditButtonBar* bar) {
    MEMBER_BY_OFFSET(EditButtonBar*, uiLayer, EditorUI__m_editButtonBar) = bar;
}
#if GAME_VERSION < GV_1_7
CCArray* getBarButtons(EditButtonBar* bar) {
    return MEMBER_BY_OFFSET(CCArray*, bar, EditButtonBar__m_buttons);
}
#else
CCArray* getBarPages(EditButtonBar* bar) {
    return MEMBER_BY_OFFSET(CCArray*, bar, EditButtonBar__m_pages);
}
#endif
void setOriginalScale(CCMenuItemSpriteExtra* btn, float scale) {
    MEMBER_BY_OFFSET(float, btn, CCMenuItemSpriteExtra__m_ogScale) = scale;
}
#if GAME_VERSION < GV_1_4
void setEditObjectButton(EditorUI* self, CCMenuItemSpriteExtra* btn) {
    MEMBER_BY_OFFSET(CCMenuItemSpriteExtra*, self, EditorUI__m_editObjectButton) = btn;
}
#endif

int getGlobalOrderOfArrival() {
    return *reinterpret_cast<int*>(get_address(global_order_of_arrival));
}

LevelSettingsObject* getEditorSettingsObject(LevelEditorLayer* lel) {
    return MEMBER_BY_OFFSET(LevelSettingsObject*, lel, LevelEditorLayer__m_settings);
}
void setEditorSettingsObject(LevelEditorLayer* lel, LevelSettingsObject* settings) {
    MEMBER_BY_OFFSET(LevelSettingsObject*, lel, LevelEditorLayer__m_settings) = settings;
}
int getAudioTrack(LevelSettingsObject* obj) {
    return MEMBER_BY_OFFSET(int, obj, LevelSettingsObject__m_audioTrack);
}

#if GAME_VERSION < GV_1_2
std::string getAllowedChars(CCTextInputNode* input) {
    return MEMBER_BY_OFFSET(std::string, input, CCTextInputNode__m_allowedChars);
}
#endif

#if GAME_VERSION >= GV_1_5
void setCharLimitBypass(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__CCTextInputNode_setCharLimit)),
            std::vector<uint8_t>({0x70, 0x47}).data(), 2
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__CCTextInputNode_setCharLimit)),
            std::vector<uint8_t>({0xc0, 0xf8}).data(), 2
        );
    }
}
void setProfanityBypass(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__CCTextInputNode_setProfanity)),
            std::vector<uint8_t>({0x70, 0x47}).data(), 2
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__CCTextInputNode_setProfanity)),
            std::vector<uint8_t>({0x80, 0xf8}).data(), 2
        );
    }
}
void setBlockVerify(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__GJGameLevel_setIsVerified)),
            std::vector<uint8_t>({0x00, 0x20, 0x70, 0x47}).data(), 2
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(__GJGameLevel_setIsVerified)),
#if GAME_VERSION == GV_1_5
            std::vector<uint8_t>({0x80, 0xf8, 0x59, 0x11}).data(), 2
#elif GAME_VERSION == GV_1_6
            std::vector<uint8_t>({0x80, 0xf8, 0x5d, 0x11}).data(), 2
#elif GAME_VERSION == GV_1_7
            std::vector<uint8_t>({0x80, 0xf8, 0x29, 0x11}).data(), 2
#endif
        );
    }
}
std::vector<uint8_t> uintptrToBytes(uintptr_t value) {
    std::vector<uint8_t> bytes(sizeof(uintptr_t));

    for (size_t i = 0; i < bytes.size(); ++i) {
        // Little-endian: extract least significant byte first
        bytes[i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
    }

    return bytes;
}
bool getShowProgressBar() {
    GameManager* gman = GameManager::sharedState();
    return MEMBER_BY_OFFSET(bool, gman, GameManager__m_showProgressBar);
}
CCSprite* getProgressBar(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(CCSprite*, playLayer, PlayLayer__m_progressBar);
}
CCSprite* getProgressBar() {
    return getProgressBar(getPlayLayer());
}
CCMenuItemSpriteExtra* getZoomOutButton(EditorUI* uiLayer) {
    return MEMBER_BY_OFFSET(CCMenuItemSpriteExtra*, uiLayer, EditorUI__m_zoomOutButton);
}
#endif
#if GAME_VERSION >= GV_1_6
void setClubstepBypass(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(clubstep_bypass)),
            std::vector<uint8_t>({0xe0}).data(), 1
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(clubstep_bypass)),
            std::vector<uint8_t>({0xdd}).data(), 1
        );
    }
}
#endif
const char* intToReadableGV(int gv) {
    switch (gv) {
        case 1:
            return "1.0";
        case 2:
            return "1.1";
        case 3:
            return "1.2";
        case 4:
            return "1.3";
        case 5:
            return "1.4";
        case 6:
            return "1.5";
        case 7:
            return "1.6";
        case 10:
            return "1.7";
        case 11:
            return "1.80";
        case 18:
            return "1.81";
        case 19:
            return "1.9";
        case 20:
            return "2.0";
        case 21:
            return "2.1";
        case 22:
            return "2.2";
        default:
            return "Unknown";
    }
}
#if GAME_VERSION < GV_1_4
void setNoShipTint(bool enable) {
    if (enable) {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(ship_tint)),
            std::vector<uint8_t>({0x00, 0x00, 0x80, 0x3f}).data(), 4
        );
    } else {
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(ship_tint)),
            std::vector<uint8_t>({0xcd, 0xcc, 0x4c, 0x3f}).data(), 4
        );
    }
}
#endif

int getSectionIdx(GameObject* obj) {
    return MEMBER_BY_OFFSET(int, obj, GameObject__m_sectionIdx);
}
#if GAME_VERSION < GV_1_1
CCDictionary* getKeyTimers() {
    return MEMBER_BY_OFFSET(CCDictionary*, GameLevelManager::sharedState(), GameLevelManager__m_keyTimers);
}
#else
bool getAutoRetry() {
    return MEMBER_BY_OFFSET(bool, GameManager::sharedState(), GameManager__m_autoRetry);
}
#if GAME_VERSION < GV_1_6
void setShouldRunDelayedReset(PlayLayer* playLayer, bool value) {
    MEMBER_BY_OFFSET(bool, playLayer, PlayLayer__m_shouldRunDelayReset) = value;
}
#endif
#endif

GJGameLevel* getLELLevel(LevelEditorLayer* lel) {
    return MEMBER_BY_OFFSET(GJGameLevel*, lel, LevelEditorLayer__m_level);
}
#if GAME_VERSION >= GV_1_7
CCArray* getSpeedObjects(DrawGridLayer* gridLayer) {
    return MEMBER_BY_OFFSET(CCArray*, gridLayer, DrawGridLayer__m_speedObjects);
}
int getStartSpeed(LevelSettingsObject* settings) {
    return MEMBER_BY_OFFSET(int, settings, LevelSettingsObject__m_startSpeed);
}
// float getLevelDistance(LevelEditorLayer* lel) {
//     return MEMBER_BY_OFFSET(float, lel, LevelEditorLayer__m_distance);
// }
#endif
CCMenu* getCheckpointMenu(UILayer* self) {
    return MEMBER_BY_OFFSET(CCMenu*, self, UILayer__m_checkpointMenu);
}
#if GAME_VERSION >= GV_1_3
LeaderboardState getLeaderboardState(LeaderboardsLayer* self) {
    return MEMBER_BY_OFFSET(LeaderboardState, self, LeaderboardsLayer__m_boardType);
}
void setLeaderboardState(LeaderboardsLayer* self, int value) {
    MEMBER_BY_OFFSET(int, self, LeaderboardsLayer__m_boardType) = value;
}
#endif
CCPoint getCameraPos(PlayLayer* self) {
    return MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_cameraPos);
}
CCPoint getCameraPos() {
    return getCameraPos(getPlayLayer());
}
#if GAME_VERSION >= GV_1_4
float getRadius(GameObject* obj) {
    return MEMBER_BY_OFFSET(float, obj, GameObject__m_radius);
}
#endif
bool getIsFlipped(PlayLayer* layer) {
    return MEMBER_BY_OFFSET(bool, layer, PlayLayer__m_isFlipped);
}
bool getIsFlipped() {
    return getIsFlipped(getPlayLayer());
}
float getDuration(ColorSelectPopup* popup) {
    return MEMBER_BY_OFFSET(float, popup, ColorSelectPopup__m_duration);
}
void setDuration(ColorSelectPopup* popup, float value) {
    MEMBER_BY_OFFSET(float, popup, ColorSelectPopup__m_duration) = value;
}
CCLabelBMFont* getDurationLabel(ColorSelectPopup* popup) {
    return MEMBER_BY_OFFSET(CCLabelBMFont*, popup, ColorSelectPopup__m_durLabel);
}
Slider* getCSPSlider(ColorSelectPopup* popup) {
    return MEMBER_BY_OFFSET(Slider*, popup, ColorSelectPopup__m_slider);
}
void setDecimals(char val) {
    DobbyCodePatch(
        reinterpret_cast<void*>(get_address(_0_1f)),
        std::vector<uint8_t>({val}).data(), 1
    );
}
#if GAME_VERSION == GV_1_4
bool getFlyMode(PlayerObject* player) {
    return MEMBER_BY_OFFSET(bool, player, PlayerObject__m_flyMode);
}
bool getGravityFlipped(PlayerObject* player) {
    return MEMBER_BY_OFFSET(bool, player, PlayerObject__m_gravityFlipped);
}
float getPlayerScale(PlayerObject* player) {
    return MEMBER_BY_OFFSET(float, player, PlayerObject__m_playerScale);
}
#endif
bool getCheckpointFlipped(CCNode* checkpoint) {
    return MEMBER_BY_OFFSET(bool, checkpoint, CheckpointObject__m_isFlipped);
}

#if GDPS == GDPS_NEOPOINTFOUR
CCDictionary* getSettingsDict(GameLevelManager* glman) {
    return MEMBER_BY_OFFSET(CCDictionary*, glman, GameLevelManager__m_settingsDict);
}
CCDictionary* getCompletedDict(GameStatsManager* gsman) {
    return MEMBER_BY_OFFSET(CCDictionary*, gsman, GameStatsManager__m_completedDict);
}
CCDictionary* getStatsDict(GameStatsManager* gsman) {
    return MEMBER_BY_OFFSET(CCDictionary*, gsman, GameStatsManager__m_statsDict);
}
int getPlayerShip(GameManager* gm) {
    return MEMBER_BY_OFFSET(int, gm, GameManager__m_playerShip);
}
CCLayer* getMainLayer(CCLayer* infoLayer){
    return MEMBER_BY_OFFSET(CCLayer*, infoLayer, InfoLayer__m_mainLayer);
}
#endif
int getTotalLevels(LevelBrowserLayer* browser) {
    return MEMBER_BY_OFFSET(int, browser, LevelBrowserLayer__m_total);
}

const char* keyToFrame(int objectID) {
#if GAME_VERSION > GV_1_2
    return ObjectToolbox::sharedState()->keyToFrame(fmt::format("{}", objectID).c_str());
#else
    switch (objectID) { // copied from GameObject::keyToFrame (1.2)
        case 1:
            return "square_01_001.png";
        case 2:
            return "square_02_001.png";
        case 3:
            return "square_03_001.png";
        case 4:
            return "square_04_001.png";
        case 5:
            return "square_05_001.png";
        case 6:
            return "square_06_001.png";
        case 7:
            return "square_07_001.png";
        case 8:
            return "spike_01_001.png";
        case 9:
            return "pit_01_001.png";
        case 0xA:
            return "portal_01_front_001.png";
        case 0xB:
            return "portal_02_front_001.png";
        case 0xC:
            return "portal_03_front_001.png";
        case 0xD:
            return "portal_04_front_001.png";
        case 0xF:
            return "rod_01_001.png";
        case 0x10:
            return "rod_02_001.png";
        case 0x11:
            return "rod_03_001.png";
        case 0x12:
            return "d_spikes_01_001.png";
        case 0x13:
            return "d_spikes_02_001.png";
        case 0x14:
            return "d_spikes_03_001.png";
        case 0x15:
            return "d_spikes_04_001.png";
        case 0x16:
            return "edit_eeNoneBtn_001.png";
        case 0x17:
            return "edit_eeFBBtn_001.png";
        case 0x18:
            return "edit_eeFTBtn_001.png";
        case 0x19:
            return "edit_eeFLBtn_001.png";
        case 0x1A:
            return "edit_eeFRBtn_001.png";
        case 0x1B:
            return "edit_eeSUBtn_001.png";
        case 0x1C:
            return "edit_eeSDBtn_001.png";
        case 0x1D:
            return "edit_eTintBGBtn_001.png";
        case 0x1E:
            return "edit_eTintGBtn_001.png";
        case 0x1F:
            return "edit_eStartPosBtn_001.png";
        case 0x20:
            return "edit_eGhostEBtn_001.png";
        case 0x21:
            return "edit_eGhostDBtn_001.png";
        case 0x22:
            return "edit_eLevelEndBtn_001.png";
        case 0x23:
            return "bump_01_001.png";
        case 0x24:
            return "ring_01_001.png";
        case 0x27:
            return "spike_02_001.png";
        case 0x28:
            return "plank_01_001.png";
        case 0x29:
            return "chain_01_001.png";
        case 0x2A:
            return "edit_eBGEOn_001.png";
        case 0x2B:
            return "edit_eBGEOff_001.png";
        case 0x2D:
            return "portal_05_front_001.png";
        case 0x2E:
            return "portal_06_front_001.png";
        case 0x2F:
            return "portal_07_front_001.png";
        default:
            return "";
    }
#endif
}

CCPoint getTouchLocation(CCTouch* touch) {
#if GAME_VERSION >= GV_1_7
    return touch->getLocation();
#else
    return CCDirector::sharedDirector()->convertToGL(touch->locationInView());
#endif
}

CCPoint getPreviousTouchLocation(CCTouch* touch) {
#if GAME_VERSION >= GV_1_7
    return touch->getPreviousLocation();
#else
    return CCDirector::sharedDirector()->convertToGL(touch->previousLocationInView());
#endif
}