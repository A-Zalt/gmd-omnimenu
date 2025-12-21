#pragma once
#ifndef _LAYERS_EDITOR_CUSTOMIZEOBJECTLAYER_HPP
#define _LAYERS_EDITOR_CUSTOMIZEOBJECTLAYER_HPP

#include <cocos2d.h>
#include "../include/cocos2dx/robtop/layers/flalertlayer.hpp"
#include "robtop/sprites/buttonsprite.hpp"

class FLAlertLayer;
class ButtonSprite;

class CustomizeObjectLayer : public FLAlertLayer {
public:
    void onSelectColor(cocos2d::CCObject*);
    void highlightSelected(ButtonSprite*);

};

#endif