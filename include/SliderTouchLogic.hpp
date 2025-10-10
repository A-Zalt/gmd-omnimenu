#pragma once

#include <cocos2d.h>
#include "Slider.hpp"
#include "SliderThumb.hpp"

class SliderTouchLogic : cocos2d::CCMenu
{
public:
    static SliderTouchLogic* create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, float);
    
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual Slider* getSliderDelegate();
    virtual cocos2d::CCPoint getTouchOffset();
    virtual SliderThumb* getThumb();
    virtual bool getLiveDragging();

    float m_unknownUnused;
	float m_length;
	cocos2d::CCMenuItemImage* m_thumb;
	Slider* m_slider; // m_sliderDelegate
	bool m_activateThumb; // m_liveDragging
	cocos2d::CCPoint m_position; // m_touchOffset
};
