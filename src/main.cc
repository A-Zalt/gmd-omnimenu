#include <algorithm>
#include "../layers/HaxLayer.hpp"
#include "version/VersionUtils.cc"
#include "version/addresses.hpp"

#include "FLAlertLayer.hpp"
#include "HaxManager.hpp"
#include "CCTextInputNode.hpp"
#include "SliderTouchLogic.hpp"
#include "EditorUI.hpp"
#include "LevelEditorLayer.hpp"
#include "GameManager.hpp"
#include "LevelInfoLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "GameLevelManager.hpp"
#include "ButtonSprite.hpp"
#include "UILayer.hpp"
#include "MenuLayer.hpp"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "cocos2d/CocosDenshion/android/jni/SimpleAudioEngineJni.h"

std::string itoa(int value) {
    static const char digits[] = "0123456789";

    unsigned int uvalue = value < 0 ? -value : value;

    std::string result;
    do {
        result.push_back(digits[uvalue % 10]);
        uvalue /= 10;
    } while (uvalue);

    if (value < 0)
        result.push_back('-');

    std::reverse(result.begin(), result.end());
    return result;
}

void (*TRAM_PlayLayer_destroyPlayer)(PlayLayer* self);
void PlayLayer_destroyPlayer(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.noClip || hax.instantComplete) {
        getPlayLayerHazards()->removeAllObjects(); // the humble noclip lag fix
        return;
    }
    TRAM_PlayLayer_destroyPlayer(self);
    if (hax.practiceMusic && getPlayLayerPractice(self)) {
        auto audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
        audioEngine->pauseBackgroundMusic();
    }
}
bool (*TRAM_GameManager_isColorUnlocked)(void* self, int idx, bool secondary);
bool GameManager_isColorUnlocked(void* self, int idx, bool secondary) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.iconHack) return true;
    else return TRAM_GameManager_isColorUnlocked(self, idx, secondary);
}
bool (*TRAM_GameManager_isIconUnlocked)(void* self, int idx);
bool GameManager_isIconUnlocked(void* self, int idx) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.iconHack) return true;
    else return TRAM_GameManager_isIconUnlocked(self, idx);
}
void MenuLayer_onMoreGames(void* self) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, HaxLayer::scene(false)));
}
void (*TRAM_CCTextInputNode_setCharLimit)(void* self, int charLimit);
void CCTextInputNode_setCharLimit(void* self, int charLimit) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.textLengthBypass) return;
    TRAM_CCTextInputNode_setCharLimit(self, charLimit);
}
void (*TRAM_CCTextInputNode_updateLabel)(CCTextInputNode* self, char* text);
void CCTextInputNode_updateLabel(CCTextInputNode* self, char* text) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.charFilterBypass) {
        self->setAllowedChars(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
    }
    TRAM_CCTextInputNode_updateLabel(self, text);
}
void (*TRAM_CCTextInputNode_setProfanityFilter)(CCTextInputNode* self, bool profanityFilter);
void CCTextInputNode_setProfanityFilter(CCTextInputNode* self, bool profanityFilter) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.swearBypass) return;
    TRAM_CCTextInputNode_setProfanityFilter(self, profanityFilter);
}
// void (*TRAM_SliderTouchLogic_ccTouchMoved)(SliderTouchLogic* self, CCTouch* touch, CCEvent* event);
// void SliderTouchLogic_ccTouchMoved(SliderTouchLogic* self, CCTouch* touch, CCEvent* event) {
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.sliderBypass) {
//         auto touchPos = reinterpret_cast<CCNode*>(self)->convertTouchToNodeSpace(touch);
//         auto touchOffset = self->m_position;
//         auto position = ccp(touchPos.x - touchOffset.x, touchPos.y - touchOffset.y);

//         auto thumb = reinterpret_cast<CCNode*>(self->getThumb());

//         auto clamped = position.x;
//         thumb->setPosition(ccp(clamped, 0.f));

//         // if (self->getLiveDragging())
//         //     thumb->activate();

//         Slider* slider = *reinterpret_cast<Slider**>(reinterpret_cast<uintptr_t>(self) + 0x158);
//         if (slider != nullptr) {
//             slider->updateBar();
//         }
//         // if (Slider* slider = self->getSliderDelegate())
//         //     slider->updateBar();
//     } else TRAM_SliderTouchLogic_ccTouchMoved(self, touch, event);
// }
// *reinterpret_cast<PlayerObject**>(reinterpret_cast<uintptr_t>(self) + 0x22c)
JNIEnv* getEnv() {
    JNIEnv** env;
    if (cocos2d::JniHelper::getJavaVM()->GetEnv((void**)env, JNI_VERSION_1_4) != JNI_OK) {
        cocos2d::CCLog("Failed to get the environment using GetEnv()");
        return nullptr;
    }
    if (cocos2d::JniHelper::getJavaVM()->AttachCurrentThread(env, 0) < 0) {
        cocos2d::CCLog("Failed to get the environment using AttachCurrentThread()");
        return nullptr;
    }

    return *env;
}
void (*TRAM_PlayLayer_resetLevel)(PlayLayer* self);
void PlayLayer_resetLevel(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.practiceMusic && getPlayLayerPractice(self)) {
        auto audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
        int seekTime = 0;
        CCNode* lastCheckpoint = self->getLastCheckpoint();
        if (lastCheckpoint != nullptr) {
            CCPoint lastCheckpointPos = lastCheckpoint->getPosition();
            seekTime = floorf((lastCheckpointPos.x / 311.58f) * 1000.f);
        }
        JNIEnv* env = getEnv();
        if (env) {
            do {
                jclass Cocos2dxActivity = env->FindClass("org/cocos2dx/lib/Cocos2dxActivity");
                if (Cocos2dxActivity == nullptr) {
                    cocos2d::CCLog("Failed to get Cocos2dxActivity");
                    break;
                }
                jfieldID fieldID_backgroundMusicPlayer = env->GetStaticFieldID(Cocos2dxActivity, "backgroundMusicPlayer", "Lorg/cocos2dx/lib/Cocos2dxMusic;");
                if (fieldID_backgroundMusicPlayer == nullptr) {
                    cocos2d::CCLog("Failed to get field ID of backgroundMusicPlayer");
                    break;
                }
                jobject backgroundMusicPlayer = env->GetStaticObjectField(Cocos2dxActivity, fieldID_backgroundMusicPlayer);
                if (backgroundMusicPlayer == nullptr) {
                    cocos2d::CCLog("Failed to get backgroundMusicPlayer");
                    break;
                }

                jclass Cocos2dxMusic = env->FindClass("org/cocos2dx/lib/Cocos2dxMusic");
                if (Cocos2dxMusic == nullptr) {
                    cocos2d::CCLog("Failed to get Cocos2dxMusic");
                    break;
                }
                jfieldID fieldID_mBackgroundMediaPlayer = env->GetFieldID(Cocos2dxMusic, "mBackgroundMediaPlayer", "Landroid/media/MediaPlayer;");
                if (fieldID_mBackgroundMediaPlayer == nullptr) {
                    cocos2d::CCLog("Failed to get field ID of mBackgroundMediaPlayer");
                    break;
                }
                jobject mBackgroundMediaPlayer = env->GetObjectField(backgroundMusicPlayer, fieldID_mBackgroundMediaPlayer);
                if (mBackgroundMediaPlayer == nullptr) {
                    cocos2d::CCLog("Failed to get mBackgroundMediaPlayer");
                    break;
                }

                jclass MediaPlayer = env->GetObjectClass(mBackgroundMediaPlayer);
                if (MediaPlayer == nullptr) {
                    cocos2d::CCLog("Failed to get MediaPlayer");
                    break;
                }
                jmethodID seekTo = env->GetMethodID(MediaPlayer, "seekTo", "(I)V");
                if (seekTo == nullptr) {
                    cocos2d::CCLog("Failed to get method ID of seekTo");
                    break;
                }
                env->CallVoidMethod(mBackgroundMediaPlayer, seekTo, static_cast<jint>(seekTime));
            } while (0);
        } else {
            cocos2d::CCLog("Failed to get Java Env");
            audioEngine->setBackgroundMusicTime(static_cast<float>(seekTime) / 1000.f);
            audioEngine->resumeBackgroundMusic();
        }
    }
    TRAM_PlayLayer_resetLevel(self);
    if (hax.instantComplete) {
        PlayerObject* player = getPlayer(self); // PlayLayer::getPlayer
        player->lockPlayer();
        self->levelComplete();
    }
}
void (*TRAM_PauseLayer_customSetup)(CCLayer* self);
void PauseLayer_customSetup(CCLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.levelEdit) {
        GJGameLevel* level = getPlayLayerLevel();
        GJLevelType type = getLevelType(level); // GJGameLevel::getLevelType
        setLevelType(level, GJLevelType::Editor);
        TRAM_PauseLayer_customSetup(self); 
        setLevelType(level, type);
    } else {
        TRAM_PauseLayer_customSetup(self); 
    }
}
bool (*TRAM_EditLevelLayer_init)(void* self, GJGameLevel* level);
bool EditLevelLayer_init(void* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.verifyHack) {
        setLevelVerified(level, true);
    }
    return TRAM_EditLevelLayer_init(self, level);
}
void (*TRAM_CCTransitionFade_create)(float duration, CCScene* scene, const ccColor3B& color);
void CCTransitionFade_create(float duration, CCScene* scene, const ccColor3B& color = ccBLACK) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.fastMenu) TRAM_CCTransitionFade_create(0.f, scene, color);
    else TRAM_CCTransitionFade_create(duration, scene, color);
}
void (*TRAM_PlayLayer_toggleFlipped)(void* self, bool p1, bool p2);
void PlayLayer_toggleFlipped(void* self, bool p1, bool p2) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.noMirror) return;
    TRAM_PlayLayer_toggleFlipped(self, p1, p2);
}
void (*TRAM_EditorUI_showMaxError)(void* self);
void EditorUI_showMaxError(void* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.objectLimitHack) {
        FLAlertLayer::create(
            nullptr,
            "Max Objects",
            CCString::createWithFormat("You cannot create more than <cy>%i</c> <cg>objects</c> in a single level.", INCREASED_OBJECT_LIMIT)->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
        return;
    }
    TRAM_EditorUI_showMaxError(self);
}
// void (*TRAM_EditorUI_onCreate)(EditorUI* self);
// void EditorUI_onCreate(EditorUI* self) {
//     TRAM_EditorUI_onCreate(self);
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.objectLimitHack) {
//         self->onCreateObject(MEMBER_BY_OFFSET(const char*, self, 0x1B8));
//         // LevelEditorLayer* layer = *reinterpret_cast<LevelEditorLayer**>(reinterpret_cast<uintptr_t>(self) + 0x71);
//         // if (layer->getObjectCount() > 3999) {
//         //     if (layer->getObjectCount() < 16384) {
//             // } else {
//             //     FLAlertLayer::create(
//             //         nullptr,
//             //         "Max Objects",
//             //         "You cannot create more than <cy>16384</c> <cg>objects</c> in a single level.",
//             //         "OK",
//             //         nullptr,
//             //         300.f
//             //     )->show();
//             // }
//         //}
//     }
// }
// DobbyCodePatch(addr, patch, (uint32_t)patch_size);
void (*TRAM_EditorUI_constrainGameLayerPosition)(void* self);
void EditorUI_constrainGameLayerPosition(void* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.freeScroll) return;
    TRAM_EditorUI_constrainGameLayerPosition(self);
}
void LevelInfoLayer::onViewLevelInfo() {
    GJGameLevel* level = getInfoLayerLevel(this);
    CCString* flAlertInsides = CCString::createWithFormat(
        "<cy>%s</c> by <cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cr>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cl>Level ID</c>: %i\n<cb>User ID</c>: %i",
        getLevelName(level).c_str(),
        getLevelUsername(level).c_str(),
        getLevelAttempts(level),
        getLevelNormalPercent(level),
        getLevelPracticePercent(level),
        getLevelID(level),
        getLevelUserID(level)
    );
    FLAlertLayer::create(
        nullptr,
        "Level Info",
        flAlertInsides->getCString(),
        "OK",
        nullptr,
        300.f
    )->show();
}
bool (*TRAM_LevelInfoLayer_init)(LevelInfoLayer* self, GJGameLevel* level);
bool LevelInfoLayer_init(LevelInfoLayer* self, GJGameLevel* level) {
    if (!TRAM_LevelInfoLayer_init(self, level)) return false;
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    if (hax.levelCopying) {
        CCMenu* cloneMenu = CCMenu::create();
        CCSprite* cloneSpr = cocos2d::CCSprite::create("GJ_duplicateBtn.png");
        CCMenuItemSpriteExtra* cloneBtn = CCMenuItemSpriteExtra::create(cloneSpr, cloneSpr, self, menu_selector(LevelInfoLayer::onClone));

        self->addChild(cloneMenu, 1000);
        cloneMenu->addChild(cloneBtn);
        cloneMenu->setPosition(ccp(35.f, winSize.height / 2));
        // cloneBtn->setPosition(ccp(0, winSize.height / 2 - 25));
    }
    if (hax.viewAttempts) {
        CCMenu* infoMenu = CCMenu::create();
        CCSprite* infoSpr = cocos2d::CCSprite::create("GJ_infoIcon.png");
        CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, self, menu_selector(LevelInfoLayer::onViewLevelInfo));

        infoBtn->setSizeMult(1.5f);

        self->addChild(infoMenu, 1001);
        infoMenu->addChild(infoBtn);
        infoMenu->setPosition(ccp(25.f, 25.f));
    }
    return true;
}
bool (*TRAM_LevelEditorLayer_init)(LevelEditorLayer* self, GJGameLevel* level);
bool LevelEditorLayer_init(LevelEditorLayer* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.objectLimitHack)
        setObjectLimit(INCREASED_OBJECT_LIMIT - 1);
    else
        setObjectLimit(OBJECT_LIMIT);

    return TRAM_LevelEditorLayer_init(self, level);
}
// void (*TRAM_GameLevelManager_uploadLevel)(GameLevelManager* self, GJGameLevel* level);
// void GameLevelManager_uploadLevel(GameLevelManager* self, GJGameLevel* level) {
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.upload100KbFix) {
//         GameManager* gman = GameManager::sharedState();
//         gman->reportAchievementWithID("geometry.ach.submit", 100);
//         const char* key = itoa(MEMBER_BY_OFFSET(int, level, 0x1a4)).c_str(); // getM_ID
//         cocos2d::CCDictionary* crapDict = MEMBER_BY_OFFSET(cocos2d::CCDictionary*, self, 0x14C);
//         if (crapDict->objectForKey(key) != nullptr) {
//             cocos2d::CCLog("Quitting early!");
//             return;
//         }
//         crapDict->setObject(cocos2d::CCNode::create(), key);
//         std::string url = "http://nikolyas.141412.xyz/testin/uploadGJLevel.php";
//         std::string params = "udid=";
//         params += MEMBER_BY_OFFSET(std::string, gman, 0x164);
//         params += "&username=";
//         params += MEMBER_BY_OFFSET(std::string, gman, 0x168);
//         params += "&secret=Wmfd2893gb7&levelID=";
//         params += itoa(MEMBER_BY_OFFSET(int, level, 0x128));
//         params += "&levelName=";
//         params += MEMBER_BY_OFFSET(std::string, level, 0x12c);
//         params += "&levelDesc=";
//         params += MEMBER_BY_OFFSET(std::string, level, 0x130);
//         params += "&levelString=";
//         params += MEMBER_BY_OFFSET(std::string, level, 0x134);
//         params += "&levelVersion=";
//         params += itoa(MEMBER_BY_OFFSET(int, level, 0x15c));
//         params += "&levelLength=";
//         params += itoa(MEMBER_BY_OFFSET(int, level, 0x178));
//         params += "&audioTrack=";
//         params += itoa(MEMBER_BY_OFFSET(int, level, 0x144));
//         params += "&gameVersion=3";

//         cocos2d::extension::CCHttpRequest* request = new cocos2d::extension::CCHttpRequest();
//         request->setUrl(url.c_str());
//         request->setRequestType(cocos2d::extension::CCHttpRequest::HttpRequestType::kHttpPost);
//         request->setResponseCallback(self, callfuncND_selector(GameLevelManager::onUploadLevelCompleted));
//         if (self != NULL) self->retain();
//         request->setTag(key);
//         request->setRequestData(params.c_str(), strlen(params.c_str()) + 1);

//         // cocos2d::CCLog(request->getUrl());
//         // cocos2d::CCLog(params.c_str());
//         // cocos2d::CCLog(request->getRequestData());
//         // cocos2d::CCLog(request->getTag());

//         cocos2d::extension::CCHttpClient::getInstance()->send(request);

//         request->release();
//     } else {
//         TRAM_GameLevelManager_uploadLevel(self, level);
//     }
// }
void UILayer::speedUp() {
    PlayerObject* player = getPlayer();
    addXVelocity(player, 0.5d);
    player->logValues();
}
void UILayer::speedDown() {
    PlayerObject* player = getPlayer();
    addXVelocity(player, -0.5d);
    player->logValues();
}
void UILayer::gravityUp() {
    PlayerObject* player = getPlayer();
    addGravity(player, 0.5d);
    player->logValues();
}
void UILayer::gravityDown() {
    PlayerObject* player = getPlayer();
    addGravity(player, -0.5d);
    player->logValues();
}
void UILayer::yStartUp() {
    PlayerObject* player = getPlayer();
    addYStart(player, 0.5d);
    player->logValues();
}
void UILayer::yStartDown() {
    PlayerObject* player = getPlayer();
    addYStart(player, -0.5d);
    player->logValues();
}
bool (*TRAM_UILayer_init)(UILayer* self);
bool UILayer_init(UILayer* self) {
    if (!TRAM_UILayer_init(self)) return false;
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    if (hax.cheatIndicator) {
        auto cheatIndicatorLabel = CCLabelBMFont::create(".", "bigFont.fnt");
        cheatIndicatorLabel->setPosition(ccp(15, winSize.height - 10));
        switch (hax.getCheatIndicatorColor()) {
            case CheatIndicatorColor::Green:
                cheatIndicatorLabel->setColor(ccGREEN);
                break;
            case CheatIndicatorColor::Yellow:
                cheatIndicatorLabel->setColor(ccYELLOW);
                break;
            case CheatIndicatorColor::Red:
                cheatIndicatorLabel->setColor(ccRED);
                break;
            default:
                cheatIndicatorLabel->setColor(ccWHITE);
                break;
        };
        hax.cheatIndicatorLabel = cheatIndicatorLabel;
        self->addChild(cheatIndicatorLabel, 10000);
    }
    if (hax.showPercentage) {
        auto percentageLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        percentageLabel->setPosition(ccp(winSize.width / 2, winSize.height - 10));
        percentageLabel->setScale(0.5f);
        hax.percentageLabel = percentageLabel;
        self->addChild(percentageLabel, 10000);
    }
    if (hax.pCommand) {
        auto menu = CCMenu::create();
        menu->setPosition(ccp(winSize.width - 30, winSize.height - 40));

        auto btnSpr1 = ButtonSprite::create("S+", 20);
        btnSpr1->setScale(1.0f);

        auto btn1 = CCMenuItemSpriteExtra::create(btnSpr1, btnSpr1, self, menu_selector(UILayer::speedUp));
        btn1->setPosition(ccp(0, -55));
        menu->addChild(btn1);

        auto btnSpr2 = ButtonSprite::create("S-", 20);
        btnSpr2->setScale(1.0f);

        auto btn2 = CCMenuItemSpriteExtra::create(btnSpr2, btnSpr2, self, menu_selector(UILayer::speedDown));
        btn2->setPosition(ccp(0, -85));
        menu->addChild(btn2, 10002);

        auto btnSpr3 = ButtonSprite::create("G+", 20);
        btnSpr3->setScale(1.0f);

        auto btn3 = CCMenuItemSpriteExtra::create(btnSpr3, btnSpr3, self, menu_selector(UILayer::gravityUp));
        btn3->setPosition(ccp(0, -115));
        menu->addChild(btn3, 10003);

        auto btnSpr4 = ButtonSprite::create("G-", 20);
        btnSpr4->setScale(1.0f);

        auto btn4 = CCMenuItemSpriteExtra::create(btnSpr4, btnSpr4, self, menu_selector(UILayer::gravityDown));
        btn4->setPosition(ccp(0, -145));
        menu->addChild(btn4, 10004);

        auto btnSpr5 = ButtonSprite::create("Y+", 20);
        btnSpr5->setScale(1.0f);

        auto btn5 = CCMenuItemSpriteExtra::create(btnSpr5, btnSpr5, self, menu_selector(UILayer::yStartUp));
        btn5->setPosition(ccp(0, -175));
        menu->addChild(btn5, 10005);

        auto btnSpr6 = ButtonSprite::create("Y-", 20);
        btnSpr6->setScale(1.0f);

        auto btn6 = CCMenuItemSpriteExtra::create(btnSpr6, btnSpr6, self, menu_selector(UILayer::yStartDown));
        btn6->setPosition(ccp(0, -205));
        menu->addChild(btn6, 10006);

        self->addChild(menu, 10000);
    }
    return true;
}
void (*TRAM_PlayLayer_update)(PlayLayer* self, float dt);
void PlayLayer_update(PlayLayer* self, float dt) {
    TRAM_PlayLayer_update(self, dt);
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    UILayer* uiLayer = getUILayer(self);
    if (hax.cheatIndicator) {
        if (!hax.cheatIndicatorLabel) {
            auto cheatIndicatorLabel = CCLabelBMFont::create(".", "bigFont.fnt");
            cheatIndicatorLabel->setPosition(ccp(15, winSize.height - 15));
            hax.cheatIndicatorLabel = cheatIndicatorLabel;
            uiLayer->addChild(cheatIndicatorLabel, 10000);
        } else if (!hax.cheatIndicatorLabel->isVisible())
            hax.cheatIndicatorLabel->setVisible(true);

        switch (hax.getCheatIndicatorColor()) {
            case CheatIndicatorColor::Green:
                hax.cheatIndicatorLabel->setColor(ccGREEN);
                break;
            case CheatIndicatorColor::Yellow:
                hax.cheatIndicatorLabel->setColor(ccYELLOW);
                break;
            case CheatIndicatorColor::Red:
                hax.cheatIndicatorLabel->setColor(ccRED);
                break;
            default:
                hax.cheatIndicatorLabel->setColor(ccWHITE);
                break;
        };
    } else {
        if (hax.cheatIndicatorLabel && hax.cheatIndicatorLabel->isVisible()) 
            hax.cheatIndicatorLabel->setVisible(false);
    }
    if (hax.showPercentage) {
        if (!hax.percentageLabel) {
            auto percentageLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
            percentageLabel->setPosition(ccp(winSize.width / 2, winSize.height - 10));
            percentageLabel->setScale(0.5f);
            hax.percentageLabel = percentageLabel;
            uiLayer->addChild(percentageLabel, 10000);
        } else if (!hax.percentageLabel->isVisible()) {
            hax.percentageLabel->setVisible(true);
        }
        hax.percentageLabel->setString(CCString::createWithFormat("%i%%", getCurrentPercentage())->getCString());
    } else {
        if (hax.percentageLabel && hax.percentageLabel->isVisible())
            hax.percentageLabel->setVisible(false);
    }
}
/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!! SHOUTOUT TO ADELFA/AKQANILE FOR IMPLEMENTATION !!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/ 
bool (*TRAM_CCString_initWithFormatAndValist)(cocos2d::CCString* self, const char* format, va_list ap);
bool CCString_initWithFormatAndValist(cocos2d::CCString* self, const char* format, va_list ap) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.upload100KbFix) {
         size_t buf_size = static_cast<size_t>(vsnprintf(nullptr, 0, format, ap)) + 1;
        char* buf = static_cast<char*>(malloc(buf_size));

        vsprintf(buf, format, ap);

        typedef std::string* (*gd_string_assign_t)(void*, const char*, uint);
        gd_string_assign_t gd_string_assign = reinterpret_cast<gd_string_assign_t>(get_address(basicstring_assign));
        gd_string_assign((void*)&self->m_sString, buf, buf_size - 1);
        
        free(buf);

        return true;
    } else {
        return TRAM_CCString_initWithFormatAndValist(self, format, ap);
    }
}

bool (*TRAM_EditorPauseLayer_init)(cocos2d::CCLayer* self, LevelEditorLayer* editLayer);
bool EditorPauseLayer_init(cocos2d::CCLayer* self, LevelEditorLayer* editLayer) {
    if (!TRAM_EditorPauseLayer_init(self, editLayer)) return false;
    HaxManager& hax = HaxManager::sharedState();
    if (hax.objectCounter) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        auto objectLimit = OBJECT_LIMIT;
        if (hax.objectLimitHack) objectLimit = INCREASED_OBJECT_LIMIT;
        int objectCount = getObjectCount(editLayer);
        auto counterLabel = CCLabelBMFont::create(
            CCString::createWithFormat("%i/%i objects", objectCount, objectLimit)->getCString(), 
            "goldFont.fnt"
        );
        counterLabel->setScale(0.5f);
        counterLabel->setAnchorPoint({0.f, 0.5f});
        counterLabel->setPosition(ccp(10, winSize.height - 15));
        self->addChild(counterLabel, 1000);
    }
    return true;
}
void MenuLayer::onMenuInfo() {
    FLAlertLayer::create(
        nullptr,
        "Game Information",
        CCString::createWithFormat(
            "<cg>OMNImenu</c> %s\n<cl>Geometry Dash</c> %s\n<cr>Special Thanks</c>: <cy>akqanile</c>, <cy>Hris69</c>, <cy>Pololak</c>, <cy>Nikolyas</c>\nWith love from <cy>AntiMatter</c> <cr><3</c>", 
            MENU_VERSION, READABLE_GAME_VERSION)->getCString(),
        "OK",
        nullptr,
        300.f
    )->show();
}
bool (*TRAM_MenuLayer_init)(cocos2d::CCLayer* self);
bool MenuLayer_init(cocos2d::CCLayer* self) {
    if (!TRAM_MenuLayer_init(self)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    CCMenu* infoMenu = CCMenu::create();
    CCSprite* infoSpr = cocos2d::CCSprite::create("GJ_infoIcon.png");
    CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, self, menu_selector(MenuLayer::onMenuInfo));
    infoBtn->setSizeMult(1.5f);

    self->addChild(infoMenu, 1001);
    infoMenu->addChild(infoBtn);
    infoMenu->setPosition(ccp(winSize.width - 25.f, winSize.height - 25.f));

    return true;
}
void (*TRAM_PlayLayer_togglePracticeMode)(PlayLayer* self, bool toggle);
void PlayLayer_togglePracticeMode(PlayLayer* self, bool toggle) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.practiceMusic && getPlayLayerPractice(self) != toggle) {
        setPlayLayerPractice(self, toggle);
        UILayer* uiLayer = getUILayer(getPlayLayer());
        uiLayer->toggleCheckpointsMenu(toggle);
        if (!toggle) {
            cocos2d::CCArray* checkpoints = getPlayLayerCheckpoints(self);
            while (checkpoints->count() > 0) {
                self->removeLastCheckpoint();
            }
            MEMBER_BY_OFFSET(bool, self, PlayLayer__m_unkPrac) = true;
            self->resetLevel();
        }
    } else TRAM_PlayLayer_togglePracticeMode(self, toggle);
}
void (*TRAM_CCNode_setVisible)(CCNode* self, bool toggle);
void CCNode_setVisible(CCNode* self, bool toggle) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.forceVisibility) {
        TRAM_CCNode_setVisible(self, true);
    } else TRAM_CCNode_setVisible(self, toggle);
}
void (*TRAM_CCSprite_setVisible)(CCSprite* self, bool toggle);
void CCSprite_setVisible(CCSprite* self, bool toggle) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.forceVisibility) {
        TRAM_CCSprite_setVisible(self, true);
    } else TRAM_CCSprite_setVisible(self, toggle);
}
// void (*TRAM_EditorUI_zoomGameLayer)(void* self, bool zoomIn);
// void EditorUI_zoomGameLayer(void* self, bool zoomIn) {
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.zoomBypass) {
//         void* editorLayer = MEMBER_BY_OFFSET(void*, self, 0x71);
//         cocos2d::CCLayer* gameLayer = MEMBER_BY_OFFSET(cocos2d::CCLayer*, editorLayer, 0x158); // getGameLayer
//         float scale = gameLayer->getScale();
//         if (zoomIn) {
//             scale += 0.1;
//         } else {
//             if (scale > 0.1)
//                 scale -= 0.1;
//         }
//         gameLayer->setScale(scale);
//     } else TRAM_EditorUI_zoomGameLayer(self, zoomIn);
// }

// void (*TRAM_PlayerObject_update)(PlayerObject* self, float dt);
// void PlayerObject_update(PlayerObject* self, float dt) {
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.instantComplete) return;
//     TRAM_PlayerObject_update(self, dt);
// }

[[gnu::constructor]]
int main() {
    void* handle = dlopen(MAIN_LIBRARY, RTLD_NOW);

    DobbyHook(
        dlsym(handle, "_ZN9PlayLayer13destroyPlayerEv"),
        reinterpret_cast<void*>(PlayLayer_destroyPlayer),
        reinterpret_cast<void**>(&TRAM_PlayLayer_destroyPlayer)
    );
    DobbyHook(
        dlsym(handle, "_ZN9MenuLayer11onMoreGamesEv"),
        (dobby_dummy_func_t) MenuLayer_onMoreGames,
        nullptr
    );
    DobbyHook(
        dlsym(handle, "_ZN11GameManager15isColorUnlockedEib"),
        reinterpret_cast<void*>(GameManager_isColorUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isColorUnlocked)
    );
    DobbyHook(
        dlsym(handle, "_ZN11GameManager14isIconUnlockedEi"),
        reinterpret_cast<void*>(GameManager_isIconUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isIconUnlocked)
    );
    DobbyHook(
        dlsym(handle, "_ZN15CCTextInputNode12setCharLimitEi"),
        reinterpret_cast<void*>(CCTextInputNode_setCharLimit),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_setCharLimit)
    );
    DobbyHook(
        dlsym(handle, "_ZN15CCTextInputNode11updateLabelEPKc"),
        reinterpret_cast<void*>(CCTextInputNode_updateLabel),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_updateLabel)
    );
    DobbyHook(
        dlsym(handle, "_ZN15CCTextInputNode18setProfanityFilterEb"),
        reinterpret_cast<void*>(CCTextInputNode_setProfanityFilter),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_setProfanityFilter)
    );
    // DobbyHook(
    //     dlsym(handle, "_ZThn296_N16SliderTouchLogic12ccTouchMovedEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
    //     reinterpret_cast<void*>(SliderTouchLogic_ccTouchMoved),
    //     reinterpret_cast<void**>(&TRAM_SliderTouchLogic_ccTouchMoved)
    // );
    DobbyHook(
        dlsym(handle, "_ZN10PauseLayer11customSetupEv"),
        reinterpret_cast<void*>(PauseLayer_customSetup),
        reinterpret_cast<void**>(&TRAM_PauseLayer_customSetup)
    );
    DobbyHook(
        dlsym(handle, "_ZN9PlayLayer10resetLevelEv"),
        reinterpret_cast<void*>(PlayLayer_resetLevel),
        reinterpret_cast<void**>(&TRAM_PlayLayer_resetLevel)
    );
    DobbyHook(
        dlsym(handle, "_ZN14EditLevelLayer4initEP11GJGameLevel"),
        reinterpret_cast<void*>(EditLevelLayer_init),
        reinterpret_cast<void**>(&TRAM_EditLevelLayer_init)
    );
    DobbyHook(
        dlsym(handle, "_ZN9PlayLayer13toggleFlippedEbb"),
        reinterpret_cast<void*>(PlayLayer_toggleFlipped),
        reinterpret_cast<void**>(&TRAM_PlayLayer_toggleFlipped)
    );
    DobbyHook(
        dlsym(handle, "_ZN8EditorUI12showMaxErrorEv"),
        reinterpret_cast<void*>(EditorUI_showMaxError),
        reinterpret_cast<void**>(&TRAM_EditorUI_showMaxError)
    );
    // DobbyHook(
    //     dlsym(handle, "_ZN8EditorUI8onCreateEv"),
    //     reinterpret_cast<void*>(EditorUI_onCreate),
    //     reinterpret_cast<void**>(&TRAM_EditorUI_onCreate)
    // );
    // DobbyHook(
    //     dlsym(handle, "_ZN12PlayerObject6updateEf"),
    //     reinterpret_cast<void*>(PlayerObject_update),
    //     reinterpret_cast<void**>(&TRAM_PlayerObject_update)
    // );
    DobbyHook(
        dlsym(handle, "_ZN7cocos2d16CCTransitionFade6createEfPNS_7CCSceneERKNS_10_ccColor3BE"),
        reinterpret_cast<void*>(CCTransitionFade_create),
        reinterpret_cast<void**>(&TRAM_CCTransitionFade_create)
    );
    DobbyHook(
        dlsym(handle, "_ZN8EditorUI26constrainGameLayerPositionEv"),
        reinterpret_cast<void*>(EditorUI_constrainGameLayerPosition),
        reinterpret_cast<void**>(&TRAM_EditorUI_constrainGameLayerPosition)
    );
    // DobbyHook(
    //     dlsym(handle, "_ZN8EditorUI13zoomGameLayerPositionEb"),
    //     reinterpret_cast<void*>(EditorUI_zoomGameLayer),
    //     reinterpret_cast<void**>(&TRAM_EditorUI_zoomGameLayer)
    // );
    DobbyHook(
        dlsym(handle, "_ZN14LevelInfoLayer4initEP11GJGameLevel"),
        reinterpret_cast<void*>(LevelInfoLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelInfoLayer_init)
    );
    // DobbyHook(
    //     dlsym(handle, "_ZN16GameLevelManager11uploadLevelEP11GJGameLevel"),
    //     reinterpret_cast<void*>(GameLevelManager_uploadLevel),
    //     reinterpret_cast<void**>(&TRAM_GameLevelManager_uploadLevel)
    // );
    DobbyHook(
        dlsym(handle, "_ZN7UILayer4initEv"),
        reinterpret_cast<void*>(UILayer_init),
        reinterpret_cast<void**>(&TRAM_UILayer_init)
    );
    DobbyHook(
        dlsym(handle, "_ZN7cocos2d8CCString23initWithFormatAndValistEPKcSt9__va_list"),
        reinterpret_cast<void*>(CCString_initWithFormatAndValist),
        reinterpret_cast<void**>(&TRAM_CCString_initWithFormatAndValist)
    );
    // DobbyHook(
    //     dlsym(handle, "_ZN7UILayer4drawEv"),
    //     reinterpret_cast<void*>(UILayer_draw),
    //     reinterpret_cast<void**>(&TRAM_UILayer_draw)
    // );
    DobbyHook(
        dlsym(handle, "_ZN9PlayLayer6updateEf"),
        reinterpret_cast<void*>(PlayLayer_update),
        reinterpret_cast<void**>(&TRAM_PlayLayer_update)
    );
    DobbyHook(
        dlsym(handle, "_ZN16EditorPauseLayer4initEP16LevelEditorLayer"),
        reinterpret_cast<void*>(EditorPauseLayer_init),
        reinterpret_cast<void**>(&TRAM_EditorPauseLayer_init)
    );
    DobbyHook(
        dlsym(handle, "_ZN9MenuLayer4initEv"),
        reinterpret_cast<void*>(MenuLayer_init),
        reinterpret_cast<void**>(&TRAM_MenuLayer_init)
    );
    DobbyHook(
        dlsym(handle, "_ZN9PlayLayer18togglePracticeModeEb"),
        reinterpret_cast<void*>(PlayLayer_togglePracticeMode),
        reinterpret_cast<void**>(&TRAM_PlayLayer_togglePracticeMode)
    );
    DobbyHook(
        dlsym(handle, "_ZN7cocos2d6CCNode10setVisibleEb"),
        reinterpret_cast<void*>(CCNode_setVisible),
        reinterpret_cast<void**>(&TRAM_CCNode_setVisible)
    );
    // DobbyHook(
    //     dlsym(handle, "_ZN7cocos2d8CCSprite10setVisibleEb"),
    //     reinterpret_cast<void*>(CCNode_setVisible),
    //     reinterpret_cast<void**>(&TRAM_CCNode_setVisible)
    // );
}
