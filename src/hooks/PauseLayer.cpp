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
    if (value > 0.01 && value <= 500) {
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
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    CCMenu* btnMenu = CCMenu::create();
    self->addChild(btnMenu, 999);
    btnMenu->setPosition(ccp(0, winSize.height));

    CCSprite* menuSpr = CCSprite::create("OMNImenu_btn.png");
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(menuSpr, menuSpr, self, menu_selector(PauseLayer::onOpenMenu));
    btnMenu->addChild(menuBtn, 999);
    menuBtn->setPosition(ccp(50.f, -50.f));
    if (hax.getModuleEnabled(ModuleID::SPEEDHACK)) {
        auto widget = SpeedhackInput::create(self);
        widget->setupDelegates();
        self->addChild(widget);
    }
}

void PauseLayer_om() {
    Omni::hook("_ZN10PauseLayer11customSetupEv",
        reinterpret_cast<void*>(PauseLayer_customSetup),
        reinterpret_cast<void**>(&TRAM_PauseLayer_customSetup));
}