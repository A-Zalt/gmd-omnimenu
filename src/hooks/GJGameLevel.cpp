#include "hook.hpp"
#include "GJGameLevel.hpp"

// Safe Mode: custom level %
void (*TRAM_GJGameLevel_savePercentage)(GJGameLevel* self, int percentage, bool practice);
void GJGameLevel_savePercentage(GJGameLevel* self, int percentage, bool practice) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.isSafeMode()) return;
#if GAME_VERSION < GV_1_5
    TRAM_GJGameLevel_savePercentage(self, percentage, practice);
#else
    int savePercent = percentage;
    if (percentage > 100) {
        savePercent = 100;
    }
    int comparePercent = self->m_nNormalPercent;
    if (practice) {
        comparePercent = self->m_nPracticePercent;
    }
    if (comparePercent < savePercent) {
        if (practice) {
            CCLog("New Practice Mode record! %i", savePercent);
            self->m_nPracticePercent = savePercent;
        } else {
            CCLog("New Normal Mode record! %i", savePercent);
            self->m_nNormalPercent = savePercent;
        }
    }
#endif
}
// Safe Mode: verification
#if GAME_VERSION < GV_1_5
void (*TRAM_GJGameLevel_setIsVerified)(GJGameLevel* self, bool isVerified);
void GJGameLevel_setIsVerified(GJGameLevel* self, bool isVerified) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.isSafeMode() && isVerified) return;
    TRAM_GJGameLevel_setIsVerified(self, isVerified);
}
#endif

void GJGameLevel_om() {
    Omni::hook("_ZN11GJGameLevel14savePercentageEib",
        reinterpret_cast<void*>(GJGameLevel_savePercentage),
        reinterpret_cast<void**>(&TRAM_GJGameLevel_savePercentage));
#if GAME_VERSION < GV_1_5
    Omni::hook("_ZN11GJGameLevel13setIsVerifiedEb",
        reinterpret_cast<void*>(GJGameLevel_setIsVerified),
        reinterpret_cast<void**>(&TRAM_GJGameLevel_setIsVerified));
#endif
}