#pragma once

#include <cocos2d.h>
#include "GameObject.hpp"
#include "FLAlertLayer.hpp"
#include "CCTextInputNode.hpp"

class ColorSelectPopup : public FLAlertLayer {
public:
    static ColorSelectPopup* create(GameObject* object);
    bool init(GameObject* object);
    _ccColor3B getColorValue();
    void selectColor(_ccColor3B color);

    void onCopy();
    void onPaste();
    void updateDurLabel();
};

class ColorPickerPopup : public FLAlertLayer {
public:
    static ColorSelectPopup* create(GameObject* object);
    bool init(GameObject* object);
    _ccColor3B getColorValue();
    void selectColor(_ccColor3B color);

    void onCopy();
    void onPaste();
};