#include "hook.hpp"
#include "EditorUI.hpp"
#include "FLAlertLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "GameObject.hpp"
#include "DrawGridLayer.hpp"
#include "UndoObject.hpp"
#include "ButtonSprite.hpp"
#include "EditButtonBar.hpp"

void updateObjectInfoLabel(EditorUI* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::SHOW_OBJECT_INFO)) return;
    if (!hax.editorObjectInfo) {
        CCLog("no EOI");
        return;
    }
    CCLog("update object info label");
    if (getSelectedObjects(self) && getSelectedObjects(self)->count() > 0) {
        hax.editorObjectInfo->setString(CCString::createWithFormat(
            "Objects: %i", getSelectedObjects(self)->count()
        )->getCString());
    } else if (getSelectedObject(self)) {
        auto obj = getSelectedObject(self);
        hax.editorObjectInfo->setString(CCString::createWithFormat(
            "Position: (%i, %i)\nRotation: %i\nID: %i\nSection: %i\nType: %i",
            (int)obj->getPositionX(), (int)obj->getPositionY(), (int)obj->getRotation(),
            getObjectKey(obj), getSectionIdx(obj), getObjectType(obj)
        )->getCString());
    } else {
        hax.editorObjectInfo->setString("");
    }
}

void (*TRAM_EditorUI_showMaxError)(void* self);
void EditorUI_showMaxError(void* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::OBJECT_LIMIT_BYPASS)) {
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
    if (hax.getModuleEnabled(ModuleID::FREE_SCROLL)) return;
    TRAM_EditorUI_constrainGameLayerPosition(self);
}

#if GAME_VERSION < GV_1_5
void (*TRAM_EditorUI_zoomOut)(EditorUI* self);
void EditorUI_zoomOut(EditorUI* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::ZOOM_BYPASS)) {
        cocos2d::CCLayer* gameLayer = getEditorGameLayer(getUIEditorLayer(self));
        CCLog("%f", gameLayer->getScale());
        if (gameLayer->getScale() > 0.11f) TRAM_EditorUI_zoomOut(self);
    } else {
        TRAM_EditorUI_zoomOut(self);
    }
}
#elif GAME_VERSION < GV_1_7
void EditorUI::zoomOutExtra() {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::ZOOM_BYPASS)) {
        cocos2d::CCLayer* gameLayer = getEditorGameLayer(getUIEditorLayer(this));
        if (gameLayer->getScale() > 0.11f) this->zoomOut(); // value to check against has to be bigger than 0.1 because otherwise it still lets you zoom to 0 anyways
    } else {
        this->zoomOut();
    }
}
#else
void EditorUI::zoomOutExtra(CCObject* sender) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::ZOOM_BYPASS)) {
        cocos2d::CCLayer* gameLayer = getEditorGameLayer(getUIEditorLayer(this));
        if (gameLayer->getScale() > 0.11f) this->zoomOut(sender); // value to check against has to be bigger than 0.1 because otherwise it still lets you zoom to 0 anyways
    } else {
        this->zoomOut(sender);
    }
}
#endif

#if GAME_VERSION < GV_1_5
const auto cyan = ccc3(0, 255, 255);
GameObject* EditorUI::duplicateObject(GameObject* obj, void* getSaveString) {
    LevelEditorLayer* editLayer = getUIEditorLayer(this);
    DrawGridLayer* gridLayer = getGridLayer(editLayer);
    CCSpriteBatchNode* batchNode = getEditorBatchNode(editLayer);

    std::string saveString = ((std::string(*)(GameObject*))getSaveString)(obj);
    GameObject* newObj = GameObject::objectFromString(saveString);

    obj->setColor(ccWHITE);
    editLayer->addToSection(newObj);
    if (getObjectType(newObj) == 7 && getShouldSpawn(newObj)) { // trigger effect line
        gridLayer->addToEffects(newObj);
    }
    newObj->setColor(cyan);

    batchNode->addChild(newObj);
    UndoObject* undo = UndoObject::create(newObj, UndoCommand::Placement);
    editLayer->addToUndoList(undo);

    return newObj;
}
void EditorUI::onDuplicate() {
    GameObject* selObj = getSelectedObject(this);
    CCArray* selectedObjects = getSelectedObjects(this);
        
    if (selObj == nullptr && selectedObjects->count() < 0) return;

    void* getSaveString = DobbySymbolResolver(MAIN_LIBRARY, "_ZN10GameObject13getSaveStringEv");
    LevelEditorLayer* editLayer = getUIEditorLayer(this);
    getRedoArray(editLayer)->removeAllObjects();

    if (selObj != nullptr) {
        auto newObj = duplicateObject(selObj, getSaveString);
        this->selectObject(newObj);
        newObj->setColor(cyan);
    } else {
        for (int i = 0; i < selectedObjects->count(); i++) {
            auto currObj = static_cast<GameObject*>(selectedObjects->objectAtIndex(i));
            auto newObj = duplicateObject(currObj, getSaveString);
            selectedObjects->replaceObjectAtIndex(i, newObj, false);
        }
    }
}

void EditorUI::onDeleteSelected() {
    GameObject* selObj = getSelectedObject(this);
    CCArray* selectedObjects = getSelectedObjects(this);
    LevelEditorLayer* editLayer = getUIEditorLayer(this);
    if (selObj == nullptr && selectedObjects->count() < 0) return;

    if (selObj != nullptr) {
        this->deselectObject();
        editLayer->removeObject(selObj);
    } else {
        for (int i = 0; i < selectedObjects->count(); i++) {
            auto currObj = static_cast<GameObject*>(selectedObjects->objectAtIndex(i));
            editLayer->removeObject(currObj);
        }
        this->deselectAll();
    }
    updateObjectInfoLabel(this);
}
#endif // GAME_VERSION < GV_1_5

// #if GAME_VERSION < GV_1_6
// void EditorUI_keyBackClicked(EditorUI* self) {
//     CCLog("HELLO WE COME IN PEA");
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.getModuleEnabled("back_button_pause")) {
//         self->onPause();
//     }
// }
// #endif

bool (*TRAM_EditorUI_init)(EditorUI* self, LevelEditorLayer* lel);
bool EditorUI_init(EditorUI* self, LevelEditorLayer* lel) {
    if (!TRAM_EditorUI_init(self, lel)) return false;
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
#if GAME_VERSION >= GV_1_5
    getZoomOutButton(self)->setTarget(self, menu_selector(EditorUI::zoomOutExtra));
#endif
    if (hax.getModuleEnabled(ModuleID::DELETE_SELECTED)) {
        CCMenu* delMenu = CCMenu::create();
        self->addChild(delMenu);

        delMenu->setPosition(ccp(125, winSize.height - 20));
        CCSprite* delSelSpr = CCSprite::create("GJ_trashBtn.png");
        CCMenuItemSpriteExtra* delSelBtn = CCMenuItemSpriteExtra::create(delSelSpr, delSelSpr, self, menu_selector(EditorUI::onDeleteSelected));

        delMenu->addChild(delSelBtn);
    }
#if GAME_VERSION < GV_1_5
    if (hax.getModuleEnabled(ModuleID::COPY_PASTE)) {

        CCMenu* btnMenu = CCMenu::create();
        self->addChild(btnMenu);

#if GAME_VERSION == GV_1_4
        auto y = winSize.height - 120;
#else
        auto y = winSize.height - 70;
#endif
        btnMenu->setPosition(ccp(winSize.width - 27, y));
        CCSprite* copySpr = CCSprite::create("GJ_copyBtn.png");
        CCMenuItemSpriteExtra* copyBtn = CCMenuItemSpriteExtra::create(copySpr, copySpr, self, menu_selector(EditorUI::onDuplicate));

        btnMenu->addChild(copyBtn);
    }
#endif

    if (hax.getModuleEnabled(ModuleID::SHOW_OBJECT_INFO)) {
        CCLabelBMFont* objInfo = CCLabelBMFont::create("", "chatFont.fnt");
        objInfo->setAnchorPoint({0, 1});
        objInfo->setPosition(ccp(15, winSize.height - 45));
        objInfo->setScale(0.6);
        self->addChild(objInfo);
        hax.editorObjectInfo = objInfo;
    }

// #if GAME_VERSION < GV_1_6
//     self->setKeypadEnabled(true);
//     void** vtable = *(void***)self;
//     void (EditorUI::* ptrBack)(void) = &EditorUI::keyBackClicked;
//     void* offsetBack = *(void**)&ptrBack;
//     vtable[((uintptr_t)offsetBack)/sizeof(void*)] = (void*)&EditorUI_keyBackClicked;
// #endif

    return true;
}

#if GAME_VERSION < GV_1_6
void (*TRAM_EditorUI_setupDeleteMenu)(EditorUI* self);
void EditorUI_setupDeleteMenu(EditorUI* self) {
    TRAM_EditorUI_setupDeleteMenu(self);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::DELETE_START_POS)) {
        CCMenu* menu = getEditorUIButtonMenu(self);

        if (!menu || menu == nullptr) { // useless failsafe but i don't like removing those
            TRAM_EditorUI_setupDeleteMenu(self);
            menu = getEditorUIButtonMenu(self);
        }

        auto delSPSpr = ButtonSprite::create("Delete Start Pos", 85, 0, 0.6, false, "bigFont.fnt", "GJ_button_04.png");
        // delSelSpr->setScale(0.8f);

        auto delSPBtn = CCMenuItemSpriteExtra::create(delSPSpr, delSPSpr, self, menu_selector(EditorUI::onDeleteStartPos));
        menu->addChild(delSPBtn);
#if GAME_VERSION == GV_1_5
        menu->alignItemsHorizontallyWithPadding(10);
#endif 
    }
}
void EditorUI::onDeleteStartPos() {
    LevelEditorLayer* editLayer = getUIEditorLayer(this);
    CCArray* sections = getEditorSections(editLayer);
    for (int i = 0; i < sections->count(); i++) {
        auto section = static_cast<CCArray*>(sections->objectAtIndex(i));
        for (int j = 0; j < section->count(); j++) {
            auto currObj = static_cast<GameObject*>(section->objectAtIndex(j));
            // CCLog("sidx: %i, idx: %i, id: %i", i, j, getObjectKey(currObj));
            if (getObjectKey(currObj) == 31) {// start position object ID
                editLayer->removeObject(currObj);
                j--;
            }
        }
    }

    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::SHOW_OBJECT_INFO)) return;

    auto sel = getSelectedObjects(this);
    auto selObj = getSelectedObject(this);
    if (selObj && getObjectKey(selObj) == 31) {
        this->deselectObject();
    }
    if (sel && sel->count() > 0) {
        for (int i = 0; i < sel->count(); i++) {
#if GAME_vERSION >= GV_1_5
            auto currObjUncast = sel->objectAtIndex(i);
            if (!currObjUncast) {
                i--;
                continue;
            }
            auto currObj = dynamic_cast<GameObject*>(currObjUncast);
#else
            auto currObj = static_cast<GameObject*>(sel->objectAtIndex(i));
#endif
            if (currObj && getObjectKey(currObj) == 31) {
                sel->removeObject(currObj);
                currObj->setColor(ccWHITE);
                i--;
            }
        }
    }

    updateObjectInfoLabel(this);
}
#endif

void (*TRAM_EditorUI_setupCreateMenu)(EditorUI* self);
void EditorUI_setupCreateMenu(EditorUI* self) {
    HaxManager& hax = HaxManager::sharedState();
    TRAM_EditorUI_setupCreateMenu(self);
    if (hax.getModuleEnabled(ModuleID::UNLISTED_OBJECTS)) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
#if GAME_VERSION < GV_1_7

        CCArray* createBtns = getCreateButtons(self);
        CCArray* fuckingArray = CCArray::create();

        for (int i = 0; i < createBtns->count(); i++) {
            CCObject* btn = createBtns->objectAtIndex(i);
            static_cast<CCNode*>(btn)->removeFromParentAndCleanup(false);
            fuckingArray->addObject(btn);
        }
        CCNode* separator = CCNode::create();
        separator->setTag(0);

        EditButtonBar* bar = getCreateButtonBar(self);
        bar->removeFromParentAndCleanup(false);
        bar->release();

        CCNode* sep1 = CCNode::create();
        sep1->setTag(0);
#if GAME_VERSION > GV_1_2
        CCNode* sep2 = CCNode::create();
        sep2->setTag(0);
#endif
#if GAME_VERSION == GV_1_0
        fuckingArray->insertObject(sep1, 25);
#elif GAME_VERSION == GV_1_1
        fuckingArray->insertObject(sep1, 27);
#elif GAME_VERSION == GV_1_2
        fuckingArray->insertObject(sep1, 28);
#elif GAME_VERSION == GV_1_3
        fuckingArray->insertObject(sep1, 51);
        fuckingArray->insertObject(sep2, 63);
#elif GAME_VERSION == GV_1_4
        fuckingArray->insertObject(sep1, 74);
        fuckingArray->insertObject(sep2, 86);
#elif GAME_VERSION == GV_1_5
        fuckingArray->insertObject(sep1, 105);
        fuckingArray->insertObject(sep2, 117);
#elif GAME_VERSION == GV_1_6
        fuckingArray->insertObject(sep1, 163);
        fuckingArray->insertObject(sep2, 175);
#endif

        CCNode* unlistedSeparator = CCNode::create();
        unlistedSeparator->setTag(0);
        fuckingArray->addObject(unlistedSeparator);
#if GAME_VERSION < GV_1_3
        fuckingArray->addObject(self->getCreateBtn("edit_eeSDBtn_001.png", 4)); // a transition trigger that only got listed in 1.3
#endif
        fuckingArray->addObject(self->getCreateBtn("edit_eLevelEndBtn_001.png", 4)); // level end
        fuckingArray->addObject(self->getCreateBtn("edit_eBGEOn_001.png", 4)); // bg effect on
        fuckingArray->addObject(self->getCreateBtn("edit_eBGEOff_001.png", 4)); // bg effect off
#if GAME_VERSION == GV_1_0 // 1.02 ONLY!
        fuckingArray->addObject(self->getCreateBtn("portal_05_front_001.png", 4)); // orange mirror portal
        fuckingArray->addObject(self->getCreateBtn("portal_06_front_001.png", 4)); // blue mirror portal
#elif GAME_VERSION == GV_1_1 // 1.11 ONLY!
        fuckingArray->addObject(self->getCreateBtn("portal_07_front_001.png", 4)); // ball portal
#elif GAME_VERSION >= GV_1_3
        fuckingArray->addObject(self->getCreateBtn("edit_eeFABtn_001.png", 4)); // scatter transition trigger
    #if GAME_VERSION < GV_1_6 // they are removed in 1.6 and reintroduced in 1.8
        fuckingArray->addObject(self->getCreateBtn("square_b_02_001.png", 4)); // wavy slab outer corner thing
        fuckingArray->addObject(self->getCreateBtn("square_b_03_001.png", 4)); // wavy slab inner corner thing
    #endif
    #if GAME_VERSION == GV_1_3
        fuckingArray->addObject(self->getCreateBtn("square_d_01_001.png", 4)); // x blocks
        fuckingArray->addObject(self->getCreateBtn("square_d_02_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("square_d_03_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("square_d_04_001.png", 4)); // this one will crash on 1.4+
        fuckingArray->addObject(self->getCreateBtn("square_d_05_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("square_d_06_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("square_d_07_001.png", 4));
    #elif GAME_VERSION == GV_1_4
        fuckingArray->addObject(self->getCreateBtn("edit_eTintObjBtn_001.png", 4)); // obj trigger
    #endif
    #if GAME_VERSION >= GV_1_5
        fuckingArray->addObject(self->getCreateBtn("portal_10_back_001.png", 4)); // weird ufo object thing removed in 2.0
    #endif
    #if GAME_VERSION == GV_1_5
        fuckingArray->addObject(self->getCreateBtn("d_cloud_05_001.png", 4)); // small cloud
        fuckingArray->addObject(self->getCreateBtn("d_art_01_001.png", 4)); // small diamond rod
    #endif
#ifndef FORCE_AUTO_SAFE_MODE
    #if GAME_VERSION >= GV_1_6
        fuckingArray->addObject(self->getCreateBtn("secretCoin_01_001.png", 4)); // secret coin
    #endif
#endif
#endif // GAME_VERSION >= GV_1_3

#if GAME_VERSION < GV_1_6
        EditButtonBar* newBar = EditButtonBar::create(fuckingArray, ccp(winSize.width * 0.5 - 5, getScreenBottom() + getUnkFloat(self) - 6.f));
#else
        EditButtonBar* newBar = EditButtonBar::create(fuckingArray, ccp(winSize.width * 0.5 - 5, getScreenBottom() + getUnkFloat(self) - 6.f), true);
#endif
        setCreateButtonBar(self, newBar);

        self->addChild(newBar, 11);
#if GAME_VERSION < GV_1_5
        self->updateCreateMenu();
#else
        self->updateCreateMenu(false);
#endif

#else

        auto bars = getCreateButtonBars(self);
        auto fuckingArray = CCArray::create();
        auto triggerPages = getBarPages(static_cast<EditButtonBar*>(bars->lastObject()));
        auto createBtns = getCreateButtons(self);

        static_cast<CCNode*>(bars->objectAtIndex(3))->removeFromParentAndCleanup(true);

        for (int i = 0; i < 20; i++) {
            createBtns->removeObjectAtIndex(126);
        }
        for (int i = 0; i < 19; i++) {
            createBtns->removeLastObject();
        }

        fuckingArray->addObject(self->getCreateBtn("bump_01_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("bump_03_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("gravbump_01_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("ring_01_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("ring_03_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("gravring_01_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_01_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_02_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_03_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_04_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_07_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_10_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_05_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_06_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_08_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("portal_09_front_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("boost_01_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("boost_02_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("boost_03_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("boost_04_001.png", 4));

        auto sep = CCNode::create();
        sep->setTag(0);
        fuckingArray->addObject(sep);
        fuckingArray->addObject(self->getCreateBtn("portal_10_back_001.png", 4)); // weird ufo object thing removed in 2.0
        
        if (bars->objectAtIndex(3)) bars->objectAtIndex(3)->release();
        bars->removeObjectAtIndex(3);
        auto bar = EditButtonBar::create(fuckingArray, ccp(winSize.width * 0.5 - 5, getScreenBottom() + getUnkFloat(self) - 6.f), 3, true);
        self->addChild(bar, 10);
        bars->insertObject(bar, 3);
        bar->setVisible(false);

        fuckingArray->removeAllObjects();

        static_cast<CCNode*>(bars->lastObject())->removeFromParentAndCleanup(true);
        fuckingArray->addObject(self->getCreateBtn("edit_eTintBGBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eTintGBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eTintLBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eTintObjBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eTintColObjBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eStartPosBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eGhostDBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eGhostEBtn_001.png", 4));

        sep = CCNode::create();
        sep->setTag(0);
        fuckingArray->addObject(sep);

        fuckingArray->addObject(self->getCreateBtn("edit_eeNoneBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeFTBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeFBBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeFLBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeFRBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeSUBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeSDBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeFALBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeFARBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeFRHBtn_001.png", 4));
        fuckingArray->addObject(self->getCreateBtn("edit_eeFRHInvBtn_001.png", 4));
        
        sep = CCNode::create();
        sep->setTag(0);
        fuckingArray->addObject(sep);

        fuckingArray->addObject(self->getCreateBtn("edit_eLevelEndBtn_001.png", 4)); // level end
        fuckingArray->addObject(self->getCreateBtn("edit_eBGEOn_001.png", 4)); // bg effect on
        fuckingArray->addObject(self->getCreateBtn("edit_eBGEOff_001.png", 4)); // bg effect off
        fuckingArray->addObject(self->getCreateBtn("edit_eeFABtn_001.png", 4)); // scatter transition trigger

        if (bars->lastObject()) bars->lastObject()->release();
        bars->removeLastObject();
        bar = EditButtonBar::create(fuckingArray, ccp(winSize.width * 0.5 - 5, getScreenBottom() + getUnkFloat(self) - 6.f), 8, true);
        bar->setVisible(false);
        self->addChild(bar, 10);
        bars->addObject(bar);

        auto lvl = getLELLevel(getUIEditorLayer(self));

        for (int i = 0; i < bars->count(); i++) {
            auto bar = static_cast<EditButtonBar*>(bars->objectAtIndex(i));
            bar->goToPage(lvl->getLastBuildPageForTab(i));
        }

        self->selectBuildTab(lvl->m_nLastBuildTab);
        getCreateButtonBar(self)->goToPage(lvl->m_nLastBuildPage);
        self->updateCreateMenu(false);

#endif
    }
}

        // for (int i = 0; i < triggerPages->count(); i++) {
        //     CCLog("5");
        //     CCObject* page = triggerPages->objectAtIndex(i);
        //     CCLog("6");
        //     auto menu = static_cast<CCMenu*>(static_cast<CCLayer*>(page)->getChildren()->objectAtIndex(0));
        //     auto children = menu->getChildren();
        //     for (int j = 0; j < menu->getChildrenCount(); j++) {
        //         CCLog("7");
        //         CCObject* btn = children->objectAtIndex(i);
        //         CCLog("8");
        //         static_cast<CCNode*>(btn)->removeFromParentAndCleanup(false);
        //         CCLog("9");
        //         fuckingArray->addObject(btn);
        //         CCLog("10");
        //     }
        // }
        // CCLog("9");
        // auto sep1 = CCNode::create();
        // CCLog("10");
        // sep1->setTag(0);
        // CCLog("11");
        // fuckingArray->insertObject(sep1, 8);
        // auto sep2 = CCNode::create();
        // sep2->setTag(0);
        // CCLog("10");
        // fuckingArray->addObject(sep2);
        // auto interactPages = getBarPages(static_cast<EditButtonBar*>(bars->objectAtIndex(3))); // THREEEEEE
        // CCLog("4");
        // for (int i = 0; i < interactPages->count(); i++) {
        //     CCLog("5");
        //     CCObject* page = interactPages->objectAtIndex(i);
        //     CCLog("6");
        //     auto menu = static_cast<CCMenu*>(static_cast<CCLayer*>(page)->getChildren()->objectAtIndex(0));
        //     auto children = menu->getChildren();
        //     for (int j = 0; j < menu->getChildrenCount(); j++) {
        //         CCLog("7");
        //         CCObject* btn = children->objectAtIndex(i);
        //         CCLog("8");
        //         static_cast<CCNode*>(btn)->removeFromParentAndCleanup(false);
        //         CCLog("9");
        //         fuckingArray->addObject(btn);
        //         CCLog("10");
        //     }
        // }
        // sep1 = CCNode::create();
        // sep1->setTag(0);
        // fuckingArray->addObject(sep1);
// credit to akqanile/adelfa
CCMenuItemSpriteExtra* EditorUI::getSpriteButton2(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale)
{
    auto spr = CCSprite::create(name);
    auto btnSpr = ButtonSprite::create(spr, 32, 0, 32, 1.0, true, "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(btnSpr, 0, this, callback);
    btn->setScale(scale);
    setOriginalScale(btn, scale);

    if (menu) menu->addChild(btn);

    return btn;
}
CCMenuItemSpriteExtra* EditorUI::getSpriteButton3(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale, float sprScale)
{
    auto spr = CCSprite::createWithSpriteFrameName(name);
    spr->setScale(sprScale);
    auto btnSpr = ButtonSprite::create(spr, 32, 0, 32, 1.0, true, "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(btnSpr, 0, this, callback);
    btn->setScale(scale);
    setOriginalScale(btn, scale);

    if (menu) menu->addChild(btn);

    return btn;
}
void EditorUI::moveObjectCall2(CCNode* sender) {
    if (!getSelectedObject(this) && getSelectedObjects(this)->count() <= 0) return;

    auto transform = ccp(0, 0);

    switch (sender->getTag()) {

        case 100001:
            transform.y = 150;
            break;
        case 100002:
            transform.y = -150;
            break;
        case 100003:
            transform.x = -150;
            break;
        case 100004:
            transform.x = 150;
            break;

        case 100005:
            transform.y = 1;
            break;
        case 100006:
            transform.y = -1;
            break;
        case 100007:
            transform.x = -1;
            break;
        case 100008:
            transform.x = 1;
            break;

        case 100009:
            transform.y = 15;
            break;
        case 100010:
            transform.y = -15;
            break;
        case 100011:
            transform.x = -15;
            break;
        case 100012:
            transform.x = 15;
            break;

        default:
            break;
    }
    if (getSelectedObjects(this)->count() > 0) {
        for (int i = 0; i < getSelectedObjects(this)->count(); i++) {
            this->moveObject(static_cast<GameObject*>(getSelectedObjects(this)->objectAtIndex(i)), transform);
        }
    } else {
        this->moveObject(getSelectedObject(this), transform);
    }
    updateObjectInfoLabel(this);
}
void EditorUI::transformObjectCall2(CCNode* sender) {
    if (!getSelectedObject(this) && getSelectedObjects(this)->count() <= 0) return;
    auto selectedObjects = getSelectedObjects(this);

    int tag = sender->getTag();
    float rot = 0;

    switch (tag) {

        case 100013:
            rot = -45;
            break;
        case 100014:
            rot = 45;
            break;
        case 100015:
            rot = -22.5;
            break;
        case 100016:
            rot = 22.5;
            break;
        case 100017:
            rot = -11.25;
            break;
        case 100018:
            rot = 11.25;
            break;
        case 100019:
            rot = -5;
            break;
        case 100020:
            rot = 5;
            break;
        case 100021:
            rot = -1;
            break;
        case 100022:
            rot = 1;
            break;

        default:
            break;
    }
    if (selectedObjects->count() > 0) {
        for (int i = 0; i < selectedObjects->count(); i++) {
            GameObject* obj = static_cast<GameObject*>(selectedObjects->objectAtIndex(i));
            if (tag == 100023) obj->setRotation(0);
            else obj->setRotation(obj->getRotation() + rot);
        }
    } else {
        auto obj = getSelectedObject(this);
        if (tag == 100023) obj->setRotation(0);
        else obj->setRotation(obj->getRotation() + rot);
    }
    updateObjectInfoLabel(this);
}

void (*TRAM_EditorUI_createMoveMenu)(EditorUI* self);
void EditorUI_createMoveMenu(EditorUI* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::EXTRA_EDIT_BUTTONS)) {
        CCArray* buttons = CCArray::create();

        CCMenuItemSpriteExtra* btn;

        btn = self->getSpriteButton("edit_upBtn_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(3);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_downBtn_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(4);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_leftBtn_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(1);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_rightBtn_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(2);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_upBtn2_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(7);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_downBtn2_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(8);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_leftBtn2_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(5);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_rightBtn2_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(6);
        buttons->addObject(btn);



#if GAME_VERSION < GV_1_5
        btn = self->getSpriteButton2("edit_upBtn3_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(100001);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_downBtn3_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(100002);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_leftBtn3_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(100003);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rightBtn3_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(100004);
        buttons->addObject(btn);
#else
        btn = self->getSpriteButton("edit_upBtn3_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(11);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_downBtn3_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(12);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_leftBtn3_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(9);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_rightBtn3_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(10);
        buttons->addObject(btn);
#endif



        btn = self->getSpriteButton3("edit_upBtn_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9, 0.8);
        btn->setTag(100005);
        buttons->addObject(btn);
        btn = self->getSpriteButton3("edit_downBtn_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9, 0.8);
        btn->setTag(100006);
        buttons->addObject(btn);
        btn = self->getSpriteButton3("edit_leftBtn_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9, 0.8);
        btn->setTag(100007);
        buttons->addObject(btn);
        btn = self->getSpriteButton3("edit_rightBtn_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9, 0.8);
        btn->setTag(100008);
        buttons->addObject(btn);
        


        btn = self->getSpriteButton2("edit_upBtn5_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(100009);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_downBtn5_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(100010);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_leftBtn5_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(100011);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rightBtn5_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(100012);
        buttons->addObject(btn);



        btn = self->getSpriteButton("edit_flipXBtn_001.png", menu_selector(EditorUI::transformObjectCall), nullptr, 0.9);
#if GAME_VERSION < GV_1_5
        btn->setTag(9);
#elif GAME_VERSION == GV_1_5
        btn->setTag(13);
#else
        btn->setTag(17);
#endif
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_flipYBtn_001.png", menu_selector(EditorUI::transformObjectCall), nullptr, 0.9);
#if GAME_VERSION < GV_1_5
        btn->setTag(10);
#elif GAME_VERSION == GV_1_5
        btn->setTag(14);
#else
        btn->setTag(18);
#endif
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_cwBtn_001.png", menu_selector(EditorUI::transformObjectCall), nullptr, 0.9);
#if GAME_VERSION < GV_1_5
        btn->setTag(11);
#elif GAME_VERSION == GV_1_5
        btn->setTag(15);
#else
        btn->setTag(19);
#endif
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_ccwBtn_001.png", menu_selector(EditorUI::transformObjectCall), nullptr, 0.9);
#if GAME_VERSION < GV_1_5
        btn->setTag(12);
#elif GAME_VERSION == GV_1_5
        btn->setTag(16);
#else
        btn->setTag(20);
#endif
        buttons->addObject(btn);



        btn = self->getSpriteButton2("edit_rotate45rBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100014);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotate45lBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100013);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotate22rBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100016);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotate22lBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100015);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotate11rBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100018);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotate11lBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100017);
        buttons->addObject(btn);

        btn = self->getSpriteButton2("edit_rotate5rBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100020);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotate5lBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100019);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotate1rBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100022);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotate1lBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100021);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rotateResetBtn_001.png", menu_selector(EditorUI::transformObjectCall2), nullptr, 0.9);
        btn->setTag(100023);
        buttons->addObject(btn);

#if GAME_VERSION < GV_1_4
        btn = self->getSpriteButton("edit_editObjBtn_001.png", menu_selector(EditorUI::editObject), nullptr, 0.9);
        btn->setTag(12);
        buttons->addObject(btn);
        setEditObjectButton(self, btn);
#endif

        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
#if GAME_VERSION < GV_1_6
        EditButtonBar* newBar = EditButtonBar::create(buttons, ccp(winSize.width * 0.5 - 5, getScreenBottom() + getUnkFloat(self) - 6.f));
#elif GAME_VERSION == GV_1_6
        EditButtonBar* newBar = EditButtonBar::create(buttons, ccp(winSize.width * 0.5 - 5, getScreenBottom() + getUnkFloat(self) - 6.f), false);
#else
        EditButtonBar* newBar = EditButtonBar::create(buttons, ccp(winSize.width * 0.5 - 5, getScreenBottom() + getUnkFloat(self) - 6.f), 0, false);
#endif
        setEditButtonBar(self, newBar);
        self->addChild(newBar, 11);
    } else {
        TRAM_EditorUI_createMoveMenu(self);
    }
}

void (*TRAM_EditorUI_selectObject)(EditorUI* self, GameObject* obj);
void EditorUI_selectObject(EditorUI* self, GameObject* obj) {
    TRAM_EditorUI_selectObject(self, obj);
    updateObjectInfoLabel(self);
}
void (*TRAM_EditorUI_selectObjectsInRect)(EditorUI* self, CCRect rect);
void EditorUI_selectObjectsInRect(EditorUI* self, CCRect rect) {
    CCLog("select objects in rekt");
    TRAM_EditorUI_selectObjectsInRect(self, rect);
    updateObjectInfoLabel(self);
}
void (*TRAM_EditorUI_deselectObject)(EditorUI* self);
void EditorUI_deselectObject(EditorUI* self) {
    TRAM_EditorUI_deselectObject(self);
    updateObjectInfoLabel(self);
}
#if GAME_VERSION < GV_1_7
void (*TRAM_EditorUI_deselectAll)(EditorUI* self);
void EditorUI_deselectAll(EditorUI* self) {
    TRAM_EditorUI_deselectAll(self);
    updateObjectInfoLabel(self);
}
void (*TRAM_EditorUI_moveObjectCall)(EditorUI* self, CCNode* node);
void EditorUI_moveObjectCall(EditorUI* self, CCNode* node) {
    TRAM_EditorUI_moveObjectCall(self, node);
    updateObjectInfoLabel(self);
}
void (*TRAM_EditorUI_transformObjectCall)(EditorUI* self, CCNode* node);
void EditorUI_transformObjectCall(EditorUI* self, CCNode* node) {
    TRAM_EditorUI_transformObjectCall(self, node);
    updateObjectInfoLabel(self);
}
#else
void (*TRAM_EditorUI_deselectAll)(EditorUI* self, CCObject* sender);
void EditorUI_deselectAll(EditorUI* self, CCObject* sender) {
    TRAM_EditorUI_deselectAll(self, sender);
    updateObjectInfoLabel(self);
}
void (*TRAM_EditorUI_moveObjectCall)(EditorUI* self, CCObject* node);
void EditorUI_moveObjectCall(EditorUI* self, CCObject* node) {
    TRAM_EditorUI_moveObjectCall(self, node);
    updateObjectInfoLabel(self);
}
void (*TRAM_EditorUI_transformObjectCall)(EditorUI* self, CCObject* node);
void EditorUI_transformObjectCall(EditorUI* self, CCObject* node) {
    TRAM_EditorUI_transformObjectCall(self, node);
    updateObjectInfoLabel(self);
}
#endif

void (*TRAM_EditorUI_destructor)(EditorUI* self);
void EditorUI_destructor(EditorUI* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.editorObjectInfo) {
        hax.editorObjectInfo->removeFromParentAndCleanup(true);
    }
    hax.editorObjectInfo = nullptr;
    TRAM_EditorUI_destructor(self);
}
void (*TRAM_EditorUI_destructor2)(EditorUI* self);
void EditorUI_destructor2(EditorUI* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.editorObjectInfo) {
        hax.editorObjectInfo->removeFromParentAndCleanup(true);
    }
    hax.editorObjectInfo = nullptr;
    TRAM_EditorUI_destructor2(self);
}

#if GAME_VERSION >= GV_1_5
void (*TRAM_EditorUI_onDeleteSelected)(EditorUI* self);
void EditorUI_onDeleteSelected(EditorUI* self) {
    TRAM_EditorUI_onDeleteSelected(self);
    updateObjectInfoLabel(self);
}
void (*TRAM_EditorUI_onDuplicate)(EditorUI* self);
void EditorUI_onDuplicate(EditorUI* self) {
    TRAM_EditorUI_onDuplicate(self);
    updateObjectInfoLabel(self);
}
#endif
#if GAME_VERSION >= GV_1_6
void (*TRAM_EditorUI_onDeleteStartPos)(EditorUI* self);
void EditorUI_onDeleteStartPos(EditorUI* self) {
    TRAM_EditorUI_onDeleteStartPos(self);
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::SHOW_OBJECT_INFO)) return;
    
    auto sel = getSelectedObjects(self);
    auto selObj = getSelectedObject(self);
    if (selObj && getObjectKey(selObj) == 31) {
        self->deselectObject();
    }
    if (sel && sel->count() > 0) {
        for (int i = 0; i < sel->count(); i++) {
            auto currObjUncast = sel->objectAtIndex(i);
            if (!currObjUncast) {
                i--;
                continue;
            }
            auto currObj = dynamic_cast<GameObject*>(currObjUncast);
            if (currObj && getObjectKey(currObj) == 31) {
                sel->removeObject(currObj);
                currObj->setColor(ccWHITE);
                i--;
            }
        }
    }
    updateObjectInfoLabel(self);
}
#endif

void EditorUI_om() {
    Omni::hook("_ZN8EditorUI12showMaxErrorEv",
        reinterpret_cast<void*>(EditorUI_showMaxError),
        reinterpret_cast<void**>(&TRAM_EditorUI_showMaxError));
    Omni::hook("_ZN8EditorUI26constrainGameLayerPositionEv",
        reinterpret_cast<void*>(EditorUI_constrainGameLayerPosition),
        reinterpret_cast<void**>(&TRAM_EditorUI_constrainGameLayerPosition));
#if GAME_VERSION < GV_1_5
    Omni::hook("_ZN8EditorUI7zoomOutEv",
        reinterpret_cast<void*>(EditorUI_zoomOut),
        reinterpret_cast<void**>(&TRAM_EditorUI_zoomOut));
#else
    Omni::hook("_ZN8EditorUI16onDeleteSelectedEv",
        reinterpret_cast<void*>(EditorUI_onDeleteSelected),
        reinterpret_cast<void**>(&TRAM_EditorUI_onDeleteSelected));
    Omni::hook("_ZN8EditorUI11onDuplicateEv",
        reinterpret_cast<void*>(EditorUI_onDuplicate),
        reinterpret_cast<void**>(&TRAM_EditorUI_onDuplicate));
#endif
    Omni::hook("_ZN8EditorUI4initEP16LevelEditorLayer",
        reinterpret_cast<void*>(EditorUI_init),
        reinterpret_cast<void**>(&TRAM_EditorUI_init));
#if GAME_VERSION < GV_1_6
    Omni::hook("_ZN8EditorUI15setupDeleteMenuEv",
        reinterpret_cast<void*>(EditorUI_setupDeleteMenu),
        reinterpret_cast<void**>(&TRAM_EditorUI_setupDeleteMenu));
#else
    Omni::hook("_ZN8EditorUI16onDeleteStartPosEv",
        reinterpret_cast<void*>(EditorUI_onDeleteStartPos),
        reinterpret_cast<void**>(&TRAM_EditorUI_onDeleteStartPos));
#endif
    Omni::hook("_ZN8EditorUI15setupCreateMenuEv",
        reinterpret_cast<void*>(EditorUI_setupCreateMenu),
        reinterpret_cast<void**>(&TRAM_EditorUI_setupCreateMenu));
    Omni::hook("_ZN8EditorUI14createMoveMenuEv",
        reinterpret_cast<void*>(EditorUI_createMoveMenu),
        reinterpret_cast<void**>(&TRAM_EditorUI_createMoveMenu));
    Omni::hook("_ZN8EditorUI12selectObjectEP10GameObject",
        reinterpret_cast<void*>(EditorUI_selectObject),
        reinterpret_cast<void**>(&TRAM_EditorUI_selectObject));
    Omni::hook("_ZN8EditorUI19selectObjectsInRectEN7cocos2d6CCRectE",
        reinterpret_cast<void*>(EditorUI_selectObjectsInRect),
        reinterpret_cast<void**>(&TRAM_EditorUI_selectObjectsInRect));
    Omni::hook("_ZN8EditorUI14deselectObjectEv",
        reinterpret_cast<void*>(EditorUI_deselectObject),
        reinterpret_cast<void**>(&TRAM_EditorUI_deselectObject));
#if GAME_VERSION < GV_1_7
    Omni::hook("_ZN8EditorUI11deselectAllEv",
        reinterpret_cast<void*>(EditorUI_deselectAll),
        reinterpret_cast<void**>(&TRAM_EditorUI_deselectAll));
    Omni::hook("_ZN8EditorUI14moveObjectCallEPN7cocos2d6CCNodeE",
        reinterpret_cast<void*>(EditorUI_moveObjectCall),
        reinterpret_cast<void**>(&TRAM_EditorUI_moveObjectCall));
    Omni::hook("_ZN8EditorUI19transformObjectCallEPN7cocos2d6CCNodeE",
        reinterpret_cast<void*>(EditorUI_transformObjectCall),
        reinterpret_cast<void**>(&TRAM_EditorUI_transformObjectCall));
#else
    Omni::hook("_ZN8EditorUI11deselectAllEPN7cocos2d8CCObjectE",
        reinterpret_cast<void*>(EditorUI_deselectAll),
        reinterpret_cast<void**>(&TRAM_EditorUI_deselectAll));
    Omni::hook("_ZN8EditorUI14moveObjectCallEPN7cocos2d8CCObjectE",
        reinterpret_cast<void*>(EditorUI_moveObjectCall),
        reinterpret_cast<void**>(&TRAM_EditorUI_moveObjectCall));
    Omni::hook("_ZN8EditorUI19transformObjectCallEPN7cocos2d8CCObjectE",
        reinterpret_cast<void*>(EditorUI_transformObjectCall),
        reinterpret_cast<void**>(&TRAM_EditorUI_transformObjectCall));
#endif
        
#if GAME_VERSION < GV_1_5
    Omni::hook("_ZN8EditorUID1Ev",
        reinterpret_cast<void*>(EditorUI_destructor),
        reinterpret_cast<void**>(&TRAM_EditorUI_destructor));
#endif
    Omni::hook("_ZN8EditorUID2Ev",
        reinterpret_cast<void*>(EditorUI_destructor2),
        reinterpret_cast<void**>(&TRAM_EditorUI_destructor2));
}