#pragma once
#include <cocos2d.h>

class ButtonSprite : public cocos2d::CCSprite {

public:
    static ButtonSprite* create(cocos2d::CCSprite*, int, int, float, float, bool, char const*);
    static ButtonSprite* create(char const*, float);

    bool init(char const*, int, int, float, bool, char const*, char const*, float);
	bool init(cocos2d::CCSprite*, int, int, float, float, bool, char const*);
};