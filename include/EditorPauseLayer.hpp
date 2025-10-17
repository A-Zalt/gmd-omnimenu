#pragma once

#include <cocos2d.h>
#include "LevelEditorLayer.hpp"

class EditorPauseLayer : public cocos2d::CCLayer {
public:
    static EditorPauseLayer* create(LevelEditorLayer*);
    void onOpenMenu();
};
