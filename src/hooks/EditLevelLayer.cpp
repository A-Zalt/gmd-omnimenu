#include "hook.hpp"
#include "EditLevelLayer.hpp"
#include <algorithm>
#include "Utils.hpp"
#include "LevelTools.hpp"

void EditLevelLayer::onViewLevelInfo() {
    GJGameLevel* level = getEditLayerLevel(this);
    std::string s = level->m_sLevelString;
    std::string::difference_type count = std::count(s.begin(), s.end(), ';');
    int objectCount = std::max(0, static_cast<int>(count) - 1);
    CCString* flAlertInsides = CCString::createWithFormat(
#if GAME_VERSION < GV_1_6
        "<cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cr>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cy>Audio Track</c>: %s (ID %i)\n<cz>Object Count</c>: %i",
#else
        "<cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cl>Total Jumps</c>: %i\n<cp>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cy>Audio Track</c>: %s (ID %i)\n<cz>Object Count</c>: %i",
#endif
        level->m_sLevelName.c_str(),
        level->m_nAttempts,
#if GAME_VERSION >= GV_1_6
        level->m_nJumps,
#endif
        level->m_nNormalPercent,
        level->m_nPracticePercent,
        LevelTools::getAudioTitle(level->m_nAudioTrack),
        level->m_nAudioTrack,
        objectCount
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
void EditLevelLayer::onExport() {
    GJGameLevel* level = getEditLayerLevel(this);
    std::string name = level->m_sLevelName;
    if (!strcmp(name.c_str(), "")) name = "Unnamed";
    name += ".gmd";
    JNIEnv* env = getEnv();
    if (!env || env == nullptr) {
        GDSHARE_FL("Error: could not get JNI Environment");
        return;
    }
    HaxManager& hax = HaxManager::sharedState();
    hax.gdShareLevel = level;
    jclass activityClass = env->FindClass(JAVA_PATH_MAIN "/GeometryJump");
    if (activityClass == nullptr) {
        GDSHARE_FL("Error: could not get the activity class");
        return;
    }
    jmethodID showPicker = env->GetMethodID(activityClass, "showSaveFilePicker", "(Ljava/lang/String;)V");
    if (showPicker == nullptr) {
        GDSHARE_FL("Error: could not find showSaveFilePicker method. Are you sure you changed the smali?");
        return;
    }
    jstring jname = env->NewStringUTF(name.c_str());
    jmethodID getActivity = env->GetStaticMethodID(activityClass, "getInstance", "()L" JAVA_PATH_MAIN "/GeometryJump;");
    if (getActivity == nullptr) {
        GDSHARE_FL("Error: could not find getInstance method. Are you sure you changed the smali?");
        return;
    }
    jobject activity = env->CallStaticObjectMethod(activityClass, getActivity);
    env->CallVoidMethod(activity, showPicker, jname);
}
bool (*TRAM_EditLevelLayer_init)(EditLevelLayer* self, GJGameLevel* level);
bool EditLevelLayer_init(EditLevelLayer* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
#ifndef FORCE_AUTO_SAFE_MODE
    if (hax.getModuleEnabled(ModuleID::VERIFY_BYPASS)) {
        level->m_bIsVerified = true;
    }
#endif
    if (hax.getModuleEnabled(ModuleID::VIEW_LEVEL_STATS)) {
        CCMenu* infoMenu = CCMenu::create();
        CCSprite* infoSpr = createInfoSprite();
        CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, self, menu_selector(EditLevelLayer::onViewLevelInfo));

        infoBtn->setSizeMult(1.5f);

        self->addChild(infoMenu, 1001);
        infoMenu->addChild(infoBtn);
        infoMenu->setPosition(ccp(25.f, 25.f));
    }
    if (hax.getModuleEnabled(ModuleID::GDSHARE)) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        
        CCMenu* shareMenu = CCMenu::create();
        CCSprite* exportSpr = cocos2d::CCSprite::create("gdshare_export.png");
        CCMenuItemSpriteExtra* exportBtn = CCMenuItemSpriteExtra::create(exportSpr, exportSpr, self, menu_selector(EditLevelLayer::onExport));

        self->addChild(shareMenu, 1001);
        shareMenu->addChild(exportBtn);
#if GAME_VERSION < GV_1_7
        shareMenu->setPosition(ccp(winSize.width - 29, winSize.height - 150.f));
#else
        shareMenu->setPosition(ccp(29, winSize.height / 2));
#endif
    }
    return TRAM_EditLevelLayer_init(self, level);
}
void EditLevelLayer_om() {
    Omni::hook("_ZN14EditLevelLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(EditLevelLayer_init),
        reinterpret_cast<void**>(&TRAM_EditLevelLayer_init));
}