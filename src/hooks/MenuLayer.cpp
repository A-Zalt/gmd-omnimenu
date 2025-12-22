#include "hook.hpp"
#include "MenuLayer.hpp"
#include "FLAlertLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
// #include "../layers/HaxLayer.hpp"
// #include "../layers/HaxOverlay.hpp"
#include "../layers/HaxMenu.hpp"
#include "HaxManager.hpp"
#include "Utils.hpp"

// void MenuLayer_onMoreGames(void* self) {
//     CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, HaxLayer::scene(false)));
// }
void MenuLayer::onOpenMenu(SEL_MenuHandler_1_7_compat) {
    auto haxOverlay = HaxMenu::create(this); //HaxOverlay::create(this);
    this->addChild(haxOverlay, 1000);
    this->setTouchEnabled(false);
}
void MenuLayer::onMenuInfo(SEL_MenuHandler_1_7_compat) {
    CCLog("say something");
    FLAlertLayer::create(
        FLAlertLayer_1_7_compat_this,
        "Game Information",
        CCString::createWithFormat(
            "<cg>OMNImenu</c> %s\n<cl>Geometry Dash</c> %s\n<cr>Special Thanks</c>: <cy>akqanile</c>, <cy>Hris69</c>, <cy>Pololak</c>, <cy>Nikolyas</c>, <cy>Capeling</c>, <cy>Cvolton</c>, <cy>dank_meme01</c>, <cy>prevter</c>, <cy>Thelazycat</c>, <cy>HJFod</c>, <cy>iAndyHD_3</c>, <cy>JJ Reed</c>\nWith love from <cy>AntiMatter</c> <cr><3</c>", 
            MENU_VERSION, READABLE_GAME_VERSION_FULL)->getCString(),
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
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.hasInitialized) hax.loadSettingsFromFile();

    CCMenu* bottomMenu = static_cast<CCMenu*>(self->getChildren()->objectAtIndex(self->getChildrenCount() - 3));

    CCMenu* infoMenu = CCMenu::create();
    CCSprite* infoSpr = createInfoSprite();
    CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, self, menu_selector(MenuLayer::onMenuInfo));
    infoBtn->setSizeMult(1.5f);

    self->addChild(infoMenu);
    infoMenu->addChild(infoBtn);
    infoMenu->setPosition(ccp(winSize.width - 25.f, winSize.height - 25.f));

    CCSprite* menuSpr = CCSprite::create("OMNImenu_btn.png");
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(menuSpr, menuSpr, self, menu_selector(MenuLayer::onOpenMenu));
    bottomMenu->addChild(menuBtn);
    // menuBtn->setPosition(ccp(-130, -winSize.height + 70.f));
    bottomMenu->alignItemsHorizontallyWithPadding(5.0f);

    return true;
}

void MenuLayer_om() {
    // Omni::hook("_ZN9MenuLayer11onMoreGamesEv",
    //     reinterpret_cast<void*>(MenuLayer_onMoreGames),
    //     nullptr);
    Omni::hook("_ZN9MenuLayer4initEv",
        reinterpret_cast<void*>(MenuLayer_init),
        reinterpret_cast<void**>(&TRAM_MenuLayer_init));
}