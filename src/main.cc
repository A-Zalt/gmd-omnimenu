#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook
#include "FLAlertLayer.hpp"
#include "../layers/HaxLayer.hpp"
#include "HaxManager.hpp"
#include "CCTextInputNode.hpp"

void (*TRAM_PlayLayer_destroyPlayer)(void* self);
void PlayLayer_destroyPlayer(void* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.noClip) return;
    TRAM_PlayLayer_destroyPlayer(self);
}
bool (*TRAM_GameManager_isColorUnlocked)(void* self, int idx, bool secondary);
bool GameManager_isColorUnlocked(void* self, int idx, bool secondary) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.iconHack) return true;
    TRAM_GameManager_isColorUnlocked(self, idx, secondary);
}
bool (*TRAM_GameManager_isIconUnlocked)(void* self, int idx);
bool GameManager_isIconUnlocked(void* self, int idx) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.iconHack) return true;
    TRAM_GameManager_isIconUnlocked(self, idx);
}
void MenuLayer_onMoreGames(void* self) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, HaxLayer::scene(false)));
}
void (*TRAM_CCTextInputNode_setCharLimit)(void* self, int charLimit);
void CCTextInputNode_setCharLimit(void* self, int charLimit) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.textLengthBypass) return;
    TRAM_CCTextInputNode_setCharLimit(self, charLimit);
}
void (*TRAM_CCTextInputNode_updateLabel)(CCTextInputNode* self, char* text);
void CCTextInputNode_updateLabel(CCTextInputNode* self, char* text) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.charFilterBypass) {
        self->setAllowedChars(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
    }
    TRAM_CCTextInputNode_updateLabel(self, text);
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
        dlsym(handle, "_ZN11GameManager15isColorUnlockedEib"),
        reinterpret_cast<void*>(GameManager_isColorUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isColorUnlocked)
    );
    DobbyHook(
        dlsym(handle, "_ZN11GameManager14isIconUnlockedEi"),
        reinterpret_cast<void*>(GameManager_isIconUnlocked),
        reinterpret_cast<void**>(&TRAM_GameManager_isIconUnlocked)
    );
    DobbyHook(
        dlsym(handle, "_ZN15CCTextInputNode12setCharLimitEi"),
        reinterpret_cast<void*>(CCTextInputNode_setCharLimit),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_setCharLimit)
    );
    DobbyHook(
        dlsym(handle, "_ZN15CCTextInputNode11updateLabelEPKc"),
        reinterpret_cast<void*>(CCTextInputNode_updateLabel),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_updateLabel)
    );
}
