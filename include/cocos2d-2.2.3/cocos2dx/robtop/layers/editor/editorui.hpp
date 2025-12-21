#pragma once
#ifndef _LAYERS_EDITOR_EDITORUI_HPP
#define _LAYERS_EDITOR_EDITORUI_HPP

#include <cocos2d.h>

#include <robtop/layers/editor/leveleditorlayer.hpp>

enum EditCommand
{
    kEditCommandSmallLeft = 1,
    kEditCommandSmallRight = 2,
    kEditCommandSmallUp = 3,
    kEditCommandSmallDown = 4,

    kEditCommandLeft = 5,
    kEditCommandRight = 6,
    kEditCommandUp = 7,
    kEditCommandDown = 8,

    kEditCommandBigLeft = 9,
    kEditCommandBigRight = 10,
    kEditCommandBigUp = 11,
    kEditCommandBigDown = 12,

    kEditCommandTinyLeft = 13,
    kEditCommandTinyRight = 14,
    kEditCommandTinyUp = 15,
    kEditCommandTinyDown = 16,

    kEditCommandFlipX = 17,
    kEditCommandFlipY = 18,
    kEditCommandRotateCW = 19,
    kEditCommandRotateCCW = 20,
};

class EditorUI : public cocos2d::CCLayer {
public:
    void setupCreateMenu();
    CreateMenuItem* getCreateBtn(int objectID, int categoryID, bool highlighted);
    CreateMenuItem* getCreateBtn(int objectID, int categoryID);

    CreateMenuItem* getSpriteButton(char const* idk, cocos2d::SEL_MenuHandler* selector, cocos2d::CCMenu* menu, float idk2);

    void moveObjectCall(CCObject* p0);
    void moveObjectCall(EditCommand command);
    void transformObjectCall(EditCommand command);

    void onCopy(cocos2d::CCObject*);
    std::string copyObjects(cocos2d::CCArray*);

    cocos2d::CCArray* getSelectedObjects();

    void rotateObjects(cocos2d::CCArray* array, float angle, cocos2d::CCPoint center);

    void updateButtons();

    int init(LevelEditorLayer*);
};

#endif
