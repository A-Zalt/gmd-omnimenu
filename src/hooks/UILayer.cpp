#include "hook.hpp"
#include "UILayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "ButtonSprite.hpp"
#include "PlayerObject.hpp"

void UILayer::createCheatIndicator() {
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto cheatIndicatorLabel = CCLabelBMFont::create(".", "bigFont.fnt");
    cheatIndicatorLabel->setPosition(ccp(15, winSize.height - 10));
    switch (hax.getCheatIndicatorColor()) {
        case CheatIndicatorColor::Green:
            cheatIndicatorLabel->setColor(ccGREEN);
            break;
        case CheatIndicatorColor::Yellow:
            cheatIndicatorLabel->setColor(ccYELLOW);
            break;
        case CheatIndicatorColor::Orange:
            cheatIndicatorLabel->setColor(ccORANGE);
            break;
        case CheatIndicatorColor::Red:
            cheatIndicatorLabel->setColor(ccRED);
            break;
        default:
            cheatIndicatorLabel->setColor(ccWHITE);
            break;
    };
    hax.cheatIndicatorLabel = cheatIndicatorLabel;
    addChild(cheatIndicatorLabel, 10000);
}

void UILayer::createPercentageLabel() {
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto percentageLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
    percentageLabel->setPosition(ccp(winSize.width / 2, winSize.height - 10));
    percentageLabel->setScale(0.5f);
    hax.percentageLabel = percentageLabel;
    addChild(percentageLabel, 10000);
}

CCMenuItemSpriteExtra* createPCButton(const char* label, float y, SEL_MenuHandler selector, UILayer* self, CCMenu* menu) {
    auto btnSpr1 = ButtonSprite::create(label, 20);
    btnSpr1->setScale(1.0f);

    auto btn1 = CCMenuItemSpriteExtra::create(btnSpr1, btnSpr1, self, selector);
    btn1->setPosition(ccp(0, y));
    menu->addChild(btn1);
    return btn1;
}
void UILayer::createPCommand() {
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    if (!hax.pMenu || hax.pMenu == nullptr) {
        auto menu = CCMenu::create();
        menu->setPosition(ccp(winSize.width - 30, winSize.height - 40));
        this->addChild(menu, 10000);
        hax.pMenu = menu;
    }
    if (!hax.pButton1 || hax.pButton1 == nullptr) 
        hax.pButton1 = createPCButton("S+", -55, menu_selector(UILayer::speedUp), this, hax.pMenu);
    if (!hax.pButton2 || hax.pButton2 == nullptr) 
        hax.pButton2 = createPCButton("S-", -85, menu_selector(UILayer::speedDown), this, hax.pMenu);
    if (!hax.pButton3 || hax.pButton3 == nullptr) 
        hax.pButton3 = createPCButton("G+", -115, menu_selector(UILayer::gravityUp), this, hax.pMenu);
    if (!hax.pButton4 || hax.pButton4 == nullptr) 
        hax.pButton4 = createPCButton("G-", -145, menu_selector(UILayer::gravityDown), this, hax.pMenu);
    if (!hax.pButton5 || hax.pButton5 == nullptr) 
        hax.pButton5 = createPCButton("Y+", -175, menu_selector(UILayer::yStartUp), this, hax.pMenu);
    if (!hax.pButton6 || hax.pButton6 == nullptr) 
        hax.pButton6 = createPCButton("Y-", -205, menu_selector(UILayer::yStartDown), this, hax.pMenu);
}

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
    if (hax.getModuleEnabled("cheat_indicator")) {
        self->createCheatIndicator();
    }
    if (hax.getModuleEnabled("show_percentage")) {
        self->createPercentageLabel();
    }
    if (hax.getModuleEnabled("pcommand")) {
        self->createPCommand();
    }
    return true;
}
void (*TRAM_UILayer_destructor)(UILayer* self);
void UILayer_destructor(UILayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    TRAM_UILayer_destructor(self);
    hax.cheatIndicatorLabel = nullptr;
    hax.percentageLabel = nullptr;
    hax.pButton1 = nullptr;
    hax.pButton2 = nullptr;
    hax.pButton3 = nullptr;
    hax.pButton4 = nullptr;
    hax.pButton5 = nullptr;
    hax.pButton6 = nullptr;
    hax.pMenu = nullptr;
    // CC_SAFE_DELETE(hax.cheatIndicatorLabel);
    // CC_SAFE_DELETE(hax.percentageLabel);
    // CC_SAFE_DELETE(hax.pButton1);
    // CC_SAFE_DELETE(hax.pButton2);
    // CC_SAFE_DELETE(hax.pButton3);
    // CC_SAFE_DELETE(hax.pButton4);
    // CC_SAFE_DELETE(hax.pButton5);
    // CC_SAFE_DELETE(hax.pButton6);
    // CC_SAFE_DELETE(hax.pMenu);
}


void UILayer_om() {
    Omni::hook("_ZN7UILayer4initEv",
        reinterpret_cast<void*>(UILayer_init),
        reinterpret_cast<void**>(&TRAM_UILayer_init));
    Omni::hook("_ZN7UILayerD0Ev",
        reinterpret_cast<void*>(UILayer_destructor),
        reinterpret_cast<void**>(&TRAM_UILayer_destructor));
}