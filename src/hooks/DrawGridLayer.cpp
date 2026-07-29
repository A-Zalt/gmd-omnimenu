#include "hook.hpp"
#include "DrawGridLayer.hpp"

void (*TRAM_DrawGridLayer_draw)(DrawGridLayer* self);
void DrawGridLayer_draw(DrawGridLayer* self) {
    TRAM_DrawGridLayer_draw(self);
    if (g_playback) {
        float dt = cocos2d::CCDirector::sharedDirector()->getScheduler()->getTimeScale() * cocos2d::CCDirector::sharedDirector()->getAnimationInterval();
        ccDrawColor4F(0.0f, 1.f, 0.0f, 1.0f);
        ccDrawLine(ccp(g_lineX, 0), ccp(g_lineX, 100000));
    }
}

void DrawGridLayer_om() {
    Omni::hook("_ZN13DrawGridLayer4drawEv",
        reinterpret_cast<void*>(DrawGridLayer_draw),
        reinterpret_cast<void**>(&TRAM_DrawGridLayer_draw));
}