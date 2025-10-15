#include "hook.hpp"
#include <jni.h>
#include "PlayLayer.hpp"
#include "UILayer.hpp"
#include "platform/android/jni/JniHelper.h"

JNIEnv* getEnv() {
    JNIEnv* env;

    if (cocos2d::JniHelper::getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        cocos2d::CCLog("Failed to get the environment using GetEnv()");
        return nullptr;
    }
    if (static_cast<int>(cocos2d::JniHelper::getJavaVM()->AttachCurrentThread(&env, 0)) < 0) {
        cocos2d::CCLog("Failed to get the environment using AttachCurrentThread()");
        return nullptr;
    }

    return env;
}

void seekBackgroundMusicTo(int ms) {
    JNIEnv* env = getEnv();

    if (!env) {
        cocos2d::CCLog("Failed to get JNI environment");
        return;
    }
    jclass Cocos2dxActivity = env->FindClass("org/cocos2dx/lib/Cocos2dxActivity");
    if (Cocos2dxActivity == nullptr) {
        cocos2d::CCLog("Failed to find Cocos2dxActivity class");
        return;
    }
    // some cocos2d java class names have been obfuscated, but not all
    jfieldID fieldID_backgroundMusicPlayer = env->GetStaticFieldID(Cocos2dxActivity, "backgroundMusicPlayer", "Lorg/cocos2dx/lib/p;");
    if (fieldID_backgroundMusicPlayer == nullptr) {
        cocos2d::CCLog("Failed to get field ID of backgroundMusicPlayer");
        return;
    }
    jobject backgroundMusicPlayer = env->GetStaticObjectField(Cocos2dxActivity, fieldID_backgroundMusicPlayer);
    if (backgroundMusicPlayer == nullptr) {
        cocos2d::CCLog("Failed to get backgroundMusicPlayer");
        return;
    }

    jclass Cocos2dxMusic = env->FindClass("org/cocos2dx/lib/p");
    if (Cocos2dxMusic == nullptr) {
        cocos2d::CCLog("Failed to get Cocos2dxMusic");
        return;
    }
    jfieldID fieldID_mBackgroundMediaPlayer = env->GetFieldID(Cocos2dxMusic, "mBackgroundMediaPlayer", "Landroid/media/MediaPlayer;");
    if (fieldID_mBackgroundMediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get field ID of mBackgroundMediaPlayer");
        return;
    }
    jobject mBackgroundMediaPlayer = env->GetObjectField(backgroundMusicPlayer, fieldID_mBackgroundMediaPlayer);
    if (mBackgroundMediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get mBackgroundMediaPlayer");
        return;
    }

    jclass MediaPlayer = env->GetObjectClass(mBackgroundMediaPlayer);
    if (MediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get MediaPlayer");
        return;
    }
    
    bool useNew = true;
    jclass versionClass;
    jfieldID sdkIntFieldID;
    jint sdkJint;
    int sdkInt;
    jmethodID seekTo;
    do {
        versionClass = env->FindClass("android/os/Build$VERSION");
        if (versionClass == nullptr) {
            cocos2d::CCLog("Warning: Failed to get class Build$VERSION. Using old \"broken\" method.");
            useNew = false;
            break;
        }
        sdkIntFieldID = env->GetStaticFieldID(versionClass, "SDK_INT", "I");
        if (sdkIntFieldID == nullptr) {
            cocos2d::CCLog("Warning: Failed to get field ID of SDK_INT. Using old \"broken\" method.");
            useNew = false;
            break;
        }

        sdkJint = env->GetStaticIntField(versionClass, sdkIntFieldID);
        sdkInt = static_cast<int>(sdkJint);
        if (sdkInt < 26) {
            cocos2d::CCLog("sdkInt: %i < 26. Using old \"broken\" method.", sdkInt);
            useNew = false;
            break;
        }
        seekTo = env->GetMethodID(MediaPlayer, "seekTo", "(JI)V");
        if (seekTo == nullptr) {
            cocos2d::CCLog("Warning: Failed to get method ID of seekTo(long, int). Using old \"broken\" method.");
            useNew = false;
            break;
        }
    } while (0);

    if (useNew) {
        env->CallVoidMethod(mBackgroundMediaPlayer, seekTo, static_cast<jlong>(static_cast<long>(ms)), static_cast<jint>(2));
    } else {
        jmethodID oldSeekTo = env->GetMethodID(MediaPlayer, "seekTo", "(I)V");
        if (oldSeekTo == nullptr) {
            cocos2d::CCLog("Failed to get method ID of seekTo(int)");
            return;
        }
        env->CallVoidMethod(mBackgroundMediaPlayer, oldSeekTo, static_cast<jint>(ms));
    } 
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

void (*TRAM_PlayLayer_resetLevel)(PlayLayer* self);
void PlayLayer_resetLevel(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.practiceMusic && getPlayLayerPractice(self)) {
        auto audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
        int seekTime = 0;
        CCPoint startPos = getStartPos(self);
        seekTime = floorf((startPos.x / 311.58f) * 1000.f);
        CCNode* lastCheckpoint = self->getLastCheckpoint();
        if (lastCheckpoint != nullptr) {
            CCPoint lastCheckpointPos = getCheckpointPosition(lastCheckpoint);
            seekTime = floorf((lastCheckpointPos.x / 311.58f) * 1000.f);
        }
        if (seekTime > 0) {
            JNIEnv* env = getEnv();
            if (env) {
                seekBackgroundMusicTo(seekTime);
            } else {
                cocos2d::CCLog("Failed to get Java Env");
                audioEngine->setBackgroundMusicTime(static_cast<float>(seekTime) / 1000.f);
                audioEngine->resumeBackgroundMusic();
            }
        } else {
            audioEngine->setBackgroundMusicTime(0.f);
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

#if GAME_VERSION > 1
void (*TRAM_PlayLayer_toggleFlipped)(void* self, bool p1, bool p2);
void PlayLayer_toggleFlipped(void* self, bool p1, bool p2) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.noMirror) return;
    TRAM_PlayLayer_toggleFlipped(self, p1, p2);
}
#endif

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

void PlayLayer_om() {
    Omni::hook("_ZN9PlayLayer13destroyPlayerEv", 
        reinterpret_cast<void*>(PlayLayer_destroyPlayer),
        reinterpret_cast<void**>(&TRAM_PlayLayer_destroyPlayer));
    Omni::hook("_ZN9PlayLayer18togglePracticeModeEb",
        reinterpret_cast<void*>(PlayLayer_togglePracticeMode),
        reinterpret_cast<void**>(&TRAM_PlayLayer_togglePracticeMode));
    Omni::hook("_ZN9PlayLayer6updateEf",
        reinterpret_cast<void*>(PlayLayer_update),
        reinterpret_cast<void**>(&TRAM_PlayLayer_update));
#if GAME_VERSION > 1
    Omni::hook("_ZN9PlayLayer13toggleFlippedEbb",
        reinterpret_cast<void*>(PlayLayer_toggleFlipped),
        reinterpret_cast<void**>(&TRAM_PlayLayer_toggleFlipped));
#endif
    Omni::hook("_ZN9PlayLayer10resetLevelEv",
        reinterpret_cast<void*>(PlayLayer_resetLevel),
        reinterpret_cast<void**>(&TRAM_PlayLayer_resetLevel));
}