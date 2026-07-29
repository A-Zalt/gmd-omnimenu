#include "hook.hpp"
#include "HaxButton.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include <cocos2d.h>

void (*TRAM_CCMenuItemSpriteExtra_selected)(CCMenuItemSpriteExtra* self);
void CCMenuItemSpriteExtra_selected(CCMenuItemSpriteExtra* self) {
    TRAM_CCMenuItemSpriteExtra_selected(self);
    if (self->getTag() == HAX_BUTTON_TAG) {
        auto menu = hax.omniMenu->m_sMenu;
        self->stopAllActions();
        self->runAction(CCFadeTo::create(0.1f, 255));
    }
}

void (*TRAM_CCMenuItemSpriteExtra_unselected)(CCMenuItemSpriteExtra* self);
void CCMenuItemSpriteExtra_unselected(CCMenuItemSpriteExtra* self) {
    TRAM_CCMenuItemSpriteExtra_unselected(self);
    if (self->getTag() == HAX_BUTTON_TAG) {
        self->stopAllActions();
        self->runAction(CCFadeTo::create(0.1f, 128));
    }
}

void CCMenuItemSpriteExtra_om() {
    Omni::hook("_ZN21CCMenuItemSpriteExtra8selectedEv",
        reinterpret_cast<void*>(CCMenuItemSpriteExtra_selected),
        reinterpret_cast<void**>(&TRAM_CCMenuItemSpriteExtra_selected));
    
    Omni::hook("_ZN21CCMenuItemSpriteExtra10unselectedEv",
        reinterpret_cast<void*>(CCMenuItemSpriteExtra_unselected),
        reinterpret_cast<void**>(&TRAM_CCMenuItemSpriteExtra_unselected));
}