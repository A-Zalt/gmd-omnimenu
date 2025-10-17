#include "hook.hpp"
#include "LevelInfoLayer.hpp"
#include "GJGameLevel.hpp"
#include "FLAlertLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"

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
    if (hax.getModuleEnabled("level_copy")) {
        CCMenu* cloneMenu = CCMenu::create();
        CCSprite* cloneSpr = cocos2d::CCSprite::create("GJ_duplicateBtn.png");
        CCMenuItemSpriteExtra* cloneBtn = CCMenuItemSpriteExtra::create(cloneSpr, cloneSpr, self, menu_selector(LevelInfoLayer::onClone));

        self->addChild(cloneMenu, 1000);
        cloneMenu->addChild(cloneBtn);
        cloneMenu->setPosition(ccp(35.f, winSize.height / 2));
        // cloneBtn->setPosition(ccp(0, winSize.height / 2 - 25));
    }
    if (hax.getModuleEnabled("view_level_stats")) {
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

void LevelInfoLayer_om() {
    Omni::hook("_ZN14LevelInfoLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(LevelInfoLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelInfoLayer_init));
}