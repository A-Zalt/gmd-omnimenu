#include "hook.hpp"

CCTransitionFade* (*TRAM_CCTransitionFade_create)(float duration, CCScene* scene, const ccColor3B& color);
CCTransitionFade* CCTransitionFade_create(float duration, CCScene* scene, const ccColor3B& color = ccBLACK) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::FAST_MENU)) return TRAM_CCTransitionFade_create(0.f, scene, color);
    return TRAM_CCTransitionFade_create(duration, scene, color);
}

void CCTransitionFade_om() {
    Omni::hook("_ZN7cocos2d16CCTransitionFade6createEfPNS_7CCSceneERKNS_10_ccColor3BE",
        reinterpret_cast<void*>(CCTransitionFade_create),
        reinterpret_cast<void**>(&TRAM_CCTransitionFade_create));
}