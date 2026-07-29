#pragma once

#include "constants.hpp"
#include <cocos2d.h>
#include "FLAlertLayer.hpp"
#include "GameObject.hpp"
#include "CCMenuItemSpriteExtra.hpp"

class CreateMenuItem : public CCMenuItemSpriteExtra {};

struct SelectionContext {
    float centerX, centerY, minX, minY, maxX, maxY;
};

class EditorUI : public cocos2d::CCLayer
{
public:
    static EditorUI* create();
    bool init();
#if GAME_VERSION < GV_1_7
    void zoomOut();
    void zoomOutExtra();
#else
    void zoomOut(CCObject*);
    void zoomOutExtra(CCObject*);
#endif

// #if GAME_VERSION < 7
//     void keyBackClicked();
// #endif
    void onPause();
    void deselectAll();
    void deselectObject();
    void selectObject(GameObject*);
#if GAME_VERSION < GV_1_7
    void onDeleteSelected();
#else
    void onDeleteSelected(CCObject*);
#endif
#if GAME_VERSION < GV_1_5
    void updateCreateMenu();
#else
    void updateCreateMenu(bool);
#endif
    CreateMenuItem* getCreateBtn(const char*, int);
#if GAME_VERSION < GV_1_5
    GameObject* duplicateObject(GameObject* obj, void* getSaveString);
    void onDuplicate();
#endif
#if GAME_VERSION < GV_1_6
    void onDeleteStartPos();
#endif

    CCMenuItemSpriteExtra* getSpriteButton(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale);
    CCMenuItemSpriteExtra* getSpriteButton2(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale);
    CCMenuItemSpriteExtra* getSpriteButton3(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale, float scale2);
#if GAME_VERSION < GV_1_7
    void moveObjectCall(CCNode* sender);
    void transformObjectCall(CCNode* sender);
#else
    void moveObjectCall(CCObject* sender);
    void transformObjectCall(CCObject* sender);
    void selectBuildTab(int tab);
#endif
    void moveObject(GameObject* obj, CCPoint transform);
    void editObject();

    void moveObjectCall2(CCNode* sender);
    void transformObjectCall2(CCNode* sender);
    
    void onSelectFilter();
    int getSelectedObjectID();
    
    void onPlay(CCObject*);
    void stopMusic(CCObject*);

    void rotateObjects(float rot, int tag);
    SelectionContext getSelectionContext();
};
