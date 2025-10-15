#include "hook.hpp"
#include "CCTextInputNode.hpp"

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

void CCTextInputNode_om() {
    Omni::hook("_ZN15CCTextInputNode12setCharLimitEi",
        reinterpret_cast<void*>(CCTextInputNode_setCharLimit),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_setCharLimit));
    Omni::hook("_ZN15CCTextInputNode11updateLabelEPKc",
        reinterpret_cast<void*>(CCTextInputNode_updateLabel),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_updateLabel));
    Omni::hook("_ZN15CCTextInputNode18setProfanityFilterEb",
        reinterpret_cast<void*>(CCTextInputNode_setProfanityFilter),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_setProfanityFilter));
}