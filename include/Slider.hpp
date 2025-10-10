#pragma once

#include <cocos2d.h>
#include "SliderThumb.hpp"

class Slider : cocos2d::CCLayer
{
public:
	static Slider* create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);

	bool init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);
    
	void updateBar();
	SliderThumb* getThumb();
};
