#include "hook.hpp"
#include "GameObject.hpp"

void (*TRAM_GameObject_addGlow)(void* self);
void GameObject_addGlow(void* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_GLOW)) return;
    TRAM_GameObject_addGlow(self);
}
void (*TRAM_GameObject_setOpacity)(void* self, GLubyte opacity);
void GameObject_setOpacity(void* self, GLubyte opacity) {
    TRAM_GameObject_setOpacity(self, opacity);
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_OBJECTS)) {
        auto p = getObjectParticles(self);
        if (p && p != nullptr) p->stopSystem();
    }
    if (hax.getModuleEnabled(ModuleID::NO_PULSE)) {
        setObjectUseAudioScale(self, false);
    }
}
// CCParticleSystemQuad* (*TRAM_GameObject_createAndAddParticle)(GameObject* obj, int a2, const char* name, int a4, tCCPositionType posType);
// CCParticleSystemQuad* GameObject_createAndAddParticle(GameObject* obj, int a2, const char* name, int a4, tCCPositionType posType) {
//     HaxManager& hax = HaxManager::sharedState();
//     auto quad = TRAM_GameObject_createAndAddParticle(obj, a2, name, a4, posType);
//     if (!hax.getModuleEnabled(ModuleID::PARTICLE_END_WALL) && !strcmp(name, "endEffectPortal.plist")) {
        
//         quad->setVisible(false);
//         quad->stopSystem();
//     }
//     return quad;
// }
// Module by Akqanile/adelfa
CCRect (*TRAM_GameObject_getObjectRect)(GameObject* self, float scaleX, float scaleY);
CCRect GameObject_getObjectRect(GameObject* self, float scaleX, float scaleY) {
    CCRect objectRect = TRAM_GameObject_getObjectRect(self, scaleX, scaleY);
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::ROTATION_BUG_FIX)) return objectRect;
    
    float rotation = std::fabs(self->getRotation());

    objectRect.origin.x -= objectRect.size.width * -0.5;
    objectRect.origin.y -= objectRect.size.height * -0.5;
    
    if (rotation == 90.0f || rotation == 270.0f) {
        std::swap(objectRect.size.width, objectRect.size.height);
    }

    objectRect.origin.x += objectRect.size.width * -0.5;
    objectRect.origin.y += objectRect.size.height * -0.5;
    
    return objectRect;
}

void GameObject_om() {
    Omni::hook("_ZN10GameObject7addGlowEv",
        reinterpret_cast<void*>(GameObject_addGlow),
        reinterpret_cast<void**>(&TRAM_GameObject_addGlow));
    Omni::hook("_ZN10GameObject10setOpacityEh",
        reinterpret_cast<void*>(GameObject_setOpacity),
        reinterpret_cast<void**>(&TRAM_GameObject_setOpacity));
#if GAME_VERSION < GV_1_2
    Omni::hook("_ZN10GameObject13getObjectRectEff",
        reinterpret_cast<void*>(GameObject_getObjectRect),
        reinterpret_cast<void**>(&TRAM_GameObject_getObjectRect));
#endif
    // Omni::hook("_ZN10GameObject20createAndAddParticleEiPKciN7cocos2d15tCCPositionTypeE",
    //     reinterpret_cast<void*>(GameObject_createAndAddParticle),
    //     reinterpret_cast<void**>(&TRAM_GameObject_createAndAddParticle));
}