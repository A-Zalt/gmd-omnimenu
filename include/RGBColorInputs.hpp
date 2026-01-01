#pragma once

#include <cocos2d.h>
#include "CCTextInputNode.hpp"

#define RED_INPUT_TAG 1056817
#define GREEN_INPUT_TAG 2056817
#define BLUE_INPUT_TAG 3056817
#define RGBCOLORINPUTS_TAG 63584276

class RGBColorInputs : public CCLayer, public TextInputDelegate {
public:
    cocos2d::extension::CCControlColourPicker* parent;
    CCTextInputNode* redInput;
    CCTextInputNode* greenInput;
    CCTextInputNode* blueInput;
    CCTextInputNode* ftInput;
    bool ignore;
    bool dontChange;
    bool hasInitializedColor;
    _ccColor3B colorAtInit;
    _ccColor3B currentColor;
    bool colorPicker;
    ColorSelectPopup* csp;
    bool editingFT;

    void initColor(_ccColor3B color) {
        if (hasInitializedColor) return;
        hasInitializedColor = true;
        colorAtInit = color;
    }

    static RGBColorInputs* create(cocos2d::extension::CCControlColourPicker* parent);
    bool init(cocos2d::extension::CCControlColourPicker* parent);
    
    // must be called after create
    void setupDelegates();
    void createTheThings();
    void createFTInput();
    void setupFTDelegate();

    virtual void textChanged(CCTextInputNode* input) override;
    // virtual void textInputOpened(CCTextInputNode* input) override;
    // virtual void textInputClosed(CCTextInputNode* input) override;
    virtual void draw() override;
};