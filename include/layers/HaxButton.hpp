#pragma once
#include <cocos2d.h>
#include "CCMenuItemSpriteExtra.hpp"
using namespace cocos2d;

class HaxButton : public CCLayer {
public:
    CCMenu* m_sMenu;
    CCMenuItemSpriteExtra* m_sSprite;
    bool m_bMoved;
    bool m_movable;
    static HaxButton* create();
    virtual bool init();
    void onOpenMenu(CCObject*);
    void onSceneTransition();
    void resetVisibility();
    void fade(bool);
    virtual bool ccTouchBegan(CCTouch*, CCEvent*) override;
    virtual void ccTouchEnded(CCTouch*, CCEvent*) override;
    virtual void ccTouchMoved(CCTouch*, CCEvent*) override;
    void setup();
};
#define HAX_BUTTON_TAG 83647364