#include "hook.hpp"
#include "EditorUI.hpp"
#include "FLAlertLayer.hpp"

void (*TRAM_EditorUI_showMaxError)(void* self);
void EditorUI_showMaxError(void* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("object_hack")) {
        FLAlertLayer::create(
            nullptr,
            "Max Objects",
            CCString::createWithFormat("You cannot create more than <cy>%i</c> <cg>objects</c> in a single level.", INCREASED_OBJECT_LIMIT)->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
        return;
    }
    TRAM_EditorUI_showMaxError(self);
}
void (*TRAM_EditorUI_constrainGameLayerPosition)(void* self);
void EditorUI_constrainGameLayerPosition(void* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("free_scroll")) return;
    TRAM_EditorUI_constrainGameLayerPosition(self);
}

void EditorUI_om() {
    Omni::hook("_ZN8EditorUI12showMaxErrorEv",
        reinterpret_cast<void*>(EditorUI_showMaxError),
        reinterpret_cast<void**>(&TRAM_EditorUI_showMaxError));
    Omni::hook("_ZN8EditorUI26constrainGameLayerPositionEv",
        reinterpret_cast<void*>(EditorUI_constrainGameLayerPosition),
        reinterpret_cast<void**>(&TRAM_EditorUI_constrainGameLayerPosition));
}