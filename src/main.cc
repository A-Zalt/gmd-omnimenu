#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook
#include "FLAlertLayer.hpp"
#include "../layers/HaxLayer.hpp"

bool noclip = false;
bool iconHack = false;

void (*TRAM_PlayLayer_destroyPlayer)(void* self);
void PlayLayer_destroyPlayer(void* self) {
    if (noclip) return;
    TRAM_PlayLayer_destroyPlayer(self);
}
bool (*TRAM_GameManager_isColorUnlocked)(void* self, int idx, bool secondary);
bool GameManager_isColorUnlocked(void* self, int idx, bool secondary) {
    if (iconHack) return true;
    TRAM_GameManager_isColorUnlocked(self, idx, secondary);
}
bool (*TRAM_GameManager_isIconUnlocked)(void* self, int idx);
bool GameManager_isIconUnlocked(void* self, int idx) {
    if (iconHack) return true;
    TRAM_GameManager_isIconUnlocked(self, idx);
}
void MenuLayer_onMoreGames(void* self) {
    // auto layer = FLAlertLayer::create(
    //     nullptr,
    //     "Test",
    //     "Noclip toggled",
    //     "Ok",
    //     nullptr,
    //     300.f
    // );
    // layer->show();
    // noclip = !noclip;
    CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInT::create(0.5f, HaxLayer::scene(false)));
}
void MenuLayer_onRobTop(void* self) {
    auto layer = FLAlertLayer::create(
        nullptr,
        "Test",
        "Icon hack toggled",
        "Ok",
        nullptr,
        300.f
    );
    layer->show();
    iconHack = !iconHack;
}

[[gnu::constructor]]
int main() {
    void* handle = dlopen("libgame.so", RTLD_NOW);

    DobbyHook(
        dlsym(handle, "_ZN9PlayLayer13destroyPlayerEv"),
        reinterpret_cast<void*>(PlayLayer_destroyPlayer),
        reinterpret_cast<void**>(&TRAM_PlayLayer_destroyPlayer)
    );
    DobbyHook(
        dlsym(handle, "_ZN9MenuLayer11onMoreGamesEv"),
        (dobby_dummy_func_t) MenuLayer_onMoreGames,
        nullptr
    );
    DobbyHook(
        dlsym(handle, "_ZN9MenuLayer8onRobTopEv"),
        (dobby_dummy_func_t) MenuLayer_onRobTop,
        nullptr
    );
    DobbyHook(
        dlsym(handle, "_ZN12SupportLayer8onRobTopEv"),
        (dobby_dummy_func_t) MenuLayer_onRobTop,
        nullptr
    );
    DobbyHook(
        dlsym(handle, "_ZN11GameManager15isColorUnlockedEib"),
        reinterpret_cast<void*>(GameManager_isColorUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isColorUnlocked)
    );
    DobbyHook(
        dlsym(handle, "_ZN11GameManager14isIconUnlockedEi"),
        reinterpret_cast<void*>(GameManager_isIconUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isIconUnlocked)
    );
}
