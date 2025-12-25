#pragma once

#include <cocos2d.h>
#include "FLAlertLayer.hpp"

class PauseLayer : public cocos2d::CCLayerColor {
public:
    static PauseLayer* create(void*);
    void onOpenMenu();
    void createSpeedhack();
    void createEye();
    void toggleVisibility();
};

class ConfirmExit : public FLAlertLayerProtocol {
public:
    PauseLayer* pauseLayer;

    void FLAlert_Clicked(FLAlertLayer*, bool btn2) override;
};