#include "hook.hpp"
#include "CCTableCellLabel.hpp"

void (*TRAM_CCNode_setVisible)(CCNode* self, bool toggle);
void CCNode_setVisible(CCNode* self, bool toggle) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("force_visibility") || dynamic_cast<CCTableCellLabel*>(self) != nullptr) {
        TRAM_CCNode_setVisible(self, true);
    } else TRAM_CCNode_setVisible(self, toggle);
}

void CCNode_om() {
    Omni::hook("_ZN7cocos2d6CCNode10setVisibleEb",
        reinterpret_cast<void*>(CCNode_setVisible),
        reinterpret_cast<void**>(&TRAM_CCNode_setVisible));
}