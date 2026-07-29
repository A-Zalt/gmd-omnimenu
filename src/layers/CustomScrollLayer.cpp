#include "../layers/CustomScrollLayer.hpp"
#include "VersionUtils.hpp"

using namespace cocos2d;

CustomScrollLayer* CustomScrollLayer::create() {
    auto ret = new CustomScrollLayer;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
bool CustomScrollLayer::init() {
    m_height = 0;
    CCLog("scroll init");
    return true;
}

void CustomScrollLayer::calculateHeight() {
    if (getChildrenCount() < 1) {
        m_height = 0;
        return;
    }
    float minHeight = FLT_MAX;
    float maxHeight = -FLT_MAX;
    for (int i = 0; i < getChildrenCount(); i++) {
        auto child = static_cast<CCNode*>(getChildren()->objectAtIndex(i));
        if (child) {
            if (minHeight > child->getPositionY()) minHeight = child->getPositionY();
            if (maxHeight < child->getPositionY()) maxHeight = child->getPositionY();
        }
    }
    m_height = maxHeight - minHeight;
}
bool CustomScrollLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    CCLog("scroll ccTouchBegan");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    if (m_height < winSize.height) return false;
    m_touch = pTouch;
    return true;
}
void CustomScrollLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent) {
    CCLog("scroll ccTouchMoved");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    if (m_height < winSize.height) return;
    
    setPositionY(getPositionY() + getTouchLocation(pTouch).y - getTouchLocation(m_touch).y);
    
    m_touch = pTouch;
}