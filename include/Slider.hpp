#pragma once

#include <cocos2d.h>
#include "SliderThumb.hpp"

class Slider : cocos2d::CCLayer
{
public:
	// static Slider* create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);

	// bool init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);
    
	// virtual void updateBar();
	// SliderThumb* getThumb();
	static Slider* create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);

	static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler handler, float scale);

	static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler handler);

	bool getLiveDragging();
	SliderThumb* getThumb();
	float getValue();
	bool init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);
	void setBarVisibility(bool);
	void setLiveDragging(bool);
	void setValue(float);
	void updateBar();

	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
};
