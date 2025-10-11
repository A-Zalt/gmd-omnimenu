#pragma once

#include <cocos2d.h>
#include "FLAlertLayer.hpp"

class EditorUI : public cocos2d::CCLayer
{
public:
    static EditorUI* create();
    bool init();

    virtual void onCreateObject(const char* param1);
};
