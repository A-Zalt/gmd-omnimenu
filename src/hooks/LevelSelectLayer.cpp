#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "FLAlertLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "LevelSelectLayer.hpp"
#include "LevelTools.hpp"
#include "GameLevelManager.hpp"

void LevelSelectLayer::onViewLevelInfo() {
    // GJGameLevel* level = getInfoLayerLevel(this);
    int levelID = getCurrentScrollIndex(getSelectLayerScroll(this)) + 1;
    if (levelID > LAST_MAIN_LEVEL_ID) {
        FLAlertLayer::create(
            nullptr,
            "Level Info",
            "When them <co>fajitas</c> come out <cy>sizzlin'</c>!",
            "OK",
            nullptr,
            300.f
        )->show();
        return;
    }
    GameLevelManager* GLM = GameLevelManager::sharedState();
    GJGameLevel* level = GLM->getMainLevel(levelID); 
    CCString* flAlertInsides = CCString::createWithFormat(
        "<cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cr>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cb>Level ID</c>: %i",
        getLevelName(level).c_str(),
        getLevelAttempts(level),
        getLevelNormalPercent(level),
        getLevelPracticePercent(level),
        levelID
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

bool (*TRAM_LevelSelectLayer_init)(LevelSelectLayer* self, int a);
bool LevelSelectLayer_init(LevelSelectLayer* self, int a) {
    TRAM_LevelSelectLayer_init(self, a);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.viewAttempts) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        CCMenu* infoMenu = CCMenu::create();
        CCSprite* infoSpr = cocos2d::CCSprite::create("GJ_infoIcon.png");
        CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, self, menu_selector(LevelSelectLayer::onViewLevelInfo));

        infoBtn->setSizeMult(1.5f);

        self->addChild(infoMenu, 1000);
        infoMenu->addChild(infoBtn);
        infoMenu->setPosition(ccp(winSize.width - 20, winSize.height - 20));
    }
    return true;
}
void LevelSelectLayer_om() {
    Omni::hook("_ZN16LevelSelectLayer4initEi",
        reinterpret_cast<void*>(LevelSelectLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelSelectLayer_init));
}