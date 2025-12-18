#pragma once

#include <cocos2d.h>
#include "CCTextInputNode.hpp"
#include "PauseLayer.hpp"

class SpeedhackInput : public CCLayer, public TextInputDelegate {
public:
    PauseLayer* parent;
    CCTextInputNode* input;
    bool ignore;
    bool dontChange;

    static SpeedhackInput* create(PauseLayer* parent);
    bool init(PauseLayer* parent);
    
    // must be called after create
    void setupDelegates();
    void createTheThings();

    virtual void textChanged(CCTextInputNode* input) override;
};