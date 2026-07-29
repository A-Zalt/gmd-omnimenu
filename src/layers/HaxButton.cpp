#include "HaxButton.hpp"
#include <cocos2d.h>
#include "HaxMenu.hpp"
#include "VersionUtils.hpp"

using namespace cocos2d;

HaxButton* HaxButton::create() {
    HaxButton* ret = new HaxButton();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void HaxButton::onOpenMenu(CCObject*) {
    auto haxOverlay = HaxMenu::create(this); //HaxOverlay::create(this);
#if GAME_VERSION < GV_1_7
    MEMBER_BY_OFFSET(CCScene*, CCDirector::sharedDirector(), 0x98)->addChild(haxOverlay, 1000);
#else
    CCDirector::sharedDirector()->getRunningScene()->addChild(haxOverlay, 1000);
#endif
    m_sMenu->setEnabled(false);
    m_sMenu->setVisible(false);
    // setVisible(false);
    // m_movable = false;
}

void HaxButton::onSceneTransition() {
    auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    // dispatcher->removeDelegate(m_sMenu);
    // dispatcher->addTargetedDelegate(m_sMenu, INT_MIN, true);
}

void HaxButton::resetVisibility() {
    if (!m_sMenu) {
        CCLog("NO MENU!");
        auto menu = CCMenu::create(m_sSprite, NULL);
        m_sSprite->setTag(HAX_BUTTON_TAG);
        m_sMenu = menu;
        m_sMenu->retain();
    }
    m_sMenu->setEnabled(true);
    m_sMenu->setVisible(true);
    // setVisible(true);
    // m_movable = true;
}

bool HaxButton::init() {
    CCSprite* menuSpr = CCSprite::create("OMNImenu_btn.png");
    m_sSprite = CCMenuItemSpriteExtra::create(menuSpr, menuSpr, this, menu_selector(HaxButton::onOpenMenu));
    auto menu = CCMenu::create(m_sSprite, NULL);
    m_sSprite->setTag(HAX_BUTTON_TAG);
    m_sMenu = menu;
    m_sMenu->retain();
    this->scheduleUpdate();
    this->onEnter();     
    m_movable = true;   
    // auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    // dispatcher->addTargetedDelegate(menu, INT_MIN, true); 
    // menu->setOpacity(128);
    this->addChild(menu);
    return true;
}

void HaxButton::fade(bool fill) {
    m_sMenu->stopAllActions(); 
    GLubyte opacity = fill ? 255 : 128;
    auto fade = CCFadeTo::create(0.2f, opacity);
    CCDirector::sharedDirector()->getActionManager()->addAction(fade, m_sMenu, false);
}

bool HaxButton::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    // touch dispatcher is dumb i had to replicate ccmenuitemspriteextra
    if (!m_movable) return true;
    auto location = getTouchLocation(touch);
    // CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
    CCPoint localPos = m_sSprite->convertToNodeSpace(location);
    CCLog("%f %f", localPos.x, localPos.y);
    CCRect rect = CCRectMake(0, 0, m_sSprite->getContentSize().width, m_sSprite->getContentSize().height);
#if GAME_VERSION >= GV_1_7
    if (rect.containsPoint(localPos)) {
#else
    if (CCRect::CCRectContainsPoint(rect, localPos)) {
#endif
        m_sSprite->activate();
        this->fade(true);
        return true; 
    }
    return false;
}

void HaxButton::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    CCLog("1");
    if (!m_movable) return;
    CCLog("2");
    m_bMoved = true;
    CCLog("3");
    CCPoint currentLoc = getTouchLocation(touch);
    CCLog("4");
    CCPoint previousLoc = getPreviousTouchLocation(touch);
    CCLog("5");
    CCPoint diff = ccpSub(currentLoc, previousLoc);
    CCLog("6");
    CCPoint currentPos = this->getPosition();
    CCLog("7");
    this->setPosition(ccpAdd(currentPos, diff));
    CCLog("8");
    CCPoint localPos = m_sSprite->convertToNodeSpace(currentLoc);
    CCLog("9");
    CCRect rect = CCRectMake(0, 0, m_sSprite->getContentSize().width, m_sSprite->getContentSize().height);
#if GAME_VERSION >= GV_1_7
    CCLog("10");
    if (rect.containsPoint(localPos)) {
#else
    if (CCRect::CCRectContainsPoint(rect, localPos)) {
#endif
        CCLog("11");
        if (!m_sSprite->isSelected()) {
            CCLog("12");
            m_sSprite->selected();
            CCLog("13");
            this->fade(true);
        }
        CCLog("14");
    } else {
        CCLog("15");
        if (m_sSprite->isSelected()) {
            CCLog("16");
            m_sSprite->unselected();
            CCLog("17");
            this->fade(false);
        }
        CCLog("18");
    }
    CCLog("19");
}

void HaxButton::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    if (!m_movable) return;
    auto location = getTouchLocation(touch);
    CCPoint localPos = m_sSprite->convertToNodeSpace(location);
    CCRect rect = CCRectMake(0, 0, m_sSprite->getContentSize().width, m_sSprite->getContentSize().height);
    m_sSprite->unselected();
    this->fade(false);

#if GAME_VERSION >= GV_1_7
    if (rect.containsPoint(localPos) && m_sMenu->isEnabled() && !m_bMoved) {
        onOpenMenu(nullptr);
#else
    if (CCRect::CCRectContainsPoint(rect, localPos) && m_sMenu->isEnabled() && !m_bMoved) {
        m_sSprite->activate();
#endif
    }
    m_bMoved = false;
}

void HaxButton::setup() {
    CCDirector::sharedDirector()->setNotificationNode(this);
    auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    dispatcher->addTargetedDelegate(this, INT_MIN, true);
}