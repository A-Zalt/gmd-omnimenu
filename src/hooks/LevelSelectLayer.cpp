#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "FLAlertLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "LevelSelectLayer.hpp"
#include "LevelTools.hpp"
#include "GameLevelManager.hpp"

void LevelSelectLayer::onViewLevelInfo() {
    // GJGameLevel* level = getInfoLayerLevel(this);
    int levelID = getCurrentScrollIndex(getSelectLayerScroll(this)) + 1;
    if (levelID > LAST_MAIN_LEVEL_ID) {
        GameManager* gman = GameManager::sharedState();
        std::vector<std::string> messages = {
            "When them <co>fajitas</c> come out <cy>sizzlin'</c>!",
            "kitto todoku hazu",
            "I loooove <co>GD Cologne</c>!",
            "Another Time Then...",
            "<cl>And we Will End!!</c>",
            std::string(CCString::createWithFormat(
                "Hello <cy>%s</c>. You don't know me, but I know you. I want to play a <cr>game</c>.",
                getPlayerName().c_str()
            )->getCString()),
            "no worries I will load a backup so people can restore levels they want via account page. May take a while",
            "A wild <cy>Prew Davlou</c> appeared!",
            "i had a dream that the united nations recognized zoink as an independent country",
            "Guys, PLEASE <cr>STOP</c> calling Me <cy>Greg Heffley</c>. I'm more than just a <cg>STUPID GRAPHIC NOVEL CHARACTER</c>!!! Guys, Look at <cl>the bigger picture</c> here.",
            "Please do not disrespect other members.",
            "The person who implemented <cl>SimpleAudioEngine</c> should be considered a <cr>war criminal</c>.",
            "The family-friendly Christian Geometry Dash mod menu!",
            "Stand with <cl>Ukr</c><cy>aine</c> and <cg>Palestine</c>!"
        };
        int index = rand() % messages.size();
        FLAlertLayer::create(
            nullptr,
            "Level Info",
            messages[index].c_str(),
            "OK",
            nullptr,
            300.f
        )->show();
        return;
    }
    GameLevelManager* GLM = GameLevelManager::sharedState();
    GJGameLevel* level = GLM->getMainLevel(levelID); 
    CCString* flAlertInsides = CCString::createWithFormat(
        "<cy>%s</c>\n<cg>Total Attempts</c>: %i\n<cr>Normal</c>: %i%%\n<co>Practice</c>: %i%%\n<cb>Level ID</c>: %i",
        getLevelName(level).c_str(),
        getLevelAttempts(level),
        getLevelNormalPercent(level),
        getLevelPracticePercent(level),
        levelID
    );
    FLAlertLayer::create(
        nullptr,
        "Level Info",
        flAlertInsides->getCString(),
        "OK",
        nullptr,
        300.f
    )->show();
}

bool (*TRAM_LevelSelectLayer_init)(LevelSelectLayer* self, int a);
bool LevelSelectLayer_init(LevelSelectLayer* self, int a) {
    TRAM_LevelSelectLayer_init(self, a);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.viewAttempts) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        CCMenu* infoMenu = CCMenu::create();
        CCSprite* infoSpr = cocos2d::CCSprite::create("GJ_infoIcon.png");
        CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, self, menu_selector(LevelSelectLayer::onViewLevelInfo));

        infoBtn->setSizeMult(1.5f);

        self->addChild(infoMenu, 1000);
        infoMenu->addChild(infoBtn);
        infoMenu->setPosition(ccp(winSize.width - 20, winSize.height - 20));
    }
    return true;
}
void LevelSelectLayer_om() {
    Omni::hook("_ZN16LevelSelectLayer4initEi",
        reinterpret_cast<void*>(LevelSelectLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelSelectLayer_init));
}