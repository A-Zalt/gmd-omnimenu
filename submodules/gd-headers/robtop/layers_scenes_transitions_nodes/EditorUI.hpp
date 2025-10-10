#pragma once

#include <cocos2d.h>
#include "layers_scenes_transitions_nodes/LevelEditorLayer.hpp"
#include "layers_scenes_transitions_nodes/FLAlertLayer.hpp"

using namespace cocos2d;

class ColorSelectPopup;
class ColorSelectDelegate
{
public:
    virtual void colorSelectClosed(ColorSelectPopup*);
};

// incomplete definition
class EditButtonBar : public CCNode
{
public:
    static EditButtonBar* create(CCArray* obj_array, CCPoint unk);
};

class EditorUI : public CCLayer, public FLAlertLayerProtocol, public ColorSelectDelegate
{
public:
    EditButtonBar* m_pEditButtonBarForCreateMenu;
    int m_pEditButtonBarForMoveMenu;
    int m_pSlider;
    int d;
    int m_fEditBarHeightPlusY;
    float m_fEditBarHeight;
    bool m_bToggleSwipe;
    CCPoint *pnt1_160;
    int h[7];
    cocos2d::CCArray *m_pArr2;
    cocos2d::CCMenu *m_pDeleteMenu;
    void *m_pDeleteBtn;
    void *m_pBuildBtn;
    void *m_pEditBtn;
    CCMenuItemSpriteExtra *m_pSwipeBtn;
    CCMenuItemSpriteExtra *m_pDeselectBtn;
    CCMenuItemSpriteExtra *m_pZoomInBtn;
    CCMenuItemSpriteExtra *m_pZoomOutBtn;
    CCMenuItemSpriteExtra *m_pUndoBtn;
    CCMenuItemSpriteExtra *m_pRedoBtn;
    int frer[3];
    std::string m_sSelectedObjectTextureNameMaybeIdkIFoundItOnCreate;
    cocos2d::CCArray *m_pUnkArrayOnSetupCreateMenu;
    int m_eEditMode;
    LevelEditorLayer *m_pEditor;
    CCPoint m_pUnkPoint;
    CCPoint m_punkpoint2;
    CCPoint m_pUnkPoint3;
    CCPoint m_pUnkPoint4;
    GameObject *m_pSelectedObject;
    int unk;

    virtual ~EditorUI();
    virtual void draw();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual int getEditMode();
    virtual void setEditMode(int var);

    // no virtuals or variables yet.

    void setupCreateMenu();
    CCNode* /*CreateMenuItem*/ getCreateBtn(const char* sprite, int button_sprite_id);
    void updateCreateMenu();
    void onCreate();
};
