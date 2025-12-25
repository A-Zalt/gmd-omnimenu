#pragma once

#include <cocos2d.h>
#include "CCTextInputNode.hpp"
#include "HaxMenu.hpp"

class RespawnTimeInput : public CCLayer, public TextInputDelegate {
public:
    HaxMenu* parent;
    CCTextInputNode* input;
    bool ignore;
    bool dontChange;

    static RespawnTimeInput* create(HaxMenu* parent);
    bool init(HaxMenu* parent);
    
    // must be called after create
    void setupDelegates();
    void createTheThings();

    virtual void textChanged(CCTextInputNode* input) override;
};