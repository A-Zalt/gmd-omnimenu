#include "hook.hpp"
#include "HaxButton.hpp"
#include <cocos2d.h>

void CCMenuItem_unregisterScriptTapHandler(CCMenuItem* self) {
    self->m_nScriptTapHandler = 0;
}

void CCMenuItem_om() {
    Omni::hook("_ZN7cocos2d10CCMenuItem26unregisterScriptTapHandlerEv",
        reinterpret_cast<void*>(CCMenuItem_unregisterScriptTapHandler),
        nullptr);
}