#pragma once
#ifndef _LAYERS_SCROLLINGLAYER_HPP
#define _LAYERS_SCROLLINGLAYER_HPP

#include <cocos2d.h>

class ScrollingLayer : public cocos2d::CCLayerColor {
public:
    float m_fLayerHeight;
    cocos2d::CCLayer *m_pScrollLayer;
    cocos2d::CCNode *m_pParent;

    static ScrollingLayer *create(cocos2d::CCSize _size, cocos2d::CCPoint _point, float _unknown);
};

#endif
