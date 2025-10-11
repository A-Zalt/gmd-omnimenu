#include "../layers/HaxLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "MenuLayer.hpp"
#include "CCMenuItemToggler.hpp"
#include "HaxManager.hpp"

using namespace cocos2d;

HaxLayer* HaxLayer::create(bool fromRope) {
    auto ret = new HaxLayer();
    if (ret->init(fromRope)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
};

CCScene* HaxLayer::scene(bool fromRope) {
    auto layer = HaxLayer::create(fromRope);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool HaxLayer::init(bool fromRope) {
    if(!CCLayer::init())
        return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    m_background = CCSprite::create("menubackground.png");
    // m_background->setAnchorPoint({ 0.f, 0.f });
    m_background->setColor({66, 39, 133});
    m_background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    m_background->setScaleX(2.0f);
    addChild(m_background, -2);

    CCMenu* buttonMenu = CCMenu::create();

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, backSpr, this, menu_selector(HaxLayer::onClose));

    buttonMenu->addChild(backBtn);
    buttonMenu->setPosition(ccp(25.f, winSize.height / 2));
    backBtn->setPosition(ccp(0, winSize.height / 2 - 25));

    addChild(buttonMenu, 1);

    HaxManager& hax = HaxManager::sharedState();

    addCheckbox(0, 0, "NoClip", hax.noClip, menu_selector(HaxLayer::onNoClip),                                  buttonMenu, this);
    addCheckbox(0, 1, "Unlock Icons", hax.iconHack, menu_selector(HaxLayer::onIconHack),                        buttonMenu, this);
    addCheckbox(0, 2, "Text Length Bypass", hax.textLengthBypass, menu_selector(HaxLayer::onTextLengthBypass),  buttonMenu, this);
    addCheckbox(0, 3, "Character Filter Bypass", hax.charFilterBypass, menu_selector(HaxLayer::onFilterBypass), buttonMenu, this);
    addCheckbox(0, 4, "Swear Filter Bypass", hax.swearBypass, menu_selector(HaxLayer::onSwearBypass),           buttonMenu, this);
    addCheckbox(0, 5, "Level Edit", hax.levelEdit, menu_selector(HaxLayer::onLevelEdit),                        buttonMenu, this);
    addCheckbox(0, 6, "Instant Complete", hax.instantComplete, menu_selector(HaxLayer::onInstantComplete),      buttonMenu, this);
    addCheckbox(0, 7, "Fast Menu", hax.fastMenu, menu_selector(HaxLayer::onFastMenu),                           buttonMenu, this);
    addCheckbox(0, 8, "Verify Bypass", hax.verifyHack, menu_selector(HaxLayer::onVerifyHack),                   buttonMenu, this);
    addCheckbox(0, 9, "No Mirror", hax.noMirror, menu_selector(HaxLayer::onNoMirror),                           buttonMenu, this);

    addCheckbox(1, 0, "Object Limit Bypass", hax.objectLimitHack, menu_selector(HaxLayer::onObjectLimitHack),   buttonMenu, this);

    setKeypadEnabled(true);

    return true;
}

void HaxLayer::addCheckbox(float xIdx, float yIdx, const char* text, bool toggle, SEL_MenuHandler callback, CCMenu* menu, CCObject* target) {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

    auto checkbox = CCMenuItemToggler::create(toggleOn, toggleOff, target, callback);
    checkbox->toggle(!toggle);
    checkbox->setPosition(ccp(45 + xIdx * 100, winSize.height / 2 - 55 - (yIdx * 23)));
    checkbox->setScale(0.6f);
    menu->addChild(checkbox, 2);

    auto label = CCLabelBMFont::create(text, "bigFont.fnt");
    label->setAnchorPoint({0.f, 0.5f});
    label->setPosition(ccp(87 + xIdx * 100, winSize.height - 55 - (yIdx * 23)));
    label->setScale(0.4f);
    addChild(label, 3);
}

void HaxLayer::keyBackClicked() {
    HaxLayer::onClose(nullptr);
}

void HaxLayer::onClose(CCObject*) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, MenuLayer::scene()));
}

void HaxLayer::onNoClip(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.noClip = !hax.noClip;
}

void HaxLayer::onIconHack(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.iconHack = !hax.iconHack;
}

void HaxLayer::onTextLengthBypass(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.textLengthBypass = !hax.textLengthBypass;
}

void HaxLayer::onFilterBypass(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.charFilterBypass = !hax.charFilterBypass;
}

void HaxLayer::onSwearBypass(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.swearBypass = !hax.swearBypass;
}

void HaxLayer::onLevelEdit(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.levelEdit = !hax.levelEdit;
}

void HaxLayer::onInstantComplete(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.instantComplete = !hax.instantComplete;
}

void HaxLayer::onFastMenu(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.fastMenu = !hax.fastMenu;
}
void HaxLayer::onVerifyHack(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.verifyHack = !hax.verifyHack;
}
void HaxLayer::onNoMirror(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.noMirror = !hax.noMirror;
}
void HaxLayer::onObjectLimitHack(CCObject*) {
    HaxManager& hax = HaxManager::sharedState();
    hax.objectLimitHack = !hax.objectLimitHack;
}
// 