#include "hook.hpp"
#include "LevelEditorLayer.hpp"
#include "GJGameLevel.hpp"

void setObjectLimit(int limit) {
    if (limit > 0xFFFF) return;
    DobbyCodePatch(
        reinterpret_cast<void*>(get_address(object_limit)),
        toBytesLE(limit).data(), 2
    );
}

bool (*TRAM_LevelEditorLayer_init)(LevelEditorLayer* self, GJGameLevel* level);
bool LevelEditorLayer_init(LevelEditorLayer* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.objectLimitHack)
        setObjectLimit(INCREASED_OBJECT_LIMIT - 1);
    else
        setObjectLimit(OBJECT_LIMIT);

    return TRAM_LevelEditorLayer_init(self, level);
}

void LevelEditorLayer_om() {
    Omni::hook("_ZN16LevelEditorLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(LevelEditorLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelEditorLayer_init));
}