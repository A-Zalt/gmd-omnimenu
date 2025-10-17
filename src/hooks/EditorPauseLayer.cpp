#include "hook.hpp"
#include "EditorPauseLayer.hpp"

void EditorPauseLayer::onOpenMenu() {
    auto haxOverlay = HaxOverlay::create(this);
    this->addChild(haxOverlay, 1000);
    this->setTouchEnabled(false);
}

bool (*TRAM_EditorPauseLayer_init)(cocos2d::CCLayer* self, LevelEditorLayer* editLayer);
bool EditorPauseLayer_init(cocos2d::CCLayer* self, LevelEditorLayer* editLayer) {
    if (!TRAM_EditorPauseLayer_init(self, editLayer)) return false;
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    if (hax.getModuleEnabled("object_counter")) {
        auto objectLimit = OBJECT_LIMIT + 1;
        if (hax.getModuleEnabled("object_hack")) objectLimit = INCREASED_OBJECT_LIMIT;
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
    CCMenu* btnMenu = CCMenu::create();
    self->addChild(btnMenu, 999);
    btnMenu->setPosition(ccp(0, winSize.height));

    CCSprite* menuSpr = CCSprite::create("OMNImenu_btn.png");
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(menuSpr, menuSpr, self, menu_selector(PauseLayer::onOpenMenu));
    btnMenu->addChild(menuBtn, 999);
    menuBtn->setPosition(ccp(winSize.width / 2.f - 50.f, -50.f));
    return true;
}
void EditorPauseLayer_om() {
    Omni::hook("_ZN16EditorPauseLayer4initEP16LevelEditorLayer",
        reinterpret_cast<void*>(EditorPauseLayer_init),
        reinterpret_cast<void**>(&TRAM_EditorPauseLayer_init));
}