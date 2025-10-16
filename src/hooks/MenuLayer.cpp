#include "hook.hpp"
#include "MenuLayer.hpp"
#include "FLAlertLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "../layers/HaxLayer.hpp"

void MenuLayer_onMoreGames(void* self) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, HaxLayer::scene(false)));
}
void MenuLayer::onMenuInfo() {
    FLAlertLayer::create(
        nullptr,
        "Game Information",
        CCString::createWithFormat(
            "<cg>OMNImenu</c> %s\n<cl>Geometry Dash</c> %s\n<cr>Special Thanks</c>: <cy>akqanile</c>, <cy>Hris69</c>, <cy>Pololak</c>, <cy>Nikolyas</c>, <cy>Capeling</c>, <cy>Cvolton</c>, <cy>dank_meme01</c>, <cy>prevter</c>, <cy>Thelazycat</c>\nWith love from <cy>AntiMatter</c> <cr><3</c>", 
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

    self->addChild(infoMenu);
    infoMenu->addChild(infoBtn);
    infoMenu->setPosition(ccp(winSize.width - 25.f, winSize.height - 25.f));

    return true;
}

void MenuLayer_om() {
    Omni::hook("_ZN9MenuLayer11onMoreGamesEv",
        reinterpret_cast<void*>(MenuLayer_onMoreGames),
        nullptr);
    Omni::hook("_ZN9MenuLayer4initEv",
        reinterpret_cast<void*>(MenuLayer_init),
        reinterpret_cast<void**>(&TRAM_MenuLayer_init));
}