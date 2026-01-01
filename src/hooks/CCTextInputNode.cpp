#include "hook.hpp"
#include "CCTextInputNode.hpp"

#if GAME_VERSION < GV_1_5
void (*TRAM_CCTextInputNode_setCharLimit)(void* self, int charLimit);
void CCTextInputNode_setCharLimit(void* self, int charLimit) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::TEXT_LENGTH_BYPASS)) return;
    TRAM_CCTextInputNode_setCharLimit(self, charLimit);
}
void (*TRAM_CCTextInputNode_setProfanityFilter)(CCTextInputNode* self, bool profanityFilter);
void CCTextInputNode_setProfanityFilter(CCTextInputNode* self, bool profanityFilter) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::SWEAR_FILTER_BYPASS)) return;
    TRAM_CCTextInputNode_setProfanityFilter(self, profanityFilter);
}
#endif
void (*TRAM_CCTextInputNode_updateLabel)(CCTextInputNode* self, char* text);
void CCTextInputNode_updateLabel(CCTextInputNode* self, char* text) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::CHARACTER_FILTER_BYPASS)) {
        self->setAllowedChars(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
    }
    TRAM_CCTextInputNode_updateLabel(self, text);
}

#if GAME_VERSION < GV_1_2
void append_char(char*& a, size_t& length, size_t& capacity, char c) {
    if (length + 1 >= capacity) {
        capacity *= 2;
        a = (char*)realloc(a, capacity);
    }
    a[length++] = c;
    a[length] = '\0';
}

// https://forum.cocosengine.org/t/keyboard-input-not-working/8949
bool (*TRAM_CCTextInputNode_onTextFieldInsertText)(CCTextInputNode* self, CCTextFieldTTF* pSender, const char* text, int nLen);
bool CCTextInputNode_onTextFieldInsertText(CCTextInputNode* self, CCTextFieldTTF* pSender, const char* text, int nLen) {
    HaxManager& hax = HaxManager::sharedState();
#if GAME_VERSION < GV_1_2
    if (!hax.getModuleEnabled(ModuleID::INPUT_BUG_FIX)) return TRAM_CCTextInputNode_onTextFieldInsertText(self, pSender, text, nLen);

    if (text && strlen(text) > 1) {
        size_t capacity = 32;
        size_t length = 0;

        char* newText = (char*)malloc(capacity);
        newText[0] = '\0';

        std::string allowed = getAllowedChars(self);

        int limiter = strlen(text);
        if (!hax.getModuleEnabled(ModuleID::TEXT_LENGTH_BYPASS) && getCharLimit(self) < limiter) limiter = getCharLimit(self);

        int newLen = 0;

        for (int i = 0; i < strlen(text); i++) {
            if (allowed.find(text[i]) != std::string::npos) {
                append_char(newText, length, capacity, text[i]);
                newLen++;
            }
            if (newLen >= limiter) break;
        }

        self->setString(newText);

        return true;
    }
    // if insert enter, treat as default to detach with ime
    if ('\n' == *text) {
        return false;
    }

    std::string::size_type find = getAllowedChars(self).find(text);
    if (find == std::string::npos) return true;

    // if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
    if (pSender->getCharCount() >= getCharLimit(self) && getCharLimit(self) > 0) return true;

    return false;
#else
    // if insert enter, treat as default to detach with ime
    if ('\n' == *text) {
        return false;
    }
#endif
}
#endif

void (*TRAM_CCTextFieldTTF_detachWithIME)(CCTextFieldTTF* self);
void CCTextFieldTTF_detachWithIME(CCTextFieldTTF* self) {
    CCLog("[OMNI] CCTFTTF detachWithIME");
    TRAM_CCTextFieldTTF_detachWithIME(self);
}

void CCTextInputNode_om() {
#if GAME_VERSION < GV_1_5
    Omni::hook("_ZN15CCTextInputNode12setCharLimitEi",
        reinterpret_cast<void*>(CCTextInputNode_setCharLimit),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_setCharLimit));
    Omni::hook("_ZN15CCTextInputNode18setProfanityFilterEb",
        reinterpret_cast<void*>(CCTextInputNode_setProfanityFilter),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_setProfanityFilter));
#endif
    Omni::hook("_ZN15CCTextInputNode11updateLabelEPKc",
        reinterpret_cast<void*>(CCTextInputNode_updateLabel),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_updateLabel));
#if GAME_VERSION < GV_1_2
    Omni::hook("_ZN15CCTextInputNode21onTextFieldInsertTextEPN7cocos2d14CCTextFieldTTFEPKci",
        reinterpret_cast<void*>(CCTextInputNode_onTextFieldInsertText),
        reinterpret_cast<void**>(&TRAM_CCTextInputNode_onTextFieldInsertText));
#endif
    Omni::hook("_ZN7cocos2d14CCTextFieldTTF13detachWithIMEEv",
        reinterpret_cast<void*>(CCTextFieldTTF_detachWithIME),
        reinterpret_cast<void**>(&TRAM_CCTextFieldTTF_detachWithIME));
}