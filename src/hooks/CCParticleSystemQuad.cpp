#include "hook.hpp"

CCParticleSystemQuad* (*TRAM_CCParticleSystemQuad_create)(const char* file);
CCParticleSystemQuad* CCParticleSystemQuad_create(const char* file) {
    CCParticleSystemQuad* particle = TRAM_CCParticleSystemQuad_create(file);
    HaxManager& hax = HaxManager::sharedState();
    if ((!hax.getModuleEnabled(ModuleID::PARTICLE_DEATH_EFFECT) || hax.getModuleEnabled(ModuleID::NO_DEATH_EFFECT)) 
        && !strcmp(file, "explodeEffect.plist")) {
        particle->setVisible(false);
        return particle;
    }
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_FIREWORKS) && !strcmp(file, "firework.plist")) {
        particle->setVisible(false);
        return particle;
    }
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_LEVEL_COMPLETION) && !strcmp(file, "levelComplete01.plist")) {
        particle->setVisible(false);
        particle->stopSystem();
        return particle;
    }
    // if (!hax.getModuleEnabled(ModuleID::PARTICLE_END_WALL) && !strcmp(file, "endEffectPortal.plist")) {
    //     CCLog("hi");
    //     particle->setVisible(false);
    //     particle->stopSystem();
    //     return particle;
    // }
    return particle;
}

void (*TRAM_CCParticleSystem_resetSystem)(CCParticleSystem* self);  
void CCParticleSystem_resetSystem(CCParticleSystem* self) {
    HaxManager& hax = HaxManager::sharedState();
    TRAM_CCParticleSystem_resetSystem(self);
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_END_WALL) && self->m_sPlistFile == "endEffectPortal.plist") {
        self->setVisible(false);
        self->stopSystem();
        return;
    }
}

void CCParticleSystemQuad_om() {
    Omni::hook("_ZN7cocos2d20CCParticleSystemQuad6createEPKc",
        reinterpret_cast<void*>(CCParticleSystemQuad_create),
        reinterpret_cast<void**>(&TRAM_CCParticleSystemQuad_create));
    Omni::hook("_ZN7cocos2d16CCParticleSystem11resetSystemEv",
        reinterpret_cast<void*>(CCParticleSystem_resetSystem),
        reinterpret_cast<void**>(&TRAM_CCParticleSystem_resetSystem));
}