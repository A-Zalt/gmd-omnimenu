#include "hook.hpp"
#include "EndLevelLayer.hpp"

bool visible = true;
CCMenu* g_menu;

void EndLevelLayer::toggleVisibility() {
    visible = !visible;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    for (int i = 0; i < getChildrenCount(); i++) {
        CCNode* child = static_cast<CCNode*>(getChildren()->objectAtIndex(i));

        if (child == g_menu)
            continue;

        child->runAction(CCEaseInOut::create(CCMoveTo::create(1.0, visible ? ccp(0, 0) : ccp(0, winSize.height)), 2.0));
    }
    this->runAction(CCFadeTo::create(1.0f, visible ? 100 : 0));
}

static EndLevelLayer* (*TRAM_EndLevelLayer_create)();
static EndLevelLayer* EndLevelLayer_create() {
    auto layer = TRAM_EndLevelLayer_create();
    if (!layer) return nullptr;
    HaxManager& hax = HaxManager::sharedState();
    visible = true;
    if (hax.getModuleEnabled(ModuleID::HIDE_END_SCREEN)) {
        auto menu = CCMenu::create();
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        menu->setPosition(ccp(winSize.width - 25, winSize.height - 25));
        auto eyeSpr = CCSprite::create("hideBtn_001.png");
        auto eye = CCMenuItemSpriteExtra::create(eyeSpr, eyeSpr, layer, menu_selector(EndLevelLayer::toggleVisibility));
        eye->setOpacity(127);
        eye->setTag(1);
        eye->setSizeMult(1.5);
        menu->addChild(eye);
        layer->addChild(menu);
        g_menu = menu;
    }
    return layer;
}

void EndLevelLayer_om() {
    Omni::hook("_ZN13EndLevelLayer6createEv",
        reinterpret_cast<void*>(EndLevelLayer_create),
        reinterpret_cast<void**>(&TRAM_EndLevelLayer_create));
}