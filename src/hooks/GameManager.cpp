#include "hook.hpp"
#include "GameManager.hpp"

bool (*TRAM_GameManager_isColorUnlocked)(GameManager* self, int idx, bool secondary);
bool GameManager_isColorUnlocked(GameManager* self, int idx, bool secondary) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("icon_hack")) {
        return true;
    } else return TRAM_GameManager_isColorUnlocked(self, idx, secondary);
}
bool (*TRAM_GameManager_isIconUnlocked)(GameManager* self, int idx);
bool GameManager_isIconUnlocked(GameManager* self, int idx) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("icon_hack")) {
        return true;
    } else return TRAM_GameManager_isIconUnlocked(self, idx);
}

void GameManager_om() {
    Omni::hook("_ZN11GameManager15isColorUnlockedEib",
        reinterpret_cast<void*>(GameManager_isColorUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isColorUnlocked));
    Omni::hook("_ZN11GameManager14isIconUnlockedEi",
        reinterpret_cast<void*>(GameManager_isIconUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isIconUnlocked));
}