#include "hook.hpp"
#include "GJGameLevel.hpp"

void (*TRAM_PauseLayer_customSetup)(CCLayer* self);
void PauseLayer_customSetup(CCLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.levelEdit) {
        GJGameLevel* level = getPlayLayerLevel();
        GJLevelType type = getLevelType(level); // GJGameLevel::getLevelType
        setLevelType(level, GJLevelType::Editor);
        TRAM_PauseLayer_customSetup(self); 
        setLevelType(level, type);
    } else {
        TRAM_PauseLayer_customSetup(self); 
    }
}

void PauseLayer_om() {
    Omni::hook("_ZN10PauseLayer11customSetupEv",
        reinterpret_cast<void*>(PauseLayer_customSetup),
        reinterpret_cast<void**>(&TRAM_PauseLayer_customSetup));
}