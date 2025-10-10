#pragma once

#include <cocos2dcpp.h>

class CCMenuItemToggler : public cocos2d::CCMenuItem
{
public:
    static CCMenuItemToggler* create(cocos2d::CCNode* off, cocos2d::CCNode* on,
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
    
    void setSizeMult(float);
    void toggle(bool);
};
