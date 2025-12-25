#include "hook.hpp"
#include "LeaderboardsLayer.hpp"

void LeaderboardsLayer::onRefresh() {
    auto state = getLeaderboardState(this);
    switch (state) {
        case LeaderboardState::Top:
            GameLevelManager::sharedState()->resetTimerForKey("leaderboard_top");
            break;
        case LeaderboardState::Global:
            GameLevelManager::sharedState()->resetTimerForKey("leaderboard_global");
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
    setLeaderboardState(this, 5); // if state is the same as current, selectLeaderboard will not run
    this->selectLeaderboard(state);
}

bool (*TRAM_LeaderboardsLayer_init)(CCLayer* self, int state);
bool LeaderboardsLayer_init(CCLayer* self, int state) {
    HaxManager& hax = HaxManager::sharedState();
    if (!TRAM_LeaderboardsLayer_init(self, state)) return false;
    if (hax.getModuleEnabled(ModuleID::PAGE_REFRESH)) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        CCMenu* refreshMenu = CCMenu::create();
        CCSprite* refreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
        CCMenuItemSpriteExtra* refreshBtn = CCMenuItemSpriteExtra::create(refreshSpr, refreshSpr, self, menu_selector(LeaderboardsLayer::onRefresh));

        self->addChild(refreshMenu, 1000);
        refreshMenu->addChild(refreshBtn);
        refreshMenu->setPosition(ccp(winSize.width - 30.f, 30.f));
    }
    return true;
}

void LeaderboardsLayer_om() {
    Omni::hook("_ZN17LeaderboardsLayer4initE16LeaderboardState",
        reinterpret_cast<void*>(LeaderboardsLayer_init),
        reinterpret_cast<void**>(&TRAM_LeaderboardsLayer_init));
}