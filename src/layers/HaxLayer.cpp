#include "../layers/HaxLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "MenuLayer.hpp"

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
    addChild(m_background, -2);

    CCMenu* backMenu = CCMenu::create();

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, backSpr, this, menu_selector(HaxLayer::onClose));

    backMenu->addChild(backBtn);
    backMenu->setPosition(ccp(25.f, winSize.height - 25.f));

    addChild(backMenu, 1);

    return true;
}

void HaxLayer::keyBackClicked() {
    HaxLayer::onClose(nullptr);
}

void HaxLayer::onClose(CCObject*) {
    this->retain();
    this->removeFromParentAndCleanup(false);

    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, MenuLayer::scene()));

    this->release();
}

// 