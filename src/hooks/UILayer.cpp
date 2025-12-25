#include "hook.hpp"
#include "UILayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "ButtonSprite.hpp"
#include "PlayerObject.hpp"
#include <string>
#include <ctime>
#include <iomanip>

void UILayer::createLabel() {
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto uiLabel = CCLabelBMFont::create("", "chatFont.fnt");
    uiLabel->setAnchorPoint({0, 1});
    uiLabel->setPosition(ccp(10, winSize.height - 23));
    uiLabel->setOpacity(127);
    uiLabel->setScale(0.5f);
    addChild(uiLabel);
    hax.uiLabel = uiLabel;
    updateLabel();
}
static time_t cachedSeconds = 0;
static tm* cachedTm;
void UILayer::updateLabel() {
    HaxManager& hax = HaxManager::sharedState();
    fmt::internal::MemoryBuffer<char, 500> buf;
    fmt::BasicWriter<char> writer(buf);
    time_t now = time(NULL);
    if (hax.getModuleEnabled(ModuleID::LABEL_CLOCK)) {
        if (cachedSeconds != now || !cachedTm) {  
            cachedTm = localtime(&now);
            cachedSeconds = now;
        }
        writer.write("{:02}:{:02}:{:02}\n", cachedTm->tm_hour, cachedTm->tm_min, cachedTm->tm_sec);
    }
    if (hax.getModuleEnabled(ModuleID::LABEL_FPS)) {
        writer.write("FPS: {}\n", hax.fps);
    }
    if (hax.getModuleEnabled(ModuleID::LABEL_ATTEMPT_COUNT_SESSION)) {
        writer.write("Attempts: {}\n", getCurrentAttempts());
    }
    if (hax.getModuleEnabled(ModuleID::LABEL_ATTEMPT_COUNT_TOTAL)) {
        writer.write("Total Attempts: {}\n", getPlayLayerLevel()->m_nAttempts);
    }
    if (hax.getModuleEnabled(ModuleID::LABEL_BEST_RUN)) {
        writer.write("Best Run: ");
        if (hax.getModuleEnabled(ModuleID::SHOW_PERCENTAGE_DECIMAL)) {
            if (hax.bestRunStart > 0) {
                writer.write("{:.2f}% - ", hax.bestRunStart);
            }
            writer.write("{:.2f}%\n", hax.bestRunEnd);
        } else {
            if (hax.bestRunStart > 0) {
                writer.write("{}% - ", floorf(hax.bestRunStart));
            }
            writer.write("{}%\n", floorf(hax.bestRunEnd));
        }
    }
    if (hax.getModuleEnabled(ModuleID::LABEL_JUMPS)) {
        writer.write("Jumps: {}\n", getCurrentJumps());
    }
    if (hax.getModuleEnabled(ModuleID::LABEL_CLICKS)) {
        writer.write("Clicks: {}\n", hax.clicks);
    }
    if (hax.getModuleEnabled(ModuleID::LABEL_TIME_SPENT)) {
        writer.write("Time Spent: {}\n", getClkTimer());
    }
    if (hax.getModuleEnabled(ModuleID::LABEL_FRAMES)) {
        writer.write("Frames: {}\n", hax.frameCount);
    }
    if (hax.getModuleEnabled(ModuleID::SPEEDHACK) && 
    hax.getModuleEnabled(ModuleID::LABEL_SPEEDHACK)) {
        writer.write("Speedhack: x{:.2f}\n", hax.timeScale);
    }
    if (hax.getModuleEnabled(ModuleID::NOCLIP)) {    
        if (hax.getModuleEnabled(ModuleID::LABEL_NOCLIP_ACCURACY)) {
            writer.write("NoClip Accuracy: {:.2f}%\n", hax.noclipAccuracy);
        }
        if (hax.getModuleEnabled(ModuleID::LABEL_NOCLIP_DEATHS)) {
            writer.write("NoClip Deaths: {}\n", hax.deaths);
        }
    }
    auto player = getPlayer();
    if (player) {
        if (hax.getModuleEnabled(ModuleID::LABEL_PLAYER_POSITION)) {
            writer.write("Player X: {}\nPlayer Y: {}\n", player->getPositionX(), player->getPositionY());
        }
        if (hax.getModuleEnabled(ModuleID::LABEL_PLAYER_ROTATION)) {
            writer.write("Player Rotation: {}\n", player->getRotation());
        }
        if (hax.getModuleEnabled(ModuleID::LABEL_PCOMMAND)) {
            writer.write("X Velocity: {}\nGravity: {}\nY Velocity: {}\n", getXVelocity(player), getGravity(player), getYStart(player));
        }
    }
    hax.uiLabel->setString(writer.c_str());
}

const char* getSwitcherText() {
    HaxManager& hax = HaxManager::sharedState();
    auto startPoses = hax.getStartPositions();
    return fmt::format("{}/{}", hax.startPosIndex + 1, startPoses->count()).c_str();
}

void UILayer::createSwitcher() {
    HaxManager& hax = HaxManager::sharedState();
    auto startPoses = hax.getStartPositions();
    if (!startPoses || startPoses == nullptr) return; 
    if (startPoses->count() < 1) {
        return;
    }

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto parent = CCNode::create();
    parent->setPosition(ccp(winSize.width / 2, 23));
    addChild(parent);
    hax.spSwitcherParent = parent;

    auto label = CCLabelBMFont::create(getSwitcherText(), "bigFont.fnt");
    label->setString(getSwitcherText());
    label->setScale(0.6);
    parent->addChild(label);
    hax.switcherLabel = label;

    auto menu = CCMenu::create();
    parent->addChild(menu);
    menu->setPosition(CCPointZero);

    CCSprite* prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    prevSpr->setScale(0.75);
    CCMenuItemSpriteExtra* prevBtn = CCMenuItemSpriteExtra::create(prevSpr, prevSpr, this, menu_selector(UILayer::onBackSP));
    menu->addChild(prevBtn);
    prevBtn->setPosition({-50, 0});

    CCSprite* nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextSpr->setScale(0.75);
    nextSpr->setFlipX(true);
    CCMenuItemSpriteExtra* nextBtn = CCMenuItemSpriteExtra::create(nextSpr, nextSpr, this, menu_selector(UILayer::onForwardSP));
    menu->addChild(nextBtn);
    nextBtn->setPosition(ccp(50, 0));
}

void UILayer::onBackSP() {
    HaxManager& hax = HaxManager::sharedState();
    auto startPoses = hax.getStartPositions();
    if (hax.startPosIndex > -1) hax.startPosIndex--;
    else hax.startPosIndex = startPoses->count() - 1;
    UILayer::pickStartPos(hax.startPosIndex);
}
void UILayer::onForwardSP() {
    HaxManager& hax = HaxManager::sharedState();
    auto startPoses = hax.getStartPositions();
    if (startPoses->count() != 0 && (hax.startPosIndex == -1 || hax.startPosIndex < startPoses->count() - 1)) hax.startPosIndex++;
    else hax.startPosIndex = -1;
    UILayer::pickStartPos(hax.startPosIndex);
}

void UILayer::pickStartPos(int ind) {
    cocos2d::CCArray* checkpoints = getPlayLayerCheckpoints();
    while (checkpoints->count() > 0) {
        getPlayLayer()->removeLastCheckpoint();
    }
    HaxManager& hax = HaxManager::sharedState();
    auto startPoses = hax.getStartPositions();
    if (ind == -1) {
        setStartPos(ccp(0, 105));
        hax.startPercent = 0;
    } else if (ind >= startPoses->count()) return;
    else {
        setStartPos(getRealPosition(static_cast<GameObject*>(startPoses->objectAtIndex(ind))));
    }
    getPlayLayer()->resetLevel();
    if (ind != -1) hax.startPercent = getCurrentPercentageF();
    hax.switcherLabel->setString(getSwitcherText());
}

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
    percentageLabel->setPosition(ccp(winSize.width / 2, winSize.height - 7.5));
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
    if (!hax.pMenu) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        auto menu = CCMenu::create();
        menu->setPosition(ccp(winSize.width - 30, winSize.height - 40));
        this->addChild(menu, 10000);
        hax.pMenu = menu;
    }
    if (!hax.pButton1) 
        hax.pButton1 = createPCButton("S+", -55, menu_selector(UILayer::speedUp), this, hax.pMenu);
    if (!hax.pButton2) 
        hax.pButton2 = createPCButton("S-", -85, menu_selector(UILayer::speedDown), this, hax.pMenu);
    if (!hax.pButton3) 
        hax.pButton3 = createPCButton("G+", -115, menu_selector(UILayer::gravityUp), this, hax.pMenu);
    if (!hax.pButton4) 
        hax.pButton4 = createPCButton("G-", -145, menu_selector(UILayer::gravityDown), this, hax.pMenu);
    if (!hax.pButton5) 
        hax.pButton5 = createPCButton("Y+", -175, menu_selector(UILayer::yStartUp), this, hax.pMenu);
    if (!hax.pButton6) 
        hax.pButton6 = createPCButton("Y-", -205, menu_selector(UILayer::yStartDown), this, hax.pMenu);
}

void UILayer::speedUp() {
    PlayerObject* player = getPlayer();
    addXVelocity(player, 0.5d);
    HaxManager& hax = HaxManager::sharedState();
    hax.pSpeedModified += 1;
    hax.setCheating(true);
    player->logValues();
}
void UILayer::speedDown() {
    PlayerObject* player = getPlayer();
    addXVelocity(player, -0.5d);
    HaxManager& hax = HaxManager::sharedState();
    hax.pSpeedModified -= 1;
    hax.setCheating(true);
    player->logValues();
}
void UILayer::gravityUp() {
    PlayerObject* player = getPlayer();
    addGravity(player, 0.1d);
    HaxManager& hax = HaxManager::sharedState();
    hax.pGravityModified += 1;
    hax.setCheating(true);
    player->logValues();
}
void UILayer::gravityDown() {
    PlayerObject* player = getPlayer();
    addGravity(player, -0.1d);
    HaxManager& hax = HaxManager::sharedState();
    hax.pGravityModified -= 1;
    hax.setCheating(true);
    player->logValues();
}
void UILayer::yStartUp() {
    PlayerObject* player = getPlayer();
    addYStart(player, 0.5d);
    HaxManager& hax = HaxManager::sharedState();
    hax.pYStartModified += 1;
    hax.setCheating(true);
    player->logValues();
}
void UILayer::yStartDown() {
    PlayerObject* player = getPlayer();
    addYStart(player, -0.5d);
    HaxManager& hax = HaxManager::sharedState();
    hax.pYStartModified -= 1;
    hax.setCheating(true);
    player->logValues();
}
bool (*TRAM_UILayer_init)(UILayer* self);
bool UILayer_init(UILayer* self) {
    if (!TRAM_UILayer_init(self)) return false;
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    if (hax.getModuleEnabled(ModuleID::CHEAT_INDICATOR)) {
        self->createCheatIndicator();
    }
    if (hax.getModuleEnabled(ModuleID::SHOW_PERCENTAGE)) {
        self->createPercentageLabel();
    }
    if (hax.getModuleEnabled(ModuleID::PCOMMAND)) {
        self->createPCommand();
    }
    if (hax.getShowLabel()) {
        self->createLabel();
    }
    hax.noclipTint = CCLayerColor::create(ccc4(255, 0, 0, 0));
    hax.noclipTint->_setZOrder(1000);
    self->addChild(hax.noclipTint);
    // moved to PlayLayer::init
    // if (hax.getModuleEnabled("start_pos_switcher")) {
    //     self->createSwitcher();
    // }
    return true;
}
void (*TRAM_UILayer_destructor)(UILayer* self);
void UILayer_destructor(UILayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    TRAM_UILayer_destructor(self);
    hax.resetValues();
}
void (*TRAM_UILayer_ccTouchBegan)(UILayer* self, CCTouch* touch, CCEvent* event);
void UILayer_ccTouchBegan(UILayer* self, CCTouch* touch, CCEvent* event) {
    TRAM_UILayer_ccTouchBegan(self, touch, event);
    HaxManager& hax = HaxManager::sharedState();
    hax.clicks++;
}
void (*TRAM_UILayer_toggleCheckpointsMenu)(UILayer* self, bool toggle);
void UILayer_toggleCheckpointsMenu(UILayer* self, bool toggle) {
    TRAM_UILayer_toggleCheckpointsMenu(self, toggle);
    HaxManager& hax = HaxManager::sharedState();
    if (toggle && hax.getModuleEnabled(ModuleID::HIDE_CHECKPOINT_BUTTONS)) {
        getCheckpointMenu(self)->setOpacity(60);
    }
}

void UILayer_om() {
    Omni::hook("_ZN7UILayer4initEv",
        reinterpret_cast<void*>(UILayer_init),
        reinterpret_cast<void**>(&TRAM_UILayer_init));
    Omni::hook("_ZN7UILayerD0Ev",
        reinterpret_cast<void*>(UILayer_destructor),
        reinterpret_cast<void**>(&TRAM_UILayer_destructor));
    Omni::hook("_ZN7UILayer12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE",
        reinterpret_cast<void*>(UILayer_ccTouchBegan),
        reinterpret_cast<void**>(&TRAM_UILayer_ccTouchBegan));
    Omni::hook("_ZN7UILayer21toggleCheckpointsMenuEb",
        reinterpret_cast<void*>(UILayer_toggleCheckpointsMenu),
        reinterpret_cast<void**>(&TRAM_UILayer_toggleCheckpointsMenu));
}