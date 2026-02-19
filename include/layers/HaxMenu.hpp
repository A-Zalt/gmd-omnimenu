#pragma once

#include "constants.hpp"
#include <cocos2d.h>
#include <Module.hpp>
#include "CCMenuItemSpriteExtra.hpp"
#if GAME_VERSION >= GV_1_7
#include "cocos2d-2.2.3/cocos2dx/layers_scenes_transitions_nodes/CCLayer.h"
#endif

// #if GAME_VERSION < GV_1_7
// #else
// class HaxMenu : public CCLayer
// #endif
class HaxMenu : public CCLayerColor
{
public:
    static HaxMenu* create(CCLayer* referrer);
    static cocos2d::CCScene* scene();

protected:
    bool init(CCLayer* referrer);
    void onClose(CCObject*);
    void keyBackClicked();

    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void addButton(const char* label, float fontSize, float yOffset, CCObject* target, SEL_MenuHandler selector);
    // void addButtonRight(const char* label, float fontSize, float yOffset, CCObject* target, SEL_MenuHandler selector);
    void onCategory(ModuleCategory category);
    void onPlayer(SEL_MenuHandler_1_7_compat);
    void onVisual(SEL_MenuHandler_1_7_compat);
    void onEditor(SEL_MenuHandler_1_7_compat);
    void onBypass(SEL_MenuHandler_1_7_compat);
    void onInformational(SEL_MenuHandler_1_7_compat);
    void onUniversal(SEL_MenuHandler_1_7_compat);
    void onLabel(SEL_MenuHandler_1_7_compat);
    void onParticles(SEL_MenuHandler_1_7_compat);
    void onSaveData(SEL_MenuHandler_1_7_compat);

    void cleanupMembers();

    void toggler(CCObject*);
    void modInfo(CCObject*);
    float getDuration();
    void setColorAtIndex(int index);
    void onPih(CCObject*);
    void onUDID();
    void onThree(CCObject*);
    void onExportData(SEL_MenuHandler_1_7_compat);
    void onImportData(CCObject* sender);

    CCNode* leftParent;
    CCNode* rightParent;
    CCLayer* referrer;
    CCArray* catButtons;
    CCMenu* catMenu;
    CCMenu* modMenu;
    CCMenuItemSpriteExtra* udidBtn;
    CCSprite* leftPanel;
    CCSprite* rightPanel;
    bool canSpoofPih;
};