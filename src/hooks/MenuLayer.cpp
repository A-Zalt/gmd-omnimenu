#include "hook.hpp"
#include "MenuLayer.hpp"
#include "FLAlertLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
// #include "../layers/HaxLayer.hpp"
// #include "../layers/HaxOverlay.hpp"
#include "../layers/HaxMenu.hpp"
#include "HaxManager.hpp"
#include "Utils.hpp"
#if GAME_VERSION >= GV_1_5
#include "AppDelegate.hpp"
#endif
#if GDPS == GDPS_NEOPOINTFOUR
#include "Neopointfour/StatsRecalc.cpp"
//#include <thread>
#endif

// void MenuLayer_onMoreGames(void* self) {
//     CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, HaxLayer::scene(false)));
// }
void MenuLayer::onOpenMenu(SEL_MenuHandler_1_7_compat) {
    auto haxOverlay = HaxMenu::create(this); //HaxOverlay::create(this);
    this->addChild(haxOverlay, 1000);
    this->setTouchEnabled(false);
}
void MenuLayer::onMenuInfo(SEL_MenuHandler_1_7_compat) {
    FLAlertLayer::create(
        nullptr,
        "Game Information",
        fmt::format(
#if GDPS == GDPS_NEOPOINTFOUR
            "<cg>OMNImenu</c> {}\n<cl>Geometry Dash</c> {}\n<cr>Special Thanks</c>: <cy>akqanile</c>, <cy>Hris69</c>, <cy>Pololak</c>, <cy>Nikolyas</c>, <cy>Capeling</c>, <cy>Cvolton</c>, <cy>dank_meme01</c>, <cy>prevter</c>, <cy>Thelazycat</c>, <cy>HJFod</c>, <cy>iAndyHD_3</c>\nWith love from <cy>AntiMatter</c>, <cy>RandomB</c> and <cy>nano</c> <cr><3</c>", 
#else
            "<cg>OMNImenu</c> {}\n<cl>Geometry Dash</c> {}\n<cr>Special Thanks</c>: <cy>akqanile</c>, <cy>Hris69</c>, <cy>Pololak</c>, <cy>Nikolyas</c>, <cy>RandomB</c>, <cy>Capeling</c>, <cy>Cvolton</c>, <cy>dank_meme01</c>, <cy>prevter</c>, <cy>Thelazycat</c>, <cy>HJFod</c>, <cy>iAndyHD_3</c>\nWith love from <cy>AntiMatter</c> and <cy>nano</c> <cr><3</c>", 
#endif
            MENU_VERSION, READABLE_GAME_VERSION_FULL).c_str(),
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

    if (hax.getModuleEnabled(ModuleID::FLOATING_ICON) && !hax.omniMenu) {   
        CCLog("creating haxbutton");     
        hax.omniMenu = HaxButton::create();
        hax.omniMenu->retain();
        hax.omniMenu->setup();
        #if GAME_VERSION == GV_1_7
        self->addChild(hax.omniMenu->m_sMenu);
        #endif
    } else if (!hax.getModuleEnabled(ModuleID::FLOATING_ICON)) {
        CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(menuSpr, menuSpr, self, menu_selector(MenuLayer::onOpenMenu));
    #ifndef STEALTH_MODE 
        bottomMenu->addChild(menuBtn);
        // menuBtn->setPosition(ccp(-130, -winSize.height + 70.f));
        bottomMenu->alignItemsHorizontallyWithPadding(5.0f);
    #else
        infoBtn->setVisible(false);
        bottomMenu = CCMenu::create();
        bottomMenu->addChild(menuBtn);
        self->addChild(bottomMenu);
        bottomMenu->setPosition(ccp(50, winSize.height - 50));
        menuBtn->setOpacity(0);
    #endif
    }

    #if GDPS == GDPS_NEOPOINTFOUR
    if (!hasRecalculated) {
        hasRecalculated = true;
        recalculateStars();
    }
    #endif

    return true;
}

void (*TRAM_MenuLayer_FLAlert_Clicked)(CCLayer* self, FLAlertLayer* flAlert, bool a3);
void MenuLayer_FLAlert_Clicked(CCLayer* self, FLAlertLayer* flAlert, bool a3) {
    auto& hax = HaxManager::sharedState();
    if (a3 && hax.getModuleEnabled(ModuleID::AUTO_BACKUP) && flAlert->getTag() == 0) {
        hax.createBackup();
    }
#if GAME_VERSION < GV_1_5
    TRAM_MenuLayer_FLAlert_Clicked(self, flAlert, a3);
#else
    if (a3 && flAlert->getTag() == 0) {
        AppDelegate::get()->trySaveGame();
        self->getActionManager()->addAction(
            CCSequence::create(
                CCDelayTime::create(0.5), 
                CCCallFunc::create(self, callfunc_selector(MenuLayer::endGame)),
                nullptr
            ),
            self, false
        );
    }
#endif
}

void MenuLayer_om() {
    // Omni::hook("_ZN9MenuLayer11onMoreGamesEv",
    //     reinterpret_cast<void*>(MenuLayer_onMoreGames),
    //     nullptr);
    Omni::hook("_ZN9MenuLayer4initEv",
        reinterpret_cast<void*>(MenuLayer_init),
        reinterpret_cast<void**>(&TRAM_MenuLayer_init));
    Omni::hook("_ZN9MenuLayer15FLAlert_ClickedEP12FLAlertLayerb",
        reinterpret_cast<void*>(MenuLayer_FLAlert_Clicked),
        reinterpret_cast<void**>(&TRAM_MenuLayer_FLAlert_Clicked));
}