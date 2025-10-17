#include "hook.hpp"
#include "GJGameLevel.hpp"

bool (*TRAM_EditLevelLayer_init)(void* self, GJGameLevel* level);
bool EditLevelLayer_init(void* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("verify_bypass")) {
        setLevelVerified(level, true);
    }
    return TRAM_EditLevelLayer_init(self, level);
}
void EditLevelLayer_om() {
    Omni::hook("_ZN14EditLevelLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(EditLevelLayer_init),
        reinterpret_cast<void**>(&TRAM_EditLevelLayer_init));
}