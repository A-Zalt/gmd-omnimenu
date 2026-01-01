#pragma once

#include <cocos2d.h>
#include "GameObject.hpp"

enum GhostType {
    None = 0,
    Ghost = 1
};

class PlayerObject : public GameObject {
public:
    static PlayerObject* create(int, cocos2d::CCLayer*);
    bool init(void*);
    void lockPlayer();
    void gravityDown();
    void gravityUp();
    void speedDown();
    void speedUp();
    void yStartDown();
    void yStartUp();
    void logValues();

    void deactivateParticle();
    void flipMod();
    void stopRotation();
    void toggleGhostEffect(GhostType type);
    void touchedObject(GameObject* object);
#if GAME_VERSION >= GV_1_4
    void updatePlayerShipFrame(int id);
    void updatePlayerScale(); // exists since 1.5, but reimplemented by OMNImenu in 1.4 for Ship Gravity Bug Fix
#endif
};
