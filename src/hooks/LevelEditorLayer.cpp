#include "hook.hpp"
#include "LevelEditorLayer.hpp"
#include "GJGameLevel.hpp"

bool (*TRAM_LevelEditorLayer_init)(LevelEditorLayer* self, GJGameLevel* level);
bool LevelEditorLayer_init(LevelEditorLayer* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("object_hack"))
        setObjectLimit(INCREASED_OBJECT_LIMIT - 1);
    else
        setObjectLimit(OBJECT_LIMIT);
    setZoomBypass(hax.getModuleEnabled("zoom_bypass"));

    return TRAM_LevelEditorLayer_init(self, level);
}

void LevelEditorLayer_om() {
    Omni::hook("_ZN16LevelEditorLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(LevelEditorLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelEditorLayer_init));
}