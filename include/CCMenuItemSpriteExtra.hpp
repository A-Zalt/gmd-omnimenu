#pragma once

#include <cocos2d.h>

using namespace cocos2d;

class CCMenuItemSpriteExtra : public CCMenuItemSprite
{
public:
    virtual ~CCMenuItemSpriteExtra();

    virtual void activate();
    virtual void selected();
    virtual void unselected();
    
    CC_SYNTHESIZE(float, m_fScaleVar, ScaleVar);
    CC_SYNTHESIZE(float, m_fOriginalScale, OriginalScale);
    CC_SYNTHESIZE(bool, m_bShouldAnimate, ShouldAnimate);
    CC_SYNTHESIZE(bool, m_bDarkenClick, DarkenClick);
    CC_SYNTHESIZE(float, m_fVolume, Volume);
    CC_SYNTHESIZE(const char*, m_pClickSound, ClickSound);

    static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCNode* unk, cocos2d::CCObject* target, SEL_MenuHandler callback);
    void setSizeMult(float);
};
