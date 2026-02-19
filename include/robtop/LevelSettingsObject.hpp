#pragma once

#include <cocos2d.h>

class LevelSettingsObject : public cocos2d::CCNode
{
public:
    static LevelSettingsObject* objectFromString(std::string str);
    void setAudioTrack(int);
};
