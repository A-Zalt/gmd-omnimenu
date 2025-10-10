#pragma once

#include <cocos2d.h>

class SliderThumb : cocos2d::CCMenuItemImage
{
public:
	static SliderThumb* create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*);

	bool init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*);
};
