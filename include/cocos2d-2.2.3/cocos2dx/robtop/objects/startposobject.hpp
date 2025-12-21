#pragma once
#ifndef _OBJECTS_STARTPOSOBJECT_HPP
#define _OBJECTS_STARTPOSOBJECT_HPP

#include <cocos2d.h>

#include <robtop/objects/gameobject.hpp>

class StartPosObject : public GameObject {
public:
    static StartPosObject* create();
};

#endif
