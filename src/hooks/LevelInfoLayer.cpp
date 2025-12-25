#include "hook.hpp"
#include "LevelInfoLayer.hpp"
#include "GJGameLevel.hpp"
#include "FLAlertLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "LevelTools.hpp"
// #include "ShareCommentLayer.hpp"
#include <algorithm>

#if GAME_VERSION < GV_1_6
void LevelInfoLayer::onViewLevelInfo() {
    GJGameLevel* level = getInfoLayerLevel(this);
    std::string s = level->m_sLevelString;
    std::string::difference_type count = std::count(s.begin(), s.end(), ';');
    int objectCount = std::max(0, static_cast<int>(count) - 1);
    CCString* flAlertInsides = CCString::createWithFormat(
#if GAME_VERSION < GV_1_5
        "<cy>%s</c> by <cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cr>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cy>Audio Track</c>: %s (ID %i)\n<cl>Level ID</c>: %i\n<cb>User ID</c>: %i\n<cz>Object Count</c>: %i",
#else
        "<cy>%s</c> by <cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cr>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cy>Audio Track</c>: %s (ID %i)\n<cl>Level ID</c>: %i\n<cb>User ID</c>: %i\n<cy>Feature Score</c>: %i\n<cz>Object Count</c>: %i",
#endif
        level->m_sLevelName.c_str(),
        level->m_sUserName.c_str(),
        level->m_nAttempts,
        level->m_nNormalPercent,
        level->m_nPracticePercent,
        LevelTools::getAudioTitle(level->m_nAudioTrack),
        level->m_nAudioTrack,
        level->m_nLevelID,
        level->m_nUserID,
#if GAME_VERSION >= GV_1_5
        level->m_nFeatureScore,
#endif
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
#else
void (*TRAM_LevelInfoLayer_onLevelInfo)(LevelInfoLayer* self SEL_MenuHandler_1_7_compat2);
void LevelInfoLayer_onLevelInfo(LevelInfoLayer* self SEL_MenuHandler_1_7_compat2) {
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::VIEW_LEVEL_STATS)) return TRAM_LevelInfoLayer_onLevelInfo(self sender_param_1_7);
    GJGameLevel* level = getInfoLayerLevel(self);
    std::string s = level->m_sLevelString;
    std::string::difference_type count = std::count(s.begin(), s.end(), ';');
    int objectCount = std::max(0, static_cast<int>(count) - 1);
    CCString* flAlertInsides = CCString::createWithFormat(
// #if GAME_VERSION < GV_1_7
        "<cy>%s</c> by <cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cl>Total Jumps</c>: %i\n<cp>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cy>Audio Track</c>: %s (ID %i)\n<cr>Level ID</c>: %i\n<cb>User ID</c>: %i\n<cy>Feature Score</c>: %i\n<cz>Object Count</c>: %i",
// #else
//         "<cy>%s</c> by <cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cl>Total Jumps</c>: %i\n<cp>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cy>Audio Track</c>: %s (ID %i)\n<cr>Level ID</c>: %i\n<cb>User ID</c>: %i\n<cy>Feature Score</c>: %i\n<cg>Password</c>: %i\n<cz>Object Count</c>: %i",
// #endif
        level->m_sLevelName.c_str(),
        level->m_sUserName.c_str(),
        level->m_nAttempts,
        level->m_nJumps,
        level->m_nNormalPercent,
        level->m_nPracticePercent,
        LevelTools::getAudioTitle(level->m_nAudioTrack),
        level->m_nAudioTrack,
        level->m_nLevelID,
        level->m_nUserID,
        level->m_nFeatureScore,
// #if GAME_VERSION >= GV_1_7
//         level->m_nPassword,
// #endif
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
#endif
// void LevelInfoLayer::onComment() {
//     ShareCommentLayer::create(getInfoLayerLevel(this))->show();
// }
void LevelInfoLayer::onExport() {
    GJGameLevel* level = getInfoLayerLevel(this);
    std::string name = level->m_sLevelName;
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
bool (*TRAM_LevelInfoLayer_init)(LevelInfoLayer* self, GJGameLevel* level);
bool LevelInfoLayer_init(LevelInfoLayer* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
#if GAME_VERSION >= GV_1_7
    if (hax.getModuleEnabled(ModuleID::LEVEL_COPYING)) {
        level->m_nPassword = 1;
        level->m_nFailedPasswordAttempts = 0;
    }
#endif
    if (!TRAM_LevelInfoLayer_init(self, level)) return false;
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
#if GAME_VERSION < GV_1_7
    if (hax.getLeftLILButtons()) {
        CCMenu* leftMenu = CCMenu::create();
        self->addChild(leftMenu, 1000);
        leftMenu->setPosition(ccp(30.f, winSize.height / 2));
        if (hax.getModuleEnabled(ModuleID::LEVEL_COPYING)) {
            CCSprite* cloneSpr = cocos2d::CCSprite::create("GJ_duplicateBtn.png");
            CCMenuItemSpriteExtra* cloneBtn = CCMenuItemSpriteExtra::create(cloneSpr, cloneSpr, self, menu_selector(LevelInfoLayer::onClone));

            leftMenu->addChild(cloneBtn);
            cloneBtn->setPosition(ccp(0.f, 25.f));
        }
        if (hax.getModuleEnabled(ModuleID::GDSHARE)) {
            CCSprite* exportSpr = cocos2d::CCSprite::create("gdshare_export.png");
            CCMenuItemSpriteExtra* exportBtn = CCMenuItemSpriteExtra::create(exportSpr, exportSpr, self, menu_selector(LevelInfoLayer::onExport));

            leftMenu->addChild(exportBtn);
            exportBtn->setPosition(ccp(0.f, -25.f));
        }
    }
#else
    if (hax.getModuleEnabled(ModuleID::GDSHARE)) {
        CCSprite* exportSpr = cocos2d::CCSprite::create("gdshare_export.png");
        CCMenuItemSpriteExtra* exportBtn = CCMenuItemSpriteExtra::create(exportSpr, exportSpr, self, menu_selector(LevelInfoLayer::onExport));

        for (int i = 0; i < self->getChildrenCount(); i++) {
            auto menu = dynamic_cast<CCMenu*>(self->getChildren()->objectAtIndex(i));
            if (menu && menu->getChildrenCount() > 1) {
                menu->addChild(exportBtn);
                exportBtn->setPosition(ccp(-winSize.width / 2 + 30, 72.f));
            }
        }
    }
#endif
#if GAME_VERSION < GV_1_6
    if (hax.getModuleEnabled(ModuleID::VIEW_LEVEL_STATS)) {
        CCMenu* infoMenu = CCMenu::create();
        CCSprite* infoSpr = createInfoSprite();
        CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, self, menu_selector(LevelInfoLayer::onViewLevelInfo));

        infoBtn->setSizeMult(1.5f);

        self->addChild(infoMenu, 1001);
        infoMenu->addChild(infoBtn);
        infoMenu->setPosition(ccp(25.f, 25.f));

        // auto commentSpr = CCSprite::createWithSpriteFrameName("GJ_achBtn_001.png");
        // auto commentBtn = CCMenuItemSpriteExtra::create(commentSpr, commentSpr, self, menu_selector(LevelInfoLayer::onComment));
        // commentBtn->setSizeMult(1.5f);
        // infoMenu->addChild(commentBtn);
        // commentBtn->setPosition(ccp(0.f, 50.f));
    }
#endif
#if GAME_VERSION < GV_1_3
    if (hax.getModuleEnabled(ModuleID::SHOW_DIFFICULTY)) {
        CCSprite* diffSpr = CCSprite::createWithSpriteFrameName(
            CCString::createWithFormat(
                "difficulty_%02d_btn_001.png", 
                level->getAverageDifficulty()
            )->getCString()
        );
        self->addChild(diffSpr);
        diffSpr->setPosition(ccp(winSize.width / 2 - 120, winSize.height / 2 + 30));
    }
#endif
#ifdef NP4
    auto val = hax.featureTypeMap[level];
    if (val == 0) return true;
    CCSprite* featureFrame = CCSprite::create((val == 2) ? MAGIC_TEXTURE : FEATURED_TEXTURE);
    featureFrame->setPosition({(winSize.width / 2) - 120, (winSize.height / 2) + 35});
    featureFrame->_setZOrder(-1);
    self->addChild(featureFrame);
#endif
    return true;
}

#if GAME_VERSION >= GV_1_7
void (*TRAM_LevelInfoLayer_levelDownloadFinished)(LevelInfoLayer* self, GJGameLevel* level);
void LevelInfoLayer_levelDownloadFinished(LevelInfoLayer* self, GJGameLevel* level) {
    TRAM_LevelInfoLayer_levelDownloadFinished(self, level);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::LEVEL_COPYING)) {
        level->m_nPassword = 1;
        level->m_nFailedPasswordAttempts = 0;
    }
}
#endif

void LevelInfoLayer_om() {
    Omni::hook("_ZN14LevelInfoLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(LevelInfoLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelInfoLayer_init));
#if GAME_VERSION >= GV_1_6
    Omni::hook(
    #if GAME_VERSION < GV_1_7
        "_ZN14LevelInfoLayer11onLevelInfoEv",
    #else
        "_ZN14LevelInfoLayer11onLevelInfoEPN7cocos2d8CCObjectE",
    #endif
        reinterpret_cast<void*>(LevelInfoLayer_onLevelInfo),
        reinterpret_cast<void**>(&TRAM_LevelInfoLayer_onLevelInfo));
#endif
#if GAME_VERSION >= GV_1_7
    Omni::hook("_ZN14LevelInfoLayer21levelDownloadFinishedEP11GJGameLevel",
        reinterpret_cast<void*>(LevelInfoLayer_levelDownloadFinished),
        reinterpret_cast<void**>(&TRAM_LevelInfoLayer_levelDownloadFinished));
#endif
}