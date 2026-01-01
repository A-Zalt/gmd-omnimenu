#include "hook.hpp"
#include "PlayerObject.hpp"

void (*TRAM_PlayerObject_activateStreak)(PlayerObject* self);
void (*TRAM_PlayerObject_deactivateStreak)(PlayerObject* self);
void PlayerObject_activateStreak(PlayerObject* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_TRAIL)) {
        if (!hax.getModuleEnabled(ModuleID::TRAIL_ALWAYS_ON)) return;
        TRAM_PlayerObject_deactivateStreak(self);
        return;
    }
    TRAM_PlayerObject_activateStreak(self);
}
void PlayerObject_deactivateStreak(PlayerObject* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::TRAIL_ALWAYS_ON)) {
        if (!hax.getModuleEnabled(ModuleID::NO_TRAIL)) return;
        TRAM_PlayerObject_activateStreak(self);
        return;
    }
    TRAM_PlayerObject_deactivateStreak(self);
}
#if GAME_VERSION > GV_1_0
void (*TRAM_PlayerObject_updateShipRotation)(PlayerObject* self, float dt);
void PlayerObject_updateShipRotation(PlayerObject* self, float dt) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_ROTATION)) return;
    TRAM_PlayerObject_updateShipRotation(self, dt);
}
#else
void (*TRAM_PlayerObject_updateShipRotation)(PlayerObject* self);
void PlayerObject_updateShipRotation(PlayerObject* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_ROTATION)) return;
    TRAM_PlayerObject_updateShipRotation(self);
}
#endif
void (*TRAM_PlayerObject_runRotateAction)(PlayerObject* self);
void PlayerObject_runRotateAction(PlayerObject* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_ROTATION)) return;
    TRAM_PlayerObject_runRotateAction(self);
}
void (*TRAM_PlayerObject_runBallRotation2)(PlayerObject* self);
void PlayerObject_runBallRotation2(PlayerObject* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_ROTATION)) return;
    TRAM_PlayerObject_runBallRotation2(self);
}
void (*TRAM_PlayerObject_update)(PlayerObject* self, float dt);
void PlayerObject_update(PlayerObject* self, float dt) {
    TRAM_PlayerObject_update(self, dt);
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_SHIP_GROUND)) {
        auto p = getShipGroundParticles(self);
        if (p && p != nullptr) p->stopSystem();
    }
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_SHIP_LIFT)) {
        auto p = getShipLiftParticles(self);
        if (p && p != nullptr) p->stopSystem();
    }
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_SHIP_FIRE)) {
        auto p = getShipFireParticles(self);
        if (p && p != nullptr) p->stopSystem();
    }
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_GROUND)) {
        auto p = getGroundParticles(self);
        if (p && p != nullptr) p->stopSystem();
    }
    if (hax.getModuleEnabled(ModuleID::JUMP_HACK)) {
        setOnGround(self, true);
    }
}
void (*TRAM_PlayerObject_hitGround)(PlayerObject* self, bool b1);
void PlayerObject_hitGround(PlayerObject* self, bool b1) {
    TRAM_PlayerObject_hitGround(self, b1);
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_LANDING)) {
        auto l2 = getLandingParticles2(self);
        if (l2 && l2 != nullptr) l2->stopSystem();
        auto l1 = getLandingParticles(self);
        if (l1 && l1 != nullptr) l1->stopSystem();
    }
}
#if GAME_VERSION >= GV_1_5
void (*TRAM_PlayerObject_playBurstEffect)(PlayerObject* self);
void PlayerObject_playBurstEffect(PlayerObject* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::PARTICLE_BURST)) TRAM_PlayerObject_playBurstEffect(self);
}
#endif
void (*TRAM_PlayerObject_playerDestroyed)(PlayerObject* self);
void PlayerObject_playerDestroyed(PlayerObject* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_DEATH_EFFECT)) {

        if (MEMBER_BY_OFFSET(float, self, PlayerObject__m_unk1) != 0) {
            getPlayLayer()->removeLastCheckpoint();
            MEMBER_BY_OFFSET(float, self, PlayerObject__m_unk1) = 0;
        }
        MEMBER_BY_OFFSET(bool, self, PlayerObject__m_unk2) = true;
        self->stopRotation();
        self->deactivateParticle();
        self->touchedObject(nullptr);
        getShipFireParticles(self)->stopSystem();
        getShipLiftParticles(self)->stopSystem();
        getShipGroundParticles(self)->stopSystem();
        self->toggleGhostEffect(GhostType::None);

    } else TRAM_PlayerObject_playerDestroyed(self);
}
void (*TRAM_PlayerObject_levelFlipFinished)(PlayerObject* self);
void PlayerObject_levelFlipFinished(PlayerObject* self) {
    TRAM_PlayerObject_levelFlipFinished(self);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.hitboxLayer) {
        hax.hitboxLayer->setScaleX(getIsFlipped() ? -1 : 1);    
    }
}

#if GAME_VERSION == GV_1_4
// decompiled function from 1.5
void PlayerObject::updatePlayerScale() {
    stopActionByTag(5); // scaling action
    setScaleX(getPlayerScale(this));

    // in 1.5 this checks for Ship OR UFO, but 1.4 doesn't have UFO, so we only check for ship
    if (getFlyMode(this) && getGravityFlipped(this)) {
        setScaleY(-getPlayerScale(this));
    } else {
        setScaleY(getPlayerScale(this));
    }
}

void (*TRAM_PlayerObject_toggleFlyMode)(PlayerObject* self, bool flyMode);
void PlayerObject_toggleFlyMode(PlayerObject* self, bool flyMode) {
    bool doAnything = (flyMode != getFlyMode(self));
    TRAM_PlayerObject_toggleFlyMode(self, flyMode);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::SHIP_GRAVITY_BUG_FIX) && doAnything) {
        // in 1.5, this is not run directly if the flyMode is disabled.
        // Instead, PlayerObject::resetPlayerIcon runs, which calls this function, among other miscellaneous stuff
        self->updatePlayerScale();
    }
}
void (*TRAM_PlayerObject_flipGravity)(PlayerObject* self, bool flip);
void PlayerObject_flipGravity(PlayerObject* self, bool flip) {
    bool doAnything = (flip != getGravityFlipped(self));
    TRAM_PlayerObject_flipGravity(self, flip);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::SHIP_GRAVITY_BUG_FIX) && doAnything) {
        self->updatePlayerScale();
    }
}
void (*TRAM_PlayerObject_togglePlayerScale)(PlayerObject* self, bool mini);
void PlayerObject_togglePlayerScale(PlayerObject* self, bool mini) {
    bool doAnything = false;
    if (mini && getPlayerScale(self) == 1) doAnything = true;
    if (!mini && getPlayerScale(self) != 1) doAnything = true;
    TRAM_PlayerObject_togglePlayerScale(self, mini);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::SHIP_GRAVITY_BUG_FIX) && doAnything) {
        self->stopActionByTag(5);

        auto scaleY = getPlayerScale(self);
        if (getFlyMode(self) && getGravityFlipped(self)) scaleY = -scaleY;

        auto action = CCEaseElasticOut::create(CCScaleTo::create(0.5, getPlayerScale(self), scaleY), 0.3);
        action->setTag(5);
        self->runAction(action);
    }
}
#endif

void PlayerObject_om() {
    Omni::hook("_ZN12PlayerObject14activateStreakEv",
        reinterpret_cast<void*>(PlayerObject_activateStreak),
        reinterpret_cast<void**>(&TRAM_PlayerObject_activateStreak));
    Omni::hook("_ZN12PlayerObject16deactivateStreakEv",
        reinterpret_cast<void*>(PlayerObject_deactivateStreak),
        reinterpret_cast<void**>(&TRAM_PlayerObject_deactivateStreak));
#if GAME_VERSION > GV_1_0
    Omni::hook("_ZN12PlayerObject18updateShipRotationEf",
        reinterpret_cast<void*>(PlayerObject_updateShipRotation),
        reinterpret_cast<void**>(&TRAM_PlayerObject_updateShipRotation));
#else
    Omni::hook("_ZN12PlayerObject18updateShipRotationEv",
        reinterpret_cast<void*>(PlayerObject_updateShipRotation),
        reinterpret_cast<void**>(&TRAM_PlayerObject_updateShipRotation));
#endif
    Omni::hook("_ZN12PlayerObject15runRotateActionEv",
        reinterpret_cast<void*>(PlayerObject_runRotateAction),
        reinterpret_cast<void**>(&TRAM_PlayerObject_runRotateAction));
#if GAME_VERSION > GV_1_1
    Omni::hook("_ZN12PlayerObject16runBallRotation2Ev",
        reinterpret_cast<void*>(PlayerObject_runBallRotation2),
        reinterpret_cast<void**>(&TRAM_PlayerObject_runBallRotation2));
#endif
    Omni::hook("_ZN12PlayerObject6updateEf",
        reinterpret_cast<void*>(PlayerObject_update),
        reinterpret_cast<void**>(&TRAM_PlayerObject_update));
    Omni::hook("_ZN12PlayerObject9hitGroundEb",
        reinterpret_cast<void*>(PlayerObject_hitGround),
        reinterpret_cast<void**>(&TRAM_PlayerObject_hitGround));
#if GAME_VERSION >= GV_1_5
    Omni::hook("_ZN12PlayerObject15playBurstEffectEv",
        reinterpret_cast<void*>(PlayerObject_playBurstEffect),
        reinterpret_cast<void**>(&TRAM_PlayerObject_playBurstEffect));
#endif
    Omni::hook("_ZN12PlayerObject15playerDestroyedEv",
        reinterpret_cast<void*>(PlayerObject_playerDestroyed),
        reinterpret_cast<void**>(&TRAM_PlayerObject_playerDestroyed));
    Omni::hook("_ZN12PlayerObject17levelFlipFinishedEv",
        reinterpret_cast<void*>(PlayerObject_levelFlipFinished),
        reinterpret_cast<void**>(&TRAM_PlayerObject_levelFlipFinished));
#if GAME_VERSION == GV_1_4
    Omni::hook("_ZN12PlayerObject13toggleFlyModeEb",
        reinterpret_cast<void*>(PlayerObject_toggleFlyMode),
        reinterpret_cast<void**>(&TRAM_PlayerObject_toggleFlyMode));
    Omni::hook("_ZN12PlayerObject17togglePlayerScaleEb",
        reinterpret_cast<void*>(PlayerObject_togglePlayerScale),
        reinterpret_cast<void**>(&TRAM_PlayerObject_togglePlayerScale));
    Omni::hook("_ZN12PlayerObject11flipGravityEb",
        reinterpret_cast<void*>(PlayerObject_flipGravity),
        reinterpret_cast<void**>(&TRAM_PlayerObject_flipGravity));
#endif
}