#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook
#include "FLAlertLayer.hpp"
#include "../layers/HaxLayer.hpp"
#include "HaxManager.hpp"
#include "CCTextInputNode.hpp"
#include "SliderTouchLogic.hpp"
#include "PlayLayer.hpp"
#include "GJGameLevel.hpp"

void (*TRAM_PlayLayer_destroyPlayer)(void* self);
void PlayLayer_destroyPlayer(void* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.noClip || hax.instantComplete) return;
    TRAM_PlayLayer_destroyPlayer(self);
}
bool (*TRAM_GameManager_isColorUnlocked)(void* self, int idx, bool secondary);
bool GameManager_isColorUnlocked(void* self, int idx, bool secondary) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.iconHack) return true;
    else return TRAM_GameManager_isColorUnlocked(self, idx, secondary);
}
bool (*TRAM_GameManager_isIconUnlocked)(void* self, int idx);
bool GameManager_isIconUnlocked(void* self, int idx) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.iconHack) return true;
    else return TRAM_GameManager_isIconUnlocked(self, idx);
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
void (*TRAM_CCTextInputNode_setProfanityFilter)(CCTextInputNode* self, bool profanityFilter);
void CCTextInputNode_setProfanityFilter(CCTextInputNode* self, bool profanityFilter) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.swearBypass) return;
    TRAM_CCTextInputNode_setProfanityFilter(self, profanityFilter);
}
void (*TRAM_SliderTouchLogic_ccTouchMoved)(SliderTouchLogic* self, CCTouch* touch, CCEvent* event);
void SliderTouchLogic_ccTouchMoved(SliderTouchLogic* self, CCTouch* touch, CCEvent* event) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.sliderBypass) {
        auto touchPos = reinterpret_cast<CCNode*>(self)->convertTouchToNodeSpace(touch);
        auto touchOffset = self->m_position;
        auto position = ccp(touchPos.x - touchOffset.x, touchPos.y - touchOffset.y);

        auto thumb = reinterpret_cast<CCNode*>(self->getThumb());

        auto clamped = position.x;
        thumb->setPosition(ccp(clamped, 0.f));

        // if (self->getLiveDragging())
        //     thumb->activate();

        Slider* slider = self->getSliderDelegate();
        if (slider != nullptr) {
            slider->updateBar();
        }
        // if (Slider* slider = self->getSliderDelegate())
        //     slider->updateBar();
    } else TRAM_SliderTouchLogic_ccTouchMoved(self, touch, event);
}
void (*TRAM_PlayLayer_resetLevel)(PlayLayer* self);
void PlayLayer_resetLevel(PlayLayer* self) {
    TRAM_PlayLayer_resetLevel(self); 
    HaxManager& hax = HaxManager::sharedState();
    if (hax.instantComplete) {
        PlayerObject* player = *reinterpret_cast<PlayerObject**>(reinterpret_cast<uintptr_t>(self) + 0x22c);
        player->lockPlayer();
        self->levelComplete();
    }
}
bool (*TRAM_EditLevelLayer_init)(void* self, GJGameLevel* level);
bool EditLevelLayer_init(void* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.verifyHack) {
        *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(level) + 0x159) = true;
    }
    return TRAM_EditLevelLayer_init(self, level);
}
void (*TRAM_CCTransitionFade_create)(float duration, CCScene* scene, const ccColor3B& color);
void CCTransitionFade_create(float duration, CCScene* scene, const ccColor3B& color = ccBLACK) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.fastMenu) TRAM_CCTransitionFade_create(0.f, scene, color);
    else TRAM_CCTransitionFade_create(duration, scene, color);
}
void (*TRAM_PlayLayer_toggleFlipped)(void* self, bool p1, bool p2);
void PlayLayer_toggleFlipped(void* self, bool p1, bool p2) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.noMirror) return;
    TRAM_PlayLayer_toggleFlipped(self, p1, p2);
}

// void (*TRAM_PlayerObject_update)(PlayerObject* self, float dt);
// void PlayerObject_update(PlayerObject* self, float dt) {
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.instantComplete) return;
//     TRAM_PlayerObject_update(self, dt);
// }

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
    DobbyHook(
        dlsym(handle, "_ZN15CCTextInputNode18setProfanityFilterEb"),
        reinterpret_cast<void*>(CCTextInputNode_setProfanityFilter),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_setProfanityFilter)
    );
    DobbyHook(
        dlsym(handle, "_ZThn296_N16SliderTouchLogic12ccTouchMovedEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
        reinterpret_cast<void*>(SliderTouchLogic_ccTouchMoved),
        reinterpret_cast<void**>(&TRAM_SliderTouchLogic_ccTouchMoved)
    );
    DobbyHook(
        dlsym(handle, "_ZN9PlayLayer10resetLevelEv"),
        reinterpret_cast<void*>(PlayLayer_resetLevel),
        reinterpret_cast<void**>(&TRAM_PlayLayer_resetLevel)
    );
    DobbyHook(
        dlsym(handle, "_ZN14EditLevelLayer4initEP11GJGameLevel"),
        reinterpret_cast<void*>(EditLevelLayer_init),
        reinterpret_cast<void**>(&TRAM_EditLevelLayer_init)
    );
    DobbyHook(
        dlsym(handle, "_ZN9PlayLayer13toggleFlippedEbb"),
        reinterpret_cast<void*>(PlayLayer_toggleFlipped),
        reinterpret_cast<void**>(&TRAM_PlayLayer_toggleFlipped)
    );
    // DobbyHook(
    //     dlsym(handle, "_ZN12PlayerObject6updateEf"),
    //     reinterpret_cast<void*>(PlayerObject_update),
    //     reinterpret_cast<void**>(&TRAM_PlayerObject_update)
    // );
    DobbyHook(
        dlsym(handle, "_ZN7cocos2d16CCTransitionFade6createEfPNS_7CCSceneERKNS_10_ccColor3BE"),
        reinterpret_cast<void*>(CCTransitionFade_create),
        reinterpret_cast<void**>(&TRAM_CCTransitionFade_create)
    );
}
