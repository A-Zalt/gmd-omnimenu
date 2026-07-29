#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "PauseLayer.hpp"
#include "SpeedhackInput.hpp"
#include "CCTextInputNode.hpp"

void PauseLayer::onOpenMenu() {
    auto haxOverlay = HaxMenu::create(this);
    this->addChild(haxOverlay, 1000);
    this->setTouchEnabled(false);
}

SpeedhackInput* SpeedhackInput::create(PauseLayer* parent) {
    auto ret = new SpeedhackInput;
    if (ret->init(parent)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
bool SpeedhackInput::init(PauseLayer* parent) {
    if (!CCLayer::init()) return false;
    this->parent = parent;

    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto clock = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
    clock->setPosition(ccp(winSize.width - 105, winSize.height - 53));
    clock->setScale(0.75);
    addChild(clock);

    this->input = CCTextInputNode::create(40.0, 40.0, "Speed", "Thonburi", 12, "bigFont.fnt");
    input->setPosition(ccp(winSize.width - 70, winSize.height - 53));
    input->setMaxLabelScale(0.7);
    input->setLabelPlaceholderScale(0.5);
    setCharLimit(input, 5);
    input->setAllowedChars("0123456789.");
    input->setAnchorPoint({0, 0.5});
    input->setLabelPlaceholderColor(ccc3(127, 127, 127));
    input->setString(fmt::format("{:.2f}", hax.timeScale).c_str());
    addChild(input);

    auto bg = extension::CCScale9Sprite::create("square02_small.png", CCRectMake(0,0,40,40));
    bg->setContentSize(CCSizeMake(60, 20));
    bg->_setZOrder(-1);
    bg->setPosition({winSize.width - 69, winSize.height - 53});
    addChild(bg);
    bg->setScale(0.9);

    return true;
}

void SpeedhackInput::textChanged(CCTextInputNode* node) {
    HaxManager& hax = HaxManager::sharedState();
    if (!node) return;

    auto booba = *(CCTextFieldTTF**)((char*)node + CCTextInputNode__m_textField);
    if (!booba) return;
    if (!booba->m_pInputText) return;
    auto bro = booba->m_pInputText->c_str();
    if (!bro || bro == nullptr) {
        return;
    }

    float value = std::atof(bro);
    if (value >= 0.01 && value <= 500) {
        hax.timeScale = value;
        if (hax.timeScale != 1) hax.setCheating(true);
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(hax.timeScale);
    }
}

void SpeedhackInput::setupDelegates() {
    if (input) {
        setTextInputDelegate(input, this);
    }
}

void PauseLayer::createSpeedhack() {
    HaxManager& hax = HaxManager::sharedState();
    auto widget = SpeedhackInput::create(this);
    widget->setupDelegates();
    addChild(widget);
    hax.speedInputWidget = widget;
}
void PauseLayer::createEye() {
    HaxManager& hax = HaxManager::sharedState();
    auto menu = CCMenu::create();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    menu->setPosition(ccp(winSize.width - 35, 35));
    auto eyeSpr = CCSprite::create("hideBtn_001.png");
    auto eye = CCMenuItemSpriteExtra::create(eyeSpr, eyeSpr, this, menu_selector(PauseLayer::toggleVisibility));
    eye->setOpacity(127);
    eye->setTag(1);
    eye->setSizeMult(1.5);
    menu->addChild(eye);
    addChild(menu);
    hax.eyeMenu = menu;
}
static int oldOpacity = 0;
void PauseLayer::toggleVisibility() {
    auto children = getChildren();
    HaxManager& hax = HaxManager::sharedState();
    int temp = this->getOpacity();
    this->setOpacity(oldOpacity);
    oldOpacity = temp;
    for (int i = 0; i < children->count(); i++) {
        auto child = static_cast<CCNode*>(children->objectAtIndex(i));
        if (!hax.eyeMenu || child != static_cast<CCNode*>(hax.eyeMenu)) {
            child->setVisible(!child->isVisible());
        }
    }
    if (!hax.dead) hax.hasTouchedTheEye = true;
    auto eyeBtn = static_cast<CCMenuItemSpriteExtra*>(hax.eyeMenu->getChildByTag(1));
    if (eyeBtn->getOpacity() != 127) eyeBtn->setOpacity(127);
    else eyeBtn->setOpacity(43);
    if (hax.speedInputWidget) {
        if (hax.speedInputWidget->getScale() > 0) hax.speedInputWidget->setScale(0);
        else hax.speedInputWidget->setScale(1);
    }
}

void (*TRAM_PauseLayer_customSetup)(PauseLayer* self);
void PauseLayer_customSetup(PauseLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
#ifndef FORCE_AUTO_SAFE_MODE
    if (hax.getModuleEnabled(ModuleID::LEVEL_EDIT)) {
        setEditButton(true);
    } else {
        setEditButton(false);
    }
#endif
    if (hax.getModuleEnabled(ModuleID::SHOW_RESTART_BUTTON)) {
        setRestartButton(true);
    } else {
        setRestartButton(false);
    }
    TRAM_PauseLayer_customSetup(self);
    hax.pauseLayer = self;
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    CCMenu* btnMenu = CCMenu::create();
    self->addChild(btnMenu, 999);
    btnMenu->setPosition(ccp(0, winSize.height));

    CCSprite* menuSpr = CCSprite::create("OMNImenu_btn.png");
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(menuSpr, menuSpr, self, menu_selector(PauseLayer::onOpenMenu));
    btnMenu->addChild(menuBtn, 999);
    menuBtn->setPosition(ccp(50.f, -50.f));
#ifdef STEALTH_MODE
    menuBtn->setVisible(false);
#endif
    if (hax.getModuleEnabled(ModuleID::SPEEDHACK)) {
        self->createSpeedhack();
    }
    if (hax.getModuleEnabled(ModuleID::HIDE_PAUSE_MENU)) {
        self->createEye();
    }
}
void (*TRAM_PauseLayer_destructor)(PauseLayer* self);
void PauseLayer_destructor(PauseLayer* self) {
    TRAM_PauseLayer_destructor(self);
    HaxManager& hax = HaxManager::sharedState();
    hax.pauseLayer = nullptr;
    hax.speedInputWidget = nullptr;
    hax.eyeMenu = nullptr;
}

#if GAME_VERSION >= GV_1_7
void (*TRAM_PauseLayer_onQuit)(PauseLayer* self, CCObject* sender);
#else
void (*TRAM_PauseLayer_onQuit)(PauseLayer* self);
#endif

void ConfirmExit::FLAlert_Clicked(FLAlertLayer*, bool btn2) {
    if (btn2 && pauseLayer) {
        TRAM_PauseLayer_onQuit(pauseLayer dummy_sender_param_1_7);
    }
}

#if GAME_VERSION >= GV_1_7
void PauseLayer_onQuit(PauseLayer* self, CCObject* sender) {
#else
void PauseLayer_onQuit(PauseLayer* self) {
#endif
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::CONFIRM_EXIT)) {
        auto delegate = new ConfirmExit();
        delegate->pauseLayer = self;
        FLAlertLayer::create(
            delegate,
            "Exit Level",
            "Are you sure you want to <cr>exit</c>?",
            "Cancel",
            "Exit",
            300.f
        )->show();
        return;
    }
    TRAM_PauseLayer_onQuit(self sender_param_1_7);
}

// There is no delayedResetLevel in 1.0, so any resetLevel calls will be blocked while we are dead with custom respawn time
#if GAME_VERSION < GV_1_1
void (*TRAM_PauseLayer_onRestart)(PauseLayer* self);
void PauseLayer_onRestart(PauseLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    getPlayLayer()->resume();
    PlayLayer::resetLevelLogic(getPlayLayer());
    self->removeMeAndCleanup();
}
#endif

void (*TRAM_PauseLayer_onEdit)(PauseLayer* self SEL_MenuHandler_1_7_compat2);
void PauseLayer_onEdit(PauseLayer* self SEL_MenuHandler_1_7_compat2) {
    CCLog("PauseLayer::onEdit");
    HaxManager& hax = HaxManager::sharedState();
    hax.mbfEnabled = hax.getModuleEnabled(ModuleID::MUSIC_BUG_FIX);
    hax.areWeInPlayLayer = false;
    TRAM_PauseLayer_onEdit(self sender_param_1_7);
    // auto SAE = CocosDenshion::SimpleAudioEngine::sharedEngine();
    // SAE->stopBackgroundMusic();
}

void PauseLayer_om() {
    Omni::hook("_ZN10PauseLayer11customSetupEv",
        reinterpret_cast<void*>(PauseLayer_customSetup),
        reinterpret_cast<void**>(&TRAM_PauseLayer_customSetup));
    Omni::hook("_ZN10PauseLayerD1Ev",
        reinterpret_cast<void*>(PauseLayer_destructor),
        reinterpret_cast<void**>(&TRAM_PauseLayer_destructor));
#if GAME_VERSION < GV_1_1
    Omni::hook("_ZN10PauseLayer9onRestartEv",
        reinterpret_cast<void*>(PauseLayer_onRestart),
        reinterpret_cast<void**>(&TRAM_PauseLayer_onRestart));
#endif
    Omni::hook(
#if GAME_VERSION < GV_1_7
    "_ZN10PauseLayer6onQuitEv",
#else
    "_ZN10PauseLayer6onQuitEPN7cocos2d8CCObjectE",
#endif
        reinterpret_cast<void*>(PauseLayer_onQuit),
        reinterpret_cast<void**>(&TRAM_PauseLayer_onQuit));
    Omni::hook(
    #if GAME_VERSION < GV_1_7
        "_ZN10PauseLayer6onEditEv",
    #else
        "_ZN10PauseLayer6onEditEPN7cocos2d8CCObjectE",
    #endif
        reinterpret_cast<void*>(PauseLayer_onEdit),
        reinterpret_cast<void**>(&TRAM_PauseLayer_onEdit));
}