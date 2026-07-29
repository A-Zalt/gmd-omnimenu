#pragma once

#include <cocos2d.h>
#if GAME_VERSION >= GV_1_7
#include "cocos2d-2.2.3/cocos2dx/layers_scenes_transitions_nodes/CCLayer.h"
#endif

// #if GAME_VERSION < GV_1_7
// #else
// class HaxMenu : public CCLayer
// #endif
class CustomScrollLayer : public CCLayerColor
{
public:
    float m_height;
    CCTouch* m_touch;

    static CustomScrollLayer* create();
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);

    void calculateHeight();
};