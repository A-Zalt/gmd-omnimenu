#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "GameStatsManager.hpp"

bool (*TRAM_GJGarageLayer_init)(CCLayer* self);
bool GJGarageLayer_init(CCLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
#if GAME_VERSION >= GV_1_4
    setIconHack(hax.getModuleEnabled(ModuleID::UNLOCK_ICONS));
#endif
    if (!TRAM_GJGarageLayer_init(self)) return false;
    if (hax.getModuleEnabled(ModuleID::DEMONS_IN_GARAGE)) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
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