#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "GameStatsManager.hpp"
#ifdef EXTRA_COLORS
#include "GameToolbox.hpp"
#endif

bool (*TRAM_GJGarageLayer_init)(CCLayer* self);
bool GJGarageLayer_init(CCLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
#if GAME_VERSION >= GV_1_4
    setIconHack(hax.getModuleEnabled(ModuleID::UNLOCK_ICONS));
#endif
    if (!TRAM_GJGarageLayer_init(self)) return false;
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
#if defined(EXTRA_COLORS)
    int firstMenu = true;
    auto colSel1 = MEMBER_BY_OFFSET(CCSprite*, self, 0x150);
    auto colSel2 = MEMBER_BY_OFFSET(CCSprite*, self, 0x154);
#if GAME_VERSION == GV_1_4
    float cutoff = 18;
    float yValue = MEMBER_BY_OFFSET(float, self, 0x168) - winSize.height / 2;
    int col1 = MEMBER_BY_OFFSET(int, GameManager::sharedState(), 0x188);
    int col2 = MEMBER_BY_OFFSET(int, GameManager::sharedState(), 0x18c);
#else
    float cutoff = 17;
    float yValue = MEMBER_BY_OFFSET(float, self, 0x160) - winSize.height / 2;
    int col1 = MEMBER_BY_OFFSET(int, GameManager::sharedState(), 0x184);
    int col2 = MEMBER_BY_OFFSET(int, GameManager::sharedState(), 0x188);
#endif
    if (col1 >= cutoff) col1 = cutoff - 1;
    if (col2 >= cutoff) col2 = cutoff - 1;
    if (col1 < 0) col1 = 0;
    if (col2 < 0) col2 = 0;
    colSel1->setScale(0.8);
    colSel2->setScale(0.8);
    CCMenuItemSpriteExtra* btn1;
    CCMenuItemSpriteExtra* btn2;
    CCMenu* colorMenu;
    for (int i = 0; i < self->getChildrenCount(); i++) {
        auto child = self->getChildren()->objectAtIndex(i);
        auto menu = dynamic_cast<CCMenu*>(child);
        if (!menu) continue;
        if (firstMenu) {
            firstMenu = false;
            continue;
        }
        auto arr1 = CCArray::create();
        auto arr2 = CCArray::create();
        colorMenu = menu;
        for (int j = 0; j < menu->getChildrenCount(); j++) {
            auto child2 = menu->getChildren()->objectAtIndex(j);
            auto btn = dynamic_cast<CCMenuItemSpriteExtra*>(child2);
            if (!btn) continue;
            btn->getNormalImage()->setScale(0.8);
            if (j < cutoff) {
                if (j == col1) btn1 = btn;
                arr1->addObject(btn);
            } else {
                if (j == (col2 + cutoff)) btn2 = btn;
                arr2->addObject(btn);
            }
        }
        GameToolbox::alignItemsHorisontally(arr1, -1, ccp(0, yValue - 25));
        GameToolbox::alignItemsHorisontally(arr2, -1, ccp(0, yValue - 62));
        colSel1->setPosition(self->convertToNodeSpace(colorMenu->convertToWorldSpace(btn1->getPosition())));
        colSel2->setPosition(self->convertToNodeSpace(colorMenu->convertToWorldSpace(btn2->getPosition())));
        break;
    }
#endif
    if (hax.getModuleEnabled(ModuleID::DEMONS_IN_GARAGE)) {
        auto sprite = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
        sprite->setScale(0.8f);
        auto label = CCLabelBMFont::create(fmt::format("{}", GameStatsManager::sharedState()->getStat("5")).c_str(), "bigFont.fnt");
        label->setScale(0.5f);
        label->setAnchorPoint({1, 0.5});
#if GAME_VERSION < GV_1_6
        sprite->setPosition(ccp(winSize.width - 40, winSize.height - 65));
        label->setPosition(ccp(winSize.width - 58, winSize.height - 65));
#else
        sprite->setPosition(ccp(winSize.width - 40, winSize.height - 92));
        label->setPosition(ccp(winSize.width - 58, winSize.height - 92));
#endif
        self->addChild(sprite);
        self->addChild(label);
    }
    return true;
}

void GJGarageLayer_om() {
    Omni::hook("_ZN13GJGarageLayer4initEv",
        reinterpret_cast<void*>(GJGarageLayer_init),
        reinterpret_cast<void**>(&TRAM_GJGarageLayer_init));
}