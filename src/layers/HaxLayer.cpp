#include "../layers/HaxLayer.hpp"

HaxLayer* HaxLayer::create(bool fromRope) {
    auto ret = new HaxLayer();
    if (ret->init(fromRope)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
};

cocos2d::CCScene* HaxLayer::scene(bool fromRope) {
    auto layer = HaxLayer::create(fromRope);
    auto scene = cocos2d::CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool HaxLayer::init(bool fromRope) {
    if(!cocos2d::CCLayer::init())
        return false;

    auto director = cocos2d::CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    m_background = cocos2d::CCSprite::create("menubackground.png");
    m_background->setAnchorPoint({ 0.f, 0.f });
    m_background->setColor({66, 39, 133});
    addChild(m_background, -2);

    CCSprite* backSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(HaxLayer::onClose));

    backBtn->setPosition(ccp(-winSize.width / 2 + 23.f, winSize.height / 2 - 25.f));

    addChild(backBtn);

    return true;
}

void HaxLayer::keyBackClicked() {
    HaxLayer::onClose(nullptr);
}

void HaxLayer::onClose(cocos2d::CCObject*) {
    this->retain();
    this->removeFromParentAndCleanup(false);

    cocos2d::CCDirector::sharedDirector()->replaceScene(cocos2d::CCTransitionFadeTR::create(0.5f, MenuLayer::scene(false)));

    this->release();

    setKeyboardEnabled(false);
    setKeypadEnabled(false);
}

// 