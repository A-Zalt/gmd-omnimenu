#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "MyLevelsLayer.hpp"
#include "EditLevelLayer.hpp"

void MyLevelsLayer::onImport() {
    JNIEnv* env = getEnv();
    if (!env || env == nullptr) {
        GDSHARE_FL("Error: could not get JNI Environment");
        return;
    }
    jclass activityClass = env->FindClass(JAVA_PATH_MAIN "/GeometryJump");
    if (activityClass == nullptr) {
        GDSHARE_FL("Error: could not get the activity class");
        return;
    }
    jmethodID showPicker = env->GetMethodID(activityClass, "showOpenFilePicker", "()V");
    if (showPicker == nullptr) {
        GDSHARE_FL("Error: could not find showOpenFilePicker method. Are you sure you changed the smali?");
        return;
    }
    jmethodID getActivity = env->GetStaticMethodID(activityClass, "getInstance", "()L" JAVA_PATH_MAIN "/GeometryJump;");
    if (getActivity == nullptr) {
        GDSHARE_FL("Error: could not find getInstance method. Are you sure you changed the smali?");
        return;
    }
    jobject activity = env->CallStaticObjectMethod(activityClass, getActivity);
    env->CallVoidMethod(activity, showPicker);
}

void MyLevelsLayer::FLAlert() {
    HaxManager& hax = HaxManager::sharedState();
    switch (hax.gdShareMessageID) {
        case 1:
            GDSHARE_FL("Error: could not load level");
            break;
        case 2:
            if (hax.gdShareData != 0) {
                GDSHARE_FL(fmt::format(
                    "Error: level was made for a newer version of Geometry Dash ({} > {})",
                    intToReadableGV(hax.gdShareData),
                    READABLE_GAME_VERSION
                ).c_str());
            } else {
                GDSHARE_FL("Error: level was made for a newer version of Geometry Dash");
            }
            break;
        default:
            GDSHARE_FL("Unknown error");
            break;
    }
    hax.gdShareMessageID = 0;
}

void MyLevelsLayer::loadLevel(GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (level == nullptr) {
        hax.gdShareMessageID = 1;
        runAction(CCSequence::create(
            CCDelayTime::create(0.2f),
            CCCallFunc::create(this, callfunc_selector(MyLevelsLayer::FLAlert)),
            nullptr
        ));
        // GDSHARE_FL("Error: could not load level");
        return;
    }
    CCLog("1");
    if (level->m_nGameVersion > GAME_VERSION) {
        hax.gdShareMessageID = 2;
        hax.gdShareData = level->m_nGameVersion;
        runAction(CCSequence::create(
            CCDelayTime::create(0.2f),
            CCCallFunc::create(this, callfunc_selector(MyLevelsLayer::FLAlert)),
            nullptr
        ));
        return;
    }
    level->m_nLevelID = 0;
    level->m_eLevelType = GJLevelType::Editor;
#ifdef FORCE_AUTO_SAFE_MODE
    level->m_bIsVerified = false;
#if GAME_VERSION > GV_1_2
    level->m_bIsDemon = false;
    level->m_nStars = 0;
#endif
#endif
    getLocalLevels()->addObject(level);
    // this delay is necessary. TODO come up with a better solution?
    runAction(CCSequence::create(
        CCDelayTime::create(0.2f),
        CCCallFunc::create(this, callfunc_selector(MyLevelsLayer::reload)),
        nullptr
    ));
    
}
void MyLevelsLayer::reload() {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, MyLevelsLayer::scene()));
}

bool (*TRAM_MyLevelsLayer_init)(MyLevelsLayer* self);
bool MyLevelsLayer_init(MyLevelsLayer* self) {
    if (!TRAM_MyLevelsLayer_init(self)) return false;
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::GDSHARE)) {
        hax.myLevelsLayer = self;
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        CCMenu* importMenu = CCMenu::create();
        CCSprite* importSpr = cocos2d::CCSprite::create("gdshare_import.png");
        CCMenuItemSpriteExtra* importBtn = CCMenuItemSpriteExtra::create(importSpr, importSpr, self, menu_selector(MyLevelsLayer::onImport));

        self->addChild(importMenu, 1000);
        importMenu->addChild(importBtn);
        importMenu->setPosition(ccp(winSize.width - 30.f, winSize.height - 90.f));
    }
    return true;
}

void MyLevelsLayer_om() {
    Omni::hook("_ZN13MyLevelsLayer4initEv",
        reinterpret_cast<void*>(MyLevelsLayer_init),
        reinterpret_cast<void**>(&TRAM_MyLevelsLayer_init));
}