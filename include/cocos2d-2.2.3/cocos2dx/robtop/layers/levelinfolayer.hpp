#pragma once
#ifndef _LAYERS_LEVELINFOLAYER_HPP
#define _LAYERS_LEVELINFOLAYER_HPP

#include <cocos2d.h>

#include <robtop/layers/flalertlayer.hpp>

class LevelInfoLayer : public cocos2d::CCLayer {
public:
    int init(GJGameLevel*);
    void setupLevelInfo();
    void onPlayReplay(cocos2d::CCObject*);

    bool shouldDownloadLevel();

    void onClone(cocos2d::CCObject*);
};

class LevelFeatureLayer : public FLAlertLayer {
public:
    static LevelFeatureLayer* create(int idk);
};

#endif
