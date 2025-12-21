// massive thanks to adelfa for helping me figure out this touch dispatcher bullshit

#include "../layers/HaxMenu.hpp"
#include "FLAlertLayer.hpp"
#include "HaxManager.hpp"
#include "CCMenuItemToggler.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "Utils.hpp"
#include "GameSoundManager.hpp"
#include "ButtonSprite.hpp"

using namespace cocos2d;

HaxMenu* HaxMenu::create(CCLayer* referrer) {
    auto ret = new HaxMenu();
    if (ret->init(referrer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

// CCScene* HaxMenu::scene() {
//     auto layer = HaxMenu::create();
//     auto scene = CCScene::create();
//     scene->addChild(layer);
//     return scene;
// }

float HaxMenu::getDuration() {
    auto& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::FAST_MENU)) return 0.f;
    return 0.25f;
}

ccColor3B color = ccc3(127, 255, 255);

// bool CCRectContainsPoint(CCRect rect, const CCPoint& point)
// {
//     if (point.x >= CCRect::CCRectGetMinX(rect) && point.x < CCRect::CCRectGetMaxX(rect)
//         && point.y >= CCRect::CCRectGetMinY(rect) && point.y < CCRect::CCRectGetMaxY(rect)) {
//         return true;
//     }
//     return false;
// }


bool HaxMenu::init(CCLayer* referrer) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180)))
        return false;

    canSpoofPih = true;

    auto& hax = HaxManager::sharedState();

    this->referrer = referrer;
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    CCTouchDispatcher* touchDispatch = director->getTouchDispatcher();
    touchDispatch->setForcePrio(true);
    touchDispatch->setTargetPrio(0x80000002);

    this->catButtons = CCArray::create();
    this->catButtons->retain();

    CCNode* leftParent = CCNode::create();
    addChild(leftParent);
    leftParent->setPosition(-100.f, winSize.height / 2);
    this->leftParent = leftParent;
    
    CCSprite* leftPanel = CCSprite::create("menupanel.png");
    leftParent->addChild(leftPanel);
    leftPanel->setPosition({0.f, 0.f});
    leftPanel->setScaleY(2.0f);
    this->leftPanel = leftPanel;

    auto logo = CCSprite::create("omnimenu_logo.png");
    leftParent->addChild(logo, 1001);
    logo->setPosition(ccp(0, winSize.height / 2 - 30));
    logo->setScale(0.8f);
    
    leftParent->runAction(CCEaseOut::create(
        CCMoveTo::create(getDuration(), ccp(100.0f, winSize.height / 2)), 3
    ));

    CCNode* rightParent = CCNode::create();
    addChild(rightParent);
    rightParent->setPosition(winSize.width + 100.f, winSize.height / 2);
    this->rightParent = rightParent;
    
    CCSprite* rightPanel = CCSprite::create("menupanel.png");
    rightParent->addChild(rightPanel);
    rightPanel->setPosition({0.f, 0.f});
    rightPanel->setScaleY(2.0f);
    this->rightPanel = rightPanel;
    
    rightParent->runAction(CCEaseOut::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width - 100.f, winSize.height / 2)), 3
    ));

    this->catMenu = CCMenu::create();
    this->leftParent->addChild(catMenu, 1001);
    catMenu->setPosition(ccp(-75, 0));

    this->modMenu = CCMenu::create();
    addChild(modMenu, 1002);
    this->modMenu->setPosition(this->rightParent->getPosition());

    modMenu->runAction(CCEaseOut::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width - 80.f, winSize.height / 2)), 3
    ));
    // modMenu->setPosition(ccp(winSize.width - 75, 0));

    addButton(" Player ", 14, 80, this, menu_selector(HaxMenu::onPlayer));
    addButton(" Visual ", 14, 60, this, menu_selector(HaxMenu::onVisual));
    addButton(" Editor ", 14, 40, this, menu_selector(HaxMenu::onEditor));
    addButton(" Bypass ", 14, 20, this, menu_selector(HaxMenu::onBypass));
    addButton(" Informational ", 14, 0, this, menu_selector(HaxMenu::onInformational));
    addButton(" Universal ", 14, -20, this, menu_selector(HaxMenu::onUniversal));
    addButton(" Label ", 14, -40, this, menu_selector(HaxMenu::onLabel));
    addButton(" Particles ", 14, -60, this, menu_selector(HaxMenu::onParticles));

    setTouchEnabled(true);
    setKeypadEnabled(true);
    // referrer->setTouchEnabled(false);
    // referrer->setScale(0.2f);

    onCategory(hax.lastCategory);

    return true;
}

void HaxMenu::setColorAtIndex(int ind) {
    int index = ind + 1;
    if (ind == 7) index = 1;
    else if (ind == 0) index = 0;

    static_cast<CCLabelTTF*>(static_cast<CCMenuItemLabel*>(this->catButtons->objectAtIndex(index))->getLabel())->setColor(color);

    for (int i = 0; i < this->catButtons->count(); i++) {
        if (i == index) continue;
        static_cast<CCLabelTTF*>(static_cast<CCMenuItemLabel*>(this->catButtons->objectAtIndex(i))->getLabel())->setColor(ccWHITE);
    }
}
void HaxMenu::addButton(const char* text, float fontSize, float yOffset, CCObject* target, SEL_MenuHandler selector) {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    auto label = CCLabelTTF::create(text, "Helvetica-Oblique.ttf", fontSize);
    auto item = CCMenuItemLabel::create(label, target, selector);
    this->catButtons->addObject(item);
    this->catMenu->addChild(item, 1002);
    item->setAnchorPoint({0, 0.5});
    item->setPosition(ccp(-25, 10 + yOffset));
}
void HaxMenu::onPlayer() {
    onCategory(ModuleCategory::Player);
}
void HaxMenu::onVisual() {
    onCategory(ModuleCategory::Visual);
}
void HaxMenu::onEditor() {
    onCategory(ModuleCategory::Editor);
}
void HaxMenu::onBypass() {
    onCategory(ModuleCategory::Bypass);
}
void HaxMenu::onInformational() {
    onCategory(ModuleCategory::Informational);
}
void HaxMenu::onUniversal() {
    onCategory(ModuleCategory::Universal);
}
void HaxMenu::onLabel() {
    onCategory(ModuleCategory::Label);
}
void HaxMenu::onParticles() {
    onCategory(ModuleCategory::Particles);
}

void HaxMenu::onCategory(ModuleCategory category) {
    auto& hax = HaxManager::sharedState();
    hax.lastCategory = category;

    setColorAtIndex(static_cast<int>(category));

    while (this->rightParent->getChildrenCount() > 1) {
        CCNode* node = static_cast<CCNode*>(this->rightParent->getChildren()->objectAtIndex(1));
        this->rightParent->removeChild(node, true);
    }
    while (this->modMenu->getChildrenCount() > 0) {
        if (this->modMenu->getChildrenCount() > 1) {
            CCNode* node2 = static_cast<CCNode*>(this->modMenu->getChildren()->objectAtIndex(1));
            this->modMenu->removeChild(node2, true);
        }
        CCNode* node = static_cast<CCNode*>(this->modMenu->getChildren()->objectAtIndex(0));
        std::string* userData = static_cast<std::string*>(node->getUserData());
        delete userData;
        userData = nullptr;
        this->modMenu->removeChild(node, true);
    }
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    int y = -20;

    int key = 0;
    for (Module& record : hax.modules)
    {
        if (!record.exists) {
            key++;
            continue;
        }
        if (record.category != category) {
            key++;
            continue;
        }

#if GAME_VERSION > GV_1_0
        auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
#else
        auto toggleOn = CCSprite::create("GJ_checkOn.png");
        auto toggleOff = CCSprite::create("GJ_checkOff.png");
#endif

        auto checkbox = CCMenuItemToggler::create(toggleOn, toggleOff, this, menu_selector(HaxMenu::toggler));
        checkbox->toggle(!record.enabled);
        checkbox->setScale(0.5f);
        checkbox->setTag(key);
        this->modMenu->addChild(checkbox, 1003);
        checkbox->setPosition({-100, winSize.height / 2 + y});

        std::string labelValue = record.name;
        labelValue += " "; // italics font gets cut off grrrr
        auto label = CCLabelTTF::create(labelValue.c_str(), "Helvetica-Oblique.ttf", 11);
        this->rightParent->addChild(label, 1003);
        label->setAnchorPoint({0.f, 0.5f});
        label->setPosition(ccp(-68, winSize.height / 2 + y));

        CCSprite* infoSpr = createInfoSprite();
        infoSpr->setScale(0.6f);
        CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, this, menu_selector(HaxMenu::modInfo));
        infoBtn->setSizeMult(1.5f);
        infoBtn->setPosition(ccp(-80 + label->getContentSize().width, winSize.height / 2 + y));
        infoBtn->setTag(key);
        this->modMenu->addChild(infoBtn, 1003);
        y -= 16;
        key++;
    }
    if (category == ModuleCategory::Universal) {
        auto udidSpr = ButtonSprite::create("Copy UDID", 50, 0, 1, false, "bigFont.fnt", "GJ_button_04.png");
        // delSelSpr->setScale(0.8f);

        auto udidBtn = CCMenuItemSpriteExtra::create(udidSpr, udidSpr, this, menu_selector(HaxMenu::onUDID));
        udidBtn->setPosition(ccp(-70, -winSize.height / 2 + 30));
        this->modMenu->addChild(udidBtn);
        this->udidBtn = udidBtn;
    }
}
 // if you see this dm "dfdfdcsxxs" to scarfolk.resident on discord

void HaxMenu::onUDID() {
    std::string udid = getPlayerUDID();
    copyStringToClipboard(udid.c_str());

    FLAlertLayer::create(
        nullptr,
        "UDID",
        CCString::createWithFormat(
            "<cy>Your UDID</c>: %s. It should also appear on your <cg>clipboard</c>.\n<co>Warning</c>: Do <cr>NOT</c> share this with anyone! Your <cy>UDID</c> can be used to <cr>access your account</c>.", udid.c_str())->getCString(),
        "OK",
        nullptr,
        300.f
    )->show();
}

void HaxMenu::toggler(CCObject* sender) {
    CCMenuItem* menuItem = (CCMenuItem *)(sender);
    int tag = menuItem->getTag();
    auto& hax = HaxManager::sharedState();
    hax.modules[tag].toggle();
    if (tag == ModuleID::PIG_SPOOFING) {
        if (!canSpoofPih) return;
        float value = static_cast<float>(rand()) / RAND_MAX * 333;
        CCLog("%f", value);
        if (value >= 33 && value < 34) {
            canSpoofPih = false;
            static_cast<CCMenuItemToggler*>(sender)->toggle(false);
            this->runAction(CCSequence::create(
                CCDelayTime::create(2.f),
                CCCallFunc::create(this, callfunc_selector(HaxMenu::onThree)),
                nullptr
            ));
        } else {
            GameSoundManager::sharedManager()->playEffect("pih.mp3", 1.0, 0.0, 1.0);
            hax.modules[tag].toggle();
            static_cast<CCMenuItemToggler*>(sender)->toggle(true);
            this->runAction(CCSequence::create(
                CCDelayTime::create(0.2f),
                CCCallFunc::create(this, callfunc_selector(HaxMenu::onPih)),
                nullptr
            ));
        }
    }
}
void HaxMenu::modInfo(CCObject* sender) {
    CCMenuItem* menuItem = (CCMenuItem *)(sender);
    int tag = menuItem->getTag();
    auto& hax = HaxManager::sharedState();
    Module mod = hax.modules[tag];
    FLAlertLayer::create(
        nullptr,
        mod.name,
        mod.description.c_str(),
        "OK",
        nullptr,
        300.f
    )->show();
}
void HaxMenu::onPih(CCObject* sender) {
    // CCLog("CCObject: %x", sizeof(CCObject));
    // CCLog("CCPoint: %x", sizeof(CCPoint));
    // CCLog("CCNode: %x", sizeof(CCNode));
    // CCLog("CCLayer: %x", sizeof(CCLayer));

    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    CCSprite* pih = CCSprite::create("pih.png");
    addChild(pih, 1010);
    pih->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    pih->setScale(1.5);
    pih->runAction(CCSequence::create(
        CCFadeOut::create(1.5f),
        CCCallFunc::create(pih, callfunc_selector(CCNode::removeFromParentAndCleanup)),
        nullptr
    ));
}
void HaxMenu::onThree(CCObject* sender) {
    GameSoundManager::sharedManager()->playEffect("three.mp3", 1.0, 0.0, 1.0);
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    CCSprite* three = CCSprite::create("three.png");
    addChild(three, 1011);
    three->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    three->runAction(CCSequence::create(
        CCFadeIn::create(0.2f),
        CCDelayTime::create(1),
        CCFadeOut::create(0.25f),
        CCCallFunc::create(three, callfunc_selector(CCNode::removeFromParentAndCleanup)),
        nullptr
    ));
}

void HaxMenu::keyBackClicked() {
    onClose(nullptr);
}

void HaxMenu::onClose(CCObject* sender) {
    auto& hax = HaxManager::sharedState();
    hax.saveSettingsToFile();

    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    //referrer->setTouchEnabled(true);
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    leftParent->runAction(CCEaseIn::create(
        CCMoveTo::create(getDuration(), ccp(-80.f, winSize.height / 2)), 3
    ));
    rightParent->runAction(CCEaseIn::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width + 80.f, winSize.height / 2)), 3
    ));
    modMenu->runAction(CCEaseIn::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width + 80.f, winSize.height / 2)), 3
    ));
    if (sender) this->retain();
    this->runAction(CCSequence::create(
        CCDelayTime::create(getDuration()),
        CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParentAndCleanup)), // CCRemoveSelf does not exist in old cocos
        nullptr
    ));
}

bool HaxMenu::ccTouchBegan(cocos2d::CCTouch* t, cocos2d::CCEvent*)
{
    // CCPoint pg = t->locationInView();
    // CCPoint gl = t->locationInView();
    // CCPoint p1 = leftPanel->convertToNodeSpace(pg);
    // CCPoint p2 = rightPanel->convertToNodeSpace(pg);
    // CCRect bb1 = leftPanel->boundingBox();
    // CCRect bb2 = rightPanel->boundingBox();
    // CCLog("bb1: %i %i %i %i", CCRect::CCRectGetMinX(bb1), CCRect::CCRectGetMinY(bb1), CCRect::CCRectGetMaxX(bb1), CCRect::CCRectGetMaxY(bb1));
    // CCLog("bb2: %i %i %i %i", CCRect::CCRectGetMinX(bb2), CCRect::CCRectGetMinY(bb2), CCRect::CCRectGetMaxX(bb2), CCRect::CCRectGetMaxY(bb2));
    // CCLog("p1: %i %i", p1.x, p1.y);
    // CCLog("p2: %i %i", p2.x, p2.y);
    // CCLog("pg: %i %i", pg.x, pg.y);
    // if (!CCRectContainsPoint(leftPanel->boundingBox(), leftPanel->convertToNodeSpace(t->locationInView()))
    // && !CCRectContainsPoint(rightPanel->boundingBox(), rightPanel->convertToNodeSpace(t->locationInView()))) {
    //     onClose(nullptr);
    // }
    onClose(nullptr);
    return true;
}

// from starry sky. never in a million years would i have figured ts out without it
void HaxMenu::registerWithTouchDispatcher()
{
    CCTouchDispatcher* t = CCDirector::sharedDirector()->getTouchDispatcher();   
    t->addTargetedDelegate(this, 0x80000003, true);
}