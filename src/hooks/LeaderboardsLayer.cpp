#include "hook.hpp"
#include "LeaderboardsLayer.hpp"

void LeaderboardsLayer::onRefresh() {
    auto state = getLeaderboardState(this);
    switch (state) {
        case LeaderboardState::Top:
            GameLevelManager::sharedState()->resetTimerForKey("leaderboard_top");
#if GDPS == GDPS_NEOPOINTFOUR
            GameLevelManager::sharedState()->resetTimerForKey("leaderboard_top_demons");
#endif
            break;
        case LeaderboardState::Global:
            GameLevelManager::sharedState()->resetTimerForKey("leaderboard_global");
#if GDPS == GDPS_NEOPOINTFOUR
            GameLevelManager::sharedState()->resetTimerForKey("leaderboard_global_demons");
#endif
            break;
        case LeaderboardState::Creators:
            GameLevelManager::sharedState()->resetTimerForKey("leaderboard_creator");
            break;
#if GAME_VERSION >= GV_1_6
        case LeaderboardState::Week:
            GameLevelManager::sharedState()->resetTimerForKey("leaderboard_week");
            break;
#endif
    }

#if GDPS == GDPS_NEOPOINTFOUR
    CCMenu* statMenu = nullptr;
    CCObject* child;
    CCARRAY_FOREACH(this->getChildren(), child) {
        CCMenu* menu = dynamic_cast<CCMenu*>(child);
        if (menu && menu->getChildByTag(9001)) {
            statMenu = menu;
            break;
        }
    }
    
    if (statMenu) {
        statMenu->setVisible(state != LeaderboardState::Creators);
    }
#endif
    setLeaderboardState(this, 5); // if state is the same as current, selectLeaderboard will not run
    this->selectLeaderboard(state);
}
#if GDPS == GDPS_NEOPOINTFOUR
void LeaderboardsLayer::onStarFilter(CCObject* sender) {
    auto GLM = GameLevelManager::sharedState();
    bool currentFilter = GLM->getBoolForKey("demon_filter");
    
    if (currentFilter) {
        GLM->setBoolForKey(false, "demon_filter");
        this->updateFilterButtons();
        this->onRefresh();
    }
}

void LeaderboardsLayer::onDemonFilter(CCObject* sender) {
    auto GLM = GameLevelManager::sharedState();
    bool currentFilter = GLM->getBoolForKey("demon_filter");
    
    if (!currentFilter) {
        GLM->setBoolForKey(true, "demon_filter");
        this->updateFilterButtons();
        this->onRefresh();
    }
}

void LeaderboardsLayer::updateFilterButtons() { 
    auto GLM = GameLevelManager::sharedState();
    bool demonFilter = GLM->getBoolForKey("demon_filter");
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    
    CCMenu* statMenu = nullptr;
    CCObject* child;
    CCARRAY_FOREACH(this->getChildren(), child) {
        CCMenu* menu = dynamic_cast<CCMenu*>(child);
        if (menu && menu->getChildByTag(9001)) {
            statMenu = menu;
            break;
        }
    }
    
    if (!statMenu) return;
    
    statMenu->removeChildByTag(9001, true);
    statMenu->removeChildByTag(9002, true);

    // listen I know this looks terrible but it would just refuse to work properly if I didn't remake the entire menu -,-
    
    CCSprite* starIcon = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
    ButtonSprite* starBtnSprite = ButtonSprite::create(
        starIcon,
        30,
        0,
        60.0f,
        1.0f,
        true,
        demonFilter ? "GJ_button_01.png" : "GJ_button_02.png"
    );
    starBtnSprite->setScale(0.75f);
    CCMenuItemSpriteExtra* starBtn = CCMenuItemSpriteExtra::create(
        starBtnSprite,
        starBtnSprite,
        this,
        menu_selector(LeaderboardsLayer::onStarFilter)
    );
    starBtn->setTag(9001);
    starBtn->setPosition(ccp(0.f, 20.f));
    
    CCSprite* demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
    ButtonSprite* demonBtnSprite = ButtonSprite::create(
        demonIcon,
        30,
        0,
        60.0f,
        1.0f,
        true,
        demonFilter ? "GJ_button_02.png" : "GJ_button_01.png"
    );
    demonBtnSprite->setScale(0.75f);
    CCMenuItemSpriteExtra* demonBtn = CCMenuItemSpriteExtra::create(
        demonBtnSprite,
        demonBtnSprite,
        this,
        menu_selector(LeaderboardsLayer::onDemonFilter)
    );
    demonBtn->setTag(9002);
    demonBtn->setPosition(ccp(0.f, -20.f));
    
    statMenu->addChild(starBtn);
    statMenu->addChild(demonBtn);
    
    statMenu->setPosition(ccp(winSize.width - 30.f, winSize.height / 2.f + 30.f));
}
void (*TRAM_LeaderboardsLayer_selectLeaderboard)(LeaderboardsLayer* self, LeaderboardState state);
void LeaderboardsLayer_selectLeaderboard(LeaderboardsLayer* self, LeaderboardState state) {
    TRAM_LeaderboardsLayer_selectLeaderboard(self, state);
    CCMenu* statMenu = nullptr;
    CCObject* child;
    CCARRAY_FOREACH(self->getChildren(), child) {
        CCMenu* menu = dynamic_cast<CCMenu*>(child);
        if (menu && menu->getChildByTag(9001)) {
            statMenu = menu;
            break;
        }
    }
    
    if (statMenu) {
        statMenu->setVisible(state != LeaderboardState::Creators);
    }
}
#endif

bool (*TRAM_LeaderboardsLayer_init)(CCLayer* self, int state);
bool LeaderboardsLayer_init(CCLayer* self, int state) {
    HaxManager& hax = HaxManager::sharedState();
    if (!TRAM_LeaderboardsLayer_init(self, state)) return false;
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    if (hax.getModuleEnabled(ModuleID::PAGE_REFRESH)) {
        CCMenu* refreshMenu = CCMenu::create();
        CCSprite* refreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
        CCMenuItemSpriteExtra* refreshBtn = CCMenuItemSpriteExtra::create(refreshSpr, refreshSpr, self, menu_selector(LeaderboardsLayer::onRefresh));

        self->addChild(refreshMenu, 1000);
        refreshMenu->addChild(refreshBtn);
        refreshMenu->setPosition(ccp(winSize.width - 30.f, 30.f));
    }

#if GDPS == GDPS_NEOPOINTFOUR
    auto GLM = GameLevelManager::sharedState();
    bool demonFilter = GLM->getBoolForKey("demon_filter");
    
    CCMenu* statMenu = CCMenu::create();
    self->addChild(statMenu, 1000);
    
    CCSprite* starIcon = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
    ButtonSprite* starBtnSprite = ButtonSprite::create(
        starIcon,          // topSprite
        30,              // width
        0,                 // minWidth
        60.0f,              // height
        1.0f,              // scale
        true,             // absolute
        demonFilter ? "GJ_button_01.png" : "GJ_button_02.png"  // bgSprite
    );
    starBtnSprite->setScale(0.75f);
    CCMenuItemSpriteExtra* starBtn = CCMenuItemSpriteExtra::create(
        starBtnSprite,
        starBtnSprite,
        self,
        menu_selector(LeaderboardsLayer::onStarFilter)
    );
    starBtn->setTag(9001);
    
    CCSprite* demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
    ButtonSprite* demonBtnSprite = ButtonSprite::create(
        demonIcon,         // topSprite
        30,              // width
        0,                 // minWidth
        60.0f,              // height
        1.0f,              // scale
        true,             // absolute
        demonFilter ? "GJ_button_02.png" : "GJ_button_01.png"  // bgSprite
    );
    demonBtnSprite->setScale(0.75f);
    CCMenuItemSpriteExtra* demonBtn = CCMenuItemSpriteExtra::create(
        demonBtnSprite,
        demonBtnSprite,
        self,
        menu_selector(LeaderboardsLayer::onDemonFilter)
    );
    demonBtn->setTag(9002);
    
    statMenu->addChild(starBtn);
    statMenu->addChild(demonBtn);
    statMenu->setPosition(ccp(winSize.width - 30.f, winSize.height / 2.f + 30.f));
    
    starBtn->setPosition(ccp(0.f, 20.f));
    demonBtn->setPosition(ccp(0.f, -20.f));
    if (state == 3) {
        statMenu->setVisible(false);
    }
#endif

    return true;
}

void LeaderboardsLayer_om() {
    Omni::hook("_ZN17LeaderboardsLayer4initE16LeaderboardState",
        reinterpret_cast<void*>(LeaderboardsLayer_init),
        reinterpret_cast<void**>(&TRAM_LeaderboardsLayer_init));
#if GDPS == GDPS_NEOPOINTFOUR
    Omni::hook("_ZN17LeaderboardsLayer17selectLeaderboardE16LeaderboardState",
        reinterpret_cast<void*>(LeaderboardsLayer_selectLeaderboard),
        reinterpret_cast<void**>(&TRAM_LeaderboardsLayer_selectLeaderboard));
#endif
}