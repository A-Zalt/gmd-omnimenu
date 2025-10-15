#include "hook.hpp"
#include "UILayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "ButtonSprite.hpp"
#include "PlayerObject.hpp"

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
    if (hax.cheatIndicator) {
        auto cheatIndicatorLabel = CCLabelBMFont::create(".", "bigFont.fnt");
        cheatIndicatorLabel->setPosition(ccp(15, winSize.height - 10));
        switch (hax.getCheatIndicatorColor()) {
            case CheatIndicatorColor::Green:
                cheatIndicatorLabel->setColor(ccGREEN);
                break;
            case CheatIndicatorColor::Yellow:
                cheatIndicatorLabel->setColor(ccYELLOW);
                break;
            case CheatIndicatorColor::Red:
                cheatIndicatorLabel->setColor(ccRED);
                break;
            default:
                cheatIndicatorLabel->setColor(ccWHITE);
                break;
        };
        hax.cheatIndicatorLabel = cheatIndicatorLabel;
        self->addChild(cheatIndicatorLabel, 10000);
    }
    if (hax.showPercentage) {
        auto percentageLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        percentageLabel->setPosition(ccp(winSize.width / 2, winSize.height - 10));
        percentageLabel->setScale(0.5f);
        hax.percentageLabel = percentageLabel;
        self->addChild(percentageLabel, 10000);
    }
    if (hax.pCommand) {
        auto menu = CCMenu::create();
        menu->setPosition(ccp(winSize.width - 30, winSize.height - 40));

        auto btnSpr1 = ButtonSprite::create("S+", 20);
        btnSpr1->setScale(1.0f);

        auto btn1 = CCMenuItemSpriteExtra::create(btnSpr1, btnSpr1, self, menu_selector(UILayer::speedUp));
        btn1->setPosition(ccp(0, -55));
        menu->addChild(btn1);

        auto btnSpr2 = ButtonSprite::create("S-", 20);
        btnSpr2->setScale(1.0f);

        auto btn2 = CCMenuItemSpriteExtra::create(btnSpr2, btnSpr2, self, menu_selector(UILayer::speedDown));
        btn2->setPosition(ccp(0, -85));
        menu->addChild(btn2, 10002);

        auto btnSpr3 = ButtonSprite::create("G+", 20);
        btnSpr3->setScale(1.0f);

        auto btn3 = CCMenuItemSpriteExtra::create(btnSpr3, btnSpr3, self, menu_selector(UILayer::gravityUp));
        btn3->setPosition(ccp(0, -115));
        menu->addChild(btn3, 10003);

        auto btnSpr4 = ButtonSprite::create("G-", 20);
        btnSpr4->setScale(1.0f);

        auto btn4 = CCMenuItemSpriteExtra::create(btnSpr4, btnSpr4, self, menu_selector(UILayer::gravityDown));
        btn4->setPosition(ccp(0, -145));
        menu->addChild(btn4, 10004);

        auto btnSpr5 = ButtonSprite::create("Y+", 20);
        btnSpr5->setScale(1.0f);

        auto btn5 = CCMenuItemSpriteExtra::create(btnSpr5, btnSpr5, self, menu_selector(UILayer::yStartUp));
        btn5->setPosition(ccp(0, -175));
        menu->addChild(btn5, 10005);

        auto btnSpr6 = ButtonSprite::create("Y-", 20);
        btnSpr6->setScale(1.0f);

        auto btn6 = CCMenuItemSpriteExtra::create(btnSpr6, btnSpr6, self, menu_selector(UILayer::yStartDown));
        btn6->setPosition(ccp(0, -205));
        menu->addChild(btn6, 10006);

        self->addChild(menu, 10000);
    }
    return true;
}

void UILayer_om() {
    Omni::hook("_ZN7UILayer4initEv",
        reinterpret_cast<void*>(UILayer_init),
        reinterpret_cast<void**>(&TRAM_UILayer_init));
}