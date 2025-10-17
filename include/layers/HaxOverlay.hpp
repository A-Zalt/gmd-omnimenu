#pragma once

#include <cocos2d.h>
#include <Module.hpp>

class HaxOverlay : public CCLayerColor {

public:
    static HaxOverlay* create(CCLayer* referrer);
    static cocos2d::CCScene* scene();

protected:
    bool init(CCLayer* referrer);
    void onClose(CCObject*);
    void keyBackClicked();

    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void addButton(const char* label, int fontSize, float yOffset, CCObject* target, SEL_MenuHandler selector);
    void onCategory(ModuleCategory category);
    void onGameplay();
    void onEditor();
    void onBypass();
    void onInformational();
    void onUniversal();

    void toggler(CCObject*);
    float getDuration();

    CCNode* mainParent;
    CCNode* categoryParent;
    CCLayer* referrer;
    CCMenu* btnMenu;
    CCMenu* categoryMenu;
};