#pragma once

#include <cocos2d.h>
#include "CCTextInputNode.hpp"
#include "LevelEditorLayer.hpp"
#include "PauseLayer.hpp"

void glLineWidth(GLfloat width);

class HitboxLayer : public CCLayer {
public:
    PlayLayer* parent;

    static HitboxLayer* create(PlayLayer* parent);
    bool init(PlayLayer* parent);
    virtual void draw() override;
};
class HitboxLayerEditor : public CCLayer {
public:
    LevelEditorLayer* parent;

    static HitboxLayerEditor* create(LevelEditorLayer* parent);
    bool init(LevelEditorLayer* parent);
    virtual void draw() override;
};