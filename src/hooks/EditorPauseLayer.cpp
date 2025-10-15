#include "hook.hpp"
#include "LevelEditorLayer.hpp"

bool (*TRAM_EditorPauseLayer_init)(cocos2d::CCLayer* self, LevelEditorLayer* editLayer);
bool EditorPauseLayer_init(cocos2d::CCLayer* self, LevelEditorLayer* editLayer) {
    if (!TRAM_EditorPauseLayer_init(self, editLayer)) return false;
    HaxManager& hax = HaxManager::sharedState();
    if (hax.objectCounter) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        auto objectLimit = OBJECT_LIMIT + 1;
        if (hax.objectLimitHack) objectLimit = INCREASED_OBJECT_LIMIT;
        int objectCount = getObjectCount(editLayer);
        auto counterLabel = CCLabelBMFont::create(
            CCString::createWithFormat("%i/%i objects", objectCount, objectLimit)->getCString(), 
            "goldFont.fnt"
        );
        counterLabel->setScale(0.5f);
        counterLabel->setAnchorPoint({0.f, 0.5f});
        counterLabel->setPosition(ccp(10, winSize.height - 15));
        self->addChild(counterLabel, 1000);
    }
    return true;
}
void EditorPauseLayer_om() {
    Omni::hook("_ZN16EditorPauseLayer4initEP16LevelEditorLayer",
        reinterpret_cast<void*>(EditorPauseLayer_init),
        reinterpret_cast<void**>(&TRAM_EditorPauseLayer_init));
}