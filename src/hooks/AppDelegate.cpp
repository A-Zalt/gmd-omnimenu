#include "hook.hpp"

void (*TRAM_AppDelegate_applicationDidEnterBackground)(CCApplication* self);
void AppDelegate_applicationDidEnterBackground(CCApplication* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::AUTO_BACKUP)) {
        hax.createBackup();
    }
    TRAM_AppDelegate_applicationDidEnterBackground(self);
}

void AppDelegate_om() {
    Omni::hook("_ZN11AppDelegate29applicationDidEnterBackgroundEv",
        reinterpret_cast<void*>(AppDelegate_applicationDidEnterBackground),
        reinterpret_cast<void**>(&TRAM_AppDelegate_applicationDidEnterBackground));
}