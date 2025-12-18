#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"
#include "GameObject.hpp"
#include "UndoObject.hpp"

struct OrderingData
{
    int order_of_arrival;
    int z_order;
};

class LevelEditorLayer : public cocos2d::CCLayer
{
public:
    static LevelEditorLayer* create(GJGameLevel* level);
    bool init(GJGameLevel* level);

    void addToSection(GameObject* object);
    void addToUndoList(UndoObject*);
    void removeObject(GameObject*);
    int getSectionCount();
#if GAME_VERSION >= GV_1_6
    void addObjectFromString(std::string);
#endif
};