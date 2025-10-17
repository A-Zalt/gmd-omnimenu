// massive thanks to adelfa for helping me figure out this touch dispatcher bullshit

#include "../layers/HaxOverlay.hpp"
#include "FLAlertLayer.hpp"
#include "HaxManager.hpp"
#include "CCMenuItemToggler.hpp"

using namespace cocos2d;

HaxOverlay* HaxOverlay::create(CCLayer* referrer) {
    auto ret = new HaxOverlay();
    if (ret->init(referrer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

// CCScene* HaxOverlay::scene() {
//     auto layer = HaxOverlay::create();
//     auto scene = CCScene::create();
//     scene->addChild(layer);
//     return scene;
// }

float HaxOverlay::getDuration() {
    auto& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("fast_menu")) return 0.f;
    return 0.25f;
}

bool HaxOverlay::init(CCLayer* referrer) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180)))
        return false;

    auto& hax = HaxManager::sharedState();

    this->referrer = referrer;
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    CCTouchDispatcher* touchDispatch = director->getTouchDispatcher();
    touchDispatch->setForcePrio(true);
    touchDispatch->setTargetPrio(0x80000005);

    CCNode* parent = CCNode::create();
    addChild(parent);
    parent->setPosition(winSize.width, 0);
    this->mainParent = parent;
    
    CCNode* categoryParent = CCNode::create();
    parent->addChild(categoryParent, 1003);
    this->categoryParent = categoryParent;

    auto categoryMenu = CCMenu::create();
    parent->addChild(categoryMenu, 1003);
    // parentMenu->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->categoryMenu = categoryMenu;

    CCSprite* menuBackground = CCSprite::create("menubackground2.png");
    parent->addChild(menuBackground, 1000);
    menuBackground->setPosition(ccp(winSize.width / 2, winSize.height / 2));

    auto omniLabel = CCLabelTTF::create(" OMNI ", "Helvetica-Oblique.ttf", 14);
    parent->addChild(omniLabel, 1001);
    omniLabel->setAnchorPoint({0.5, 0});
    omniLabel->setPosition(ccp(winSize.width / 2 - 21, winSize.height / 2 + 90));

    auto menuLabel = CCLabelTTF::create(" menu ", "Helvetica-Oblique.ttf", 10);
    parent->addChild(menuLabel, 1001);
    menuLabel->setAnchorPoint({0.5, 0});
    menuLabel->setPosition(ccp(winSize.width / 2 + 21, winSize.height / 2 + 92));

    auto btnMenu = CCMenu::create();
    parent->addChild(btnMenu, 1002);
    btnMenu->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->btnMenu = btnMenu;

    addButton(" Gameplay ", 12, 90, this, menu_selector(HaxOverlay::onGameplay));
    addButton(" Editor ", 12, 50, this, menu_selector(HaxOverlay::onEditor));
    addButton(" Bypass ", 12, 10, this, menu_selector(HaxOverlay::onBypass));
    addButton(" Informational ", 10, -30, this, menu_selector(HaxOverlay::onInformational));
    addButton(" Universal ", 12, -70, this, menu_selector(HaxOverlay::onUniversal));

    setTouchEnabled(true);
    setKeypadEnabled(true);
    referrer->setTouchEnabled(false);

    parent->runAction(CCEaseOut::create(
        CCMoveTo::create(getDuration(), ccp(0.0f, 0.0f)), 3
    ));

    mainParent = parent;

    onCategory(hax.lastCategory);

    return true;
}

void HaxOverlay::addButton(const char* label, int fontSize, float yOffset, CCObject* target, SEL_MenuHandler selector) {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    auto btn1Sprite = CCSprite::create("menubtn.png");
    auto btn1 = CCMenuItemSprite::create(btn1Sprite, btn1Sprite, btn1Sprite, target, selector);
    btnMenu->addChild(btn1);
    btn1->setAnchorPoint({0, 0.5});
    btn1->setPosition(ccp(-100-189, yOffset));

    auto btn1Label = CCLabelTTF::create(label, "Helvetica-Oblique.ttf", fontSize);
    mainParent->addChild(btn1Label, 1003);
    btn1Label->setAnchorPoint({0, 0.5});
    btn1Label->setPosition(ccp(winSize.width / 2 - 80 - 189, winSize.height / 2 + yOffset));
}

void HaxOverlay::onGameplay() {
    onCategory(ModuleCategory::Gameplay);
}
void HaxOverlay::onEditor() {
    onCategory(ModuleCategory::Editor);
}
void HaxOverlay::onUniversal() {
    onCategory(ModuleCategory::Universal);
}
void HaxOverlay::onInformational() {
    onCategory(ModuleCategory::Informational);
}
void HaxOverlay::onBypass() {
    onCategory(ModuleCategory::Bypass);
}

void HaxOverlay::onCategory(ModuleCategory category) {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    while (categoryParent->getChildrenCount() > 0) {
        CCNode* node = static_cast<CCNode*>(categoryParent->getChildren()->objectAtIndex(0));
        // node->removeFromParentAndCleanup(true);
        categoryParent->removeChild(node, true);
    }
    while (categoryMenu->getChildrenCount() > 0) {
        CCNode* node = static_cast<CCNode*>(categoryMenu->getChildren()->objectAtIndex(0));
        // node->removeFromParentAndCleanup(true);
        std::string* userData = static_cast<std::string*>(node->getUserData());
        delete userData;
        userData = nullptr;
        categoryMenu->removeChild(node, true);
    }

    auto& hax = HaxManager::sharedState();
    hax.lastCategory = category;
    std::map<std::string, Module*>::iterator it;

    int y = 80;

    for (it = hax.modules.begin(); it != hax.modules.end(); it++)
    {
        std::string* key = new std::string; // heap allocation
        *key = it->first;
        Module* mod = it->second;
        if (mod->category != category) continue;

        auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

        auto checkbox = CCMenuItemToggler::create(toggleOn, toggleOff, this, menu_selector(HaxOverlay::toggler));
        checkbox->toggle(!mod->enabled);
        checkbox->setPosition(ccp(-159, y));
        checkbox->setScale(0.6f);
        checkbox->setUserData(key);
        categoryMenu->addChild(checkbox, 1003);

        std::string labelValue = mod->name;
        labelValue += " "; // italics font gets cut off grrrr
        auto label = CCLabelTTF::create(labelValue.c_str(), "Helvetica-Oblique.ttf", 10);
        categoryParent->addChild(label, 1003);
        label->setAnchorPoint({0.f, 0.5f});
        label->setPosition(ccp(winSize.width / 2 - 145, winSize.height / 2 + y));
        y -= 20;
    }
}

void HaxOverlay::toggler(CCObject* sender) {
    CCMenuItem* menuItem = (CCMenuItem *)(sender);
    std::string* userData = static_cast<std::string*>(menuItem->getUserData());
    auto& hax = HaxManager::sharedState();
    cocos2d::CCLog(userData->c_str());
    hax.getModule(userData->c_str())->toggle();
}

void HaxOverlay::keyBackClicked() {
    HaxOverlay::onClose(nullptr);
}

void HaxOverlay::onClose(CCObject*) {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    
    this->setKeypadEnabled(false);
    //referrer->setTouchEnabled(true);
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

    mainParent->runAction(CCEaseIn::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width, 0.0f)), 3
    ));
    runAction(CCSequence::create(CCArray::create(
        CCDelayTime::create(getDuration()),
        CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParentAndCleanup)) // CCRemoveSelf does not exist in old cocos
    )));
}

bool HaxOverlay::ccTouchBegan(cocos2d::CCTouch* t, cocos2d::CCEvent*)
{ 
    return true;
}

// from starry sky. never in a million years would i have figured ts out without it
void HaxOverlay::registerWithTouchDispatcher()
{
    CCTouchDispatcher* t = CCDirector::sharedDirector()->getTouchDispatcher();   
    t->addTargetedDelegate(this, 0x80000005, true);
}