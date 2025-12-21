#pragma once
#ifndef _OBJECTS_PLAYEROBJECT_HPP
#define _OBJECTS_PLAYEROBJECT_HPP

#include "gameobject.hpp"

class PlayerObject : public GameObject {
public:
    void gravityUp();
    void gravityDown();
    void speedUp(void);
};

#endif
