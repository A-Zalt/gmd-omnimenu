#pragma once

#include <cocos2d.h>
#include "CCTextInputNode.hpp"
#include "PauseLayer.hpp"

void glLineWidth(GLfloat width);

class HitboxLayer : public CCLayer {
public:
    PlayLayer* parent;
#if GAME_VERSION >= GV_1_7
    CCDrawNode* drawNode;
#endif

    static HitboxLayer* create(PlayLayer* parent);
    bool init(PlayLayer* parent);
    virtual void draw() override;
};