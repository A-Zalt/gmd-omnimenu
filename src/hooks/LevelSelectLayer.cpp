#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "FLAlertLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "LevelSelectLayer.hpp"
#include "LevelTools.hpp"
#include "GameLevelManager.hpp"
#include "Utils.hpp"

void showRandomMessage() {
    GameManager* gman = GameManager::sharedState();
    std::vector<std::string> messages = {
        "When them <co>fajitas</c> come out <cy>sizzlin'</c>!",
        "kitto todoku hazu",
        "I loooove <co>GD Cologne</c>!",
        "Another Time Then...",
        "<cl>And we Will End!!</c>",
        fmt::format("Hello <cy>{}</c>. You don't know me, but I know you. I want to play a <cr>game</c>.", getPlayerName()),
        "no worries I will load a backup so people can restore levels they want via account page. May take a while",
        "A wild <cy>Prew Davlou</c> appeared!",
        "i had a dream that the united nations recognized zoink as an independent country",
        "Guys, PLEASE <cr>STOP</c> calling Me <cy>Greg Heffley</c>. I'm more than just a <cg>STUPID GRAPHIC NOVEL CHARACTER</c>!!! Guys, Look at <cl>the bigger picture</c> here.",
        "Please do not disrespect other members.",
        "The person who implemented <cl>SimpleAudioEngine</c> should be considered a <cr>war criminal</c>.",
        "The family-friendly Christian Geometry Dash mod menu!",
        "NEVER INHERIT <cb>FLAlertLayer</c>, WORST MISTAKE OF MY LIFE!",
        "it's just simple <cr>red</c> counting",
        "i know what you did last night",
        "Beautiful risings <cy>Pharoah</c>. My temple is calling for its <cy>Pharoah</c>.",
        "Dude, I know this is unrelated, but I need your help right now.", 
        fmt::format("IS THIS A UNDERRATED <cy>{}</c> MASTERPIECE!?", READABLE_GAME_VERSION),
        "I'm literally older than yo Brainrot Ahhh.",
        "xD:Call me ...Ring...Ring... hello...What?! ok :D bye mlg",
        "The community in this game does there difficulty rating sonhorrible",
        "I got the coin and then i went to the",
        "I Was happy as hell when I found out stringstream was dead, I cooked for the whole day",
        "Shoutout roadkiller",
        "I still miss featured levels with no stars",
        "I still miss unrated autos",
        "Do you feel blame? Are you mad?",
        "You caught a Sophie <cr><3</c>",
        "You caught a Sophie <co><3</c>",
        "You caught a Sophie <cy><3</c>",
        "You caught a Sophie <cg><3</c>",
        "You caught a Sophie <cl><3</c>",
        "You caught a Sophie <cb><3</c>",
        "You caguht a Sophie <3",
#if GAME_VERSION >= GV_1_6 // pink color only on 1.6
        "You caught a Sophie <cp><3</c>",
#endif
#if GAME_VERSION < GV_1_8
        "1.8 when"
#else
        "1.8 <cp>NOW</c>"
#endif
    };
    #if GDPS == GDPS_NEOPOINTFOUR
    messages.push_back("<cy>Active</c>\n<cg>Total Attempts</c>: 0\n<cr>Normal</c>: 0%\n<co>Practice</c>: 0%\n<cb>Level ID</c>: ?");
    #endif
    int index = rand() % messages.size();
    FLAlertLayer::create(
        nullptr,
        "Level Info",
        messages[index].c_str(),
        "OK",
        nullptr,
        300.f
    )->show();
}
#if GAME_VERSION >= GV_1_6
void (*TRAM_LevelSelectLayer_onInfo)(LevelSelectLayer* self SEL_MenuHandler_1_7_compat2);
void LevelSelectLayer_onInfo(LevelSelectLayer* self SEL_MenuHandler_1_7_compat2) {
    int levelID = getCurrentScrollIndex(getSelectLayerScroll(self)) + 1;
    HaxManager& hax = HaxManager::sharedState();
    if (levelID > LAST_MAIN_LEVEL_ID && hax.getModuleEnabled(ModuleID::VIEW_LEVEL_STATS)) {
        showRandomMessage();
        return;
    }
    TRAM_LevelSelectLayer_onInfo(self sender_param_1_7);
}
#else
void LevelSelectLayer::onViewLevelInfo() {
    // GJGameLevel* level = getInfoLayerLevel(this);
    int levelID = getCurrentScrollIndex(getSelectLayerScroll(this)) + 1;
    if (levelID > LAST_MAIN_LEVEL_ID) {
        showRandomMessage();
        return;
    }
    GameLevelManager* GLM = GameLevelManager::sharedState();
    GJGameLevel* level = GLM->getMainLevel(levelID); 
    auto flAlertInsides = fmt::format(
        "<cy>{}</c>\n<cg>Total Attempts</c>: {}\n<cr>Normal</c>: {}%\n<co>Practice</c>: {}%\n<cb>Level ID</c>: {}",
        level->m_sLevelName.c_str(),
        level->m_nAttempts,
        level->m_nNormalPercent,
        level->m_nPracticePercent,
        levelID
    );
    FLAlertLayer::create(
        nullptr,
        "Level Info",
        flAlertInsides.c_str(),
        "OK",
        nullptr,
        300.f
    )->show();
}

bool (*TRAM_LevelSelectLayer_init)(LevelSelectLayer* self, int a);
bool LevelSelectLayer_init(LevelSelectLayer* self, int a) {
    TRAM_LevelSelectLayer_init(self, a);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::VIEW_LEVEL_STATS)) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        CCMenu* infoMenu = CCMenu::create();
        CCSprite* infoSpr = createInfoSprite();
        CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, self, menu_selector(LevelSelectLayer::onViewLevelInfo));

        infoBtn->setSizeMult(1.5f);

        self->addChild(infoMenu, 1000);
        infoMenu->addChild(infoBtn);
        infoMenu->setPosition(ccp(winSize.width - 20, winSize.height - 20));
    }
    return true;
}
#endif

void LevelSelectLayer_om() {
#if GAME_VERSION < GV_1_6
    Omni::hook("_ZN16LevelSelectLayer4initEi",
        reinterpret_cast<void*>(LevelSelectLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelSelectLayer_init));
#else
    Omni::hook(
#if GAME_VERSION < GV_1_7
        "_ZN16LevelSelectLayer6onInfoEv",
#else
        "_ZN16LevelSelectLayer6onInfoEPN7cocos2d8CCObjectE",
#endif
        reinterpret_cast<void*>(LevelSelectLayer_onInfo),
        reinterpret_cast<void**>(&TRAM_LevelSelectLayer_onInfo));
#endif
}