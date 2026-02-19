#include "hook.hpp"
#include "GameManager.hpp"

#if GAME_VERSION < GV_1_4
bool (*TRAM_GameManager_isIconUnlocked)(GameManager* self, int idx);
bool GameManager_isIconUnlocked(GameManager* self, int idx) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::UNLOCK_ICONS)) {
        return true;
    } else return TRAM_GameManager_isIconUnlocked(self, idx);
}
bool (*TRAM_GameManager_isColorUnlocked)(GameManager* self, int idx, bool secondary);
bool GameManager_isColorUnlocked(GameManager* self, int idx, bool secondary) {
    HaxManager& hax = HaxManager::sharedState();
#if defined(EXTRA_COLORS)
    #if GAME_VERSION == GV_1_3
        if (idx == 13 || idx == 14) return true;
    #endif
#endif
    if (hax.getModuleEnabled(ModuleID::UNLOCK_ICONS)) {
        return true;
    } else return TRAM_GameManager_isColorUnlocked(self, idx, secondary);
}
#endif
// void (*TRAM_GameManager_reportPercentageForLevel)(GameManager* self, int level, int percentage, bool practice);
// void GameManager_reportPercentageForLevel(GameManager* self, int level, int percentage, bool practice) {
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.isSafeMode()) return;
//     cocos2d::CCLog("i have to put something here at least");
//     TRAM_GameManager_reportPercentageForLevel(self, level, percentage, practice);
// }

// Safe Mode: achievements (+ main level %)
// GameManager::reportPercentageForLevel is unhookable due to some bug in Dobby
#if GAME_VERSION < GV_1_3
void (*TRAM_GameManager_reportAchievementWithID)(void* self, const char* ach, int percent);
void GameManager_reportAchievementWithID(void* self, const char* ach, int percent) {
    HaxManager& hax = HaxManager::sharedState();
    std::string s = std::string(ach);
    if (hax.isSafeMode() && s.rfind("geometry.ach.level", 0) == 0) {
        return;
    }
    TRAM_GameManager_reportAchievementWithID(self, ach, percent);
}
#else
void (*TRAM_GameManager_reportAchievementWithID)(void* self, const char* ach, int percent, bool notify);
void GameManager_reportAchievementWithID(void* self, const char* ach, int percent, bool notify) {
    HaxManager& hax = HaxManager::sharedState();
    std::string s = std::string(ach);
    if (hax.isSafeMode() && (
        s.rfind("geometry.ach.level", 0) == 0 ||
        s.rfind("geometry.ach.demon", 0) == 0 ||
        s.rfind("geometry.ach.stars", 0) == 0 ||
        s.rfind("geometry.ach.coins", 0) == 0 ||
        s.rfind("geometry.ach.custom", 0) == 0 ||
        !strcmp(ach, "geometry.ach.special01")
    )) {
        return;
    }
    TRAM_GameManager_reportAchievementWithID(self, ach, percent, notify);
}
#endif

void (*TRAM_GameManager_toggleMusic)(GameManager* self);
void GameManager_toggleMusic(GameManager* self) {
    HaxManager& hax = HaxManager::sharedState();
    bool old = hax.areWeInPlayLayer;
    hax.areWeInPlayLayer = false;
    TRAM_GameManager_toggleMusic(self);
    hax.areWeInPlayLayer = old;
}

#if defined(EXTRA_COLORS)
ccColor3B (*TRAM_GameManager_colorForIdx)(GameManager* self, int idx);
ccColor3B GameManager_colorForIdx(GameManager* self, int idx) {
    switch (idx) {
        case 0:
            return ccc3(125, 255, 0);
        case 1:
            return ccc3(0, 255, 0);
        case 2:
            return ccc3(0, 255, 125);
        case 3:
            return ccc3(0, 255, 255);
        case 4:
            return ccc3(0, 125, 255);
        case 5:
            return ccc3(0, 0, 255);
        case 6:
            return ccc3(125, 0, 255);
        case 7:
            return ccc3(255, 0, 255);
        case 8:
            return ccc3(255, 0, 125);
        case 9:
            return ccc3(255, 0, 0);
        case 10:
            return ccc3(255, 125, 0);
        case 11:
            return ccc3(255, 255, 0);
#if GAME_VERSION == GV_1_3
        case 13:
            return ccc3(250, 127, 255);
        case 14:
            return ccc3(0, 0, 0);
#else
        case 13:
            return ccc3(255, 0xb9, 0);
        case 14:
            return ccc3(250, 127, 255);
        case 15:
            return ccc3(0, 0, 0);
#endif
        default:
            return ccc3(255, 255, 255);
    }
    // #if GAME_VERSION == GV_1_3
    // if (idx <= 12 || idx > 14) return TRAM_GameManager_colorForIdx(self, idx);
    // if (idx == 14) {
    //     return ccc3(250, 127, 255);
    // }
    // #else
    // if (idx <= 13 || idx > 15) return TRAM_GameManager_colorForIdx(self, idx);
    // if (idx == 15) {
    //     return ccc3(250, 127, 255);
    // }
    // #endif
    // return ccc3(0, 0, 0);
}
#endif

// void (*TRAM_GameManager_createAndAddParticle)(void* self, int a1, const char* file, int a2, tCCPositionType a3);
// void GameManager_createAndAddParticle(void* self, int a1, const char* file, int a2, tCCPositionType a3) { 
//     HaxManager& hax = HaxManager::sharedState();
//     if (!hax.getModuleEnabled("particle_end_wall") && !strcmp(file, "endEffectPortal.plist")) {
//         return;
//     }
//     TRAM_GameManager_createAndAddParticle(self, a1, file, a2, a3);
// }
void GameManager_om() {
#if GAME_VERSION < GV_1_4
    Omni::hook(
        "_ZN11GameManager14isIconUnlockedEi",
        reinterpret_cast<void*>(GameManager_isIconUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isIconUnlocked));
    Omni::hook("_ZN11GameManager15isColorUnlockedEib",
        reinterpret_cast<void*>(GameManager_isColorUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isColorUnlocked));
#endif
    Omni::hook(
#if GAME_VERSION < GV_1_3
        "_ZN11GameManager23reportAchievementWithIDEPKci",
#else
        "_ZN11GameManager23reportAchievementWithIDEPKcib",
#endif
        reinterpret_cast<void*>(GameManager_reportAchievementWithID),
        reinterpret_cast<void**>(&TRAM_GameManager_reportAchievementWithID));
    Omni::hook("_ZN11GameManager11toggleMusicEv",
        reinterpret_cast<void*>(GameManager_toggleMusic),
        reinterpret_cast<void**>(&TRAM_GameManager_toggleMusic));
#if defined(EXTRA_COLORS)
    Omni::hook("_ZN11GameManager11colorForIdxEi",
        reinterpret_cast<void*>(GameManager_colorForIdx),
        reinterpret_cast<void**>(&TRAM_GameManager_colorForIdx));
    #if GAME_VERSION == GV_1_3
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(iconkit_colors)),
            std::vector<uint8_t>({0x0f}).data(), 1
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(iconkit_colors2)),
            std::vector<uint8_t>({0x0f}).data(), 1
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(iconkit_colors4)),
            std::vector<uint8_t>({0x0f}).data(), 1
        );
    #elif GAME_VERSION == GV_1_4
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(iconkit_colors)),
            std::vector<uint8_t>({0x10}).data(), 1
        );
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(iconkit_colors2)),
            std::vector<uint8_t>({0x10}).data(), 1
        );
    #endif
        DobbyCodePatch(
            reinterpret_cast<void*>(get_address(iconkit_colors3)),
            std::vector<uint8_t>({0x0f}).data(), 1
        );
#endif
    // Omni::hook("_ZN10GameObject20createAndAddParticleEiPKciN7cocos2d15tCCPositionTypeE",
    //     reinterpret_cast<void*>(GameManager_createAndAddParticle),
    //     reinterpret_cast<void**>(&TRAM_GameManager_createAndAddParticle));
}