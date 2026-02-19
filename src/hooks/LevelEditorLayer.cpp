#include "hook.hpp"
#include "LevelEditorLayer.hpp"
#include "GJGameLevel.hpp"
#include "LevelSettingsObject.hpp"

// original 16k fix code: https://github.com/cierra-kb/legacy-starry-sky/blob/main/src/modules/editor.cpp

void LevelEditorLayer_update(LevelEditorLayer* self, float dt) {
    CCRect visible_rect;
    
    auto vr_origin = getEditorGameLayer(self)->convertToNodeSpace(CCPoint(0,0));
    auto vr_dest = CCDirector::sharedDirector()->getWinSize();
    
    visible_rect.origin = vr_origin;
    visible_rect.size = vr_dest;
    
    // handle zooming
    visible_rect.size.width *= 1 / getEditorGameLayer(self)->getScale();
    visible_rect.size.height *= 1 / getEditorGameLayer(self)->getScale();

    // fix blocks disappearing where it shouldn't by extending the visible rect by a few blocks
    visible_rect.origin.x -= 75;
    visible_rect.origin.y -= 75;
    
    visible_rect.size.width += 150;
    visible_rect.size.height += 150;

    auto bn = getEditorBatchNode(self);

    for (uint32_t section_id = 0; section_id < getEditorSections(self)->count(); section_id++) {
        CCArray* section_objects = static_cast<CCArray*>(getEditorSections(self)->objectAtIndex(section_id));

        for (uint32_t index = 0; index < section_objects->count(); index++) {
            GameObject* object = static_cast<GameObject*>(section_objects->objectAtIndex(index));
            CCPoint object_pos = object->getPosition();

#if GAME_VERSION < GV_1_7
            if (CCRect::CCRectContainsPoint(visible_rect, object_pos)) {
#else
            if (visible_rect.containsPoint(object_pos)) {
#endif
                if (!object->getParent()) {
                    OrderingData* s = static_cast<OrderingData*>(object->getUserData());

                    bn->addChild(object, s->z_order);
                    //object->setOrderOfArrival(s->order_of_arrival);

                    bn->sortAllChildren();
                }
            } else {
                if (object->getParent()) {
                    if (object->getUserData() == nullptr) {
                        OrderingData s = OrderingData {object->getOrderOfArrival(), object->getZOrder()};
                        object->setUserData((void*)&s);
                    }

                    bn->removeChild(object, false);
                }
            }

        }
    }
}

bool (*TRAM_LevelEditorLayer_init)(LevelEditorLayer* self, GJGameLevel* level);
bool LevelEditorLayer_init(LevelEditorLayer* self, GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::OBJECT_LIMIT_BYPASS)) {
        if (hax.getModuleEnabled(ModuleID::_16K_FIX))
            setObjectLimit(2147483646);
        else
            setObjectLimit(INCREASED_OBJECT_LIMIT - 1);
    }
    else
        setObjectLimit(OBJECT_LIMIT);
    setZoomBypass(hax.getModuleEnabled(ModuleID::ZOOM_BYPASS));
    setFreeBuild(hax.getModuleEnabled(ModuleID::FREE_BUILD));

    if (!TRAM_LevelEditorLayer_init(self, level)) return false;

    if (hax.getModuleEnabled(ModuleID::_16K_FIX)) {
        // https://gist.github.com/netguy204/6097063
#if GAME_VERSION < GV_1_7
        void** vtable = *(void***)self;
        void (LevelEditorLayer::* ptr)(float) = &LevelEditorLayer::update;
        void* offset = *(void**)&ptr;
#if GAME_VERSION < GV_1_5
        // this crashes on 1.5
        vtable[((uintptr_t)offset)/sizeof(void*)] = (void*)&LevelEditorLayer_update;
#else
        DobbyCodePatch(&vtable[((uintptr_t)offset)/sizeof(void*)], uintptrToBytes((uintptr_t)&LevelEditorLayer_update).data(), 4);
#endif
#else
        DobbyCodePatch(reinterpret_cast<void*>(get_address(__LevelEditorLayer_update_v)),
        uintptrToBytes((uintptr_t)&LevelEditorLayer_update).data(), 4);
#endif
        CCLog("update schedule");
        self->scheduleUpdate();
    }

    hax.hitboxLayerEditor = HitboxLayerEditor::create(self);
    getEditorGameLayer(self)->addChild(hax.hitboxLayerEditor, 98);

#if GDPS == GDPS_NEOPOINTFOUR
    auto editorObject = getEditorSettingsObject(self);
    editorObject->setAudioTrack(level->m_nAudioTrack);
#endif

    return true;
}

void (*TRAM_LevelEditorLayer_createObjectsFromSetup)(LevelEditorLayer* self, std::string str);
void LevelEditorLayer_createObjectsFromSetup(LevelEditorLayer* self, std::string str) {
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::_16K_FIX)) return TRAM_LevelEditorLayer_createObjectsFromSetup(self, str);

    if (str.empty() || str.c_str() == " ")
        return;

    int s_globalOrderOfArrival = getGlobalOrderOfArrival();

    std::stringstream ss(
        std::string(str.c_str())
    );
    std::string split_buffer;

    if (!std::getline(ss, split_buffer, ';'))
        return; // no level settings string?

    setEditorSettingsObject(self, LevelSettingsObject::objectFromString(split_buffer));
    getEditorSettingsObject(self)->retain();

#if GAME_VERSION < GV_1_6
    while (std::getline(ss, split_buffer, ';'))
    {
        std::string objstr = CCString::createWithFormat("%s", split_buffer.c_str())->m_sString;
        GameObject* obj = GameObject::objectFromString(objstr);
        
        OrderingData s = OrderingData {s_globalOrderOfArrival++, obj->getZOrder()};
        obj->setUserData((void*)&s);
        obj->retain();

        self->addToSection(obj);
        getEditorBatchNode(self)->addChild(obj);

        if (getObjectType(obj) == 7 && getShouldSpawn(obj))
            getGridLayer(self)->addToEffects(obj);
    }
#else
    while (std::getline(ss, split_buffer, ';'))
    {
        std::string objstr = CCString::createWithFormat("%s", split_buffer.c_str())->m_sString;
        self->addObjectFromString(objstr);
    }
#endif
}


HitboxLayerEditor* HitboxLayerEditor::create(LevelEditorLayer* self) {
    auto ret = new HitboxLayerEditor;
    if (ret->init(self)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool HitboxLayerEditor::init(LevelEditorLayer* self) {
    if (!CCLayer::init()) return false;
    parent = self;
    return true;
}

#if GAME_VERSION < GV_1_7
CCRect getRectOnCamera(LevelEditorLayer* layer, CCRect otherRect) {
    return CCRect(
        (CCRect::CCRectGetMinX(otherRect)),
        (CCRect::CCRectGetMinY(otherRect)),
        (CCRect::CCRectGetMaxX(otherRect)),
        (CCRect::CCRectGetMaxY(otherRect))
    );
}
void drawRect(LevelEditorLayer* layer, CCRect rect) {
    auto cameraRect = getRectOnCamera(layer, rect);
    ccDrawRect(
        ccp(CCRect::CCRectGetMinX(cameraRect) + 0.75, CCRect::CCRectGetMinY(cameraRect) + 0.75), 
        ccp(
            CCRect::CCRectGetMaxX(cameraRect) - CCRect::CCRectGetMinX(cameraRect) - 0.75, 
            CCRect::CCRectGetMaxY(cameraRect) - CCRect::CCRectGetMinY(cameraRect) - 0.75
        )
    );
}
#else
CCRect getRectOnCamera(LevelEditorLayer* layer, CCRect otherRect) {
    auto camera = getEditorGameLayer(layer)->convertToNodeSpace(CCPoint(0,0));
    return CCRect(
        otherRect.getMinX(),
        otherRect.getMinY(),
        otherRect.size.width,
        otherRect.size.height
    );
}
void drawRect(LevelEditorLayer* layer, CCRect rect) {
    auto cameraRect = getRectOnCamera(layer, rect);
    ccDrawRect(
        ccp(cameraRect.getMinX() + 0.75, cameraRect.getMinY() + 0.75), 
        ccp(
            cameraRect.getMaxX() - 0.75, 
            cameraRect.getMaxY() - 0.75
        )
    );
}
#endif

void HitboxLayerEditor::draw() {
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::SHOW_HITBOXES_EDITOR)) return;

    GLint originalSrcFunc, originalDestFunc;
    glGetIntegerv(GL_BLEND_SRC_RGB, &originalSrcFunc);
    glGetIntegerv(GL_BLEND_DST_RGB, &originalDestFunc);

    auto scale = getEditorGameLayer(parent)->getScale();

    if (scale < 0.6) {
        glLineWidth(1);
    } else if (scale < 0.8) {
        glLineWidth(2);
    } else {
        glLineWidth(3);
    }
    glBlendFunc(GL_ONE, GL_ZERO);
#if GAME_VERSION >= GV_1_7
    auto getObjectRect = (CCRect(*)(GameObject*))(DobbySymbolResolver(MAIN_LIBRARY, "_ZN10GameObject13getObjectRectEv"));
    auto getObjectRectFF = (CCRect(*)(GameObject*, float, float))(DobbySymbolResolver(MAIN_LIBRARY, "_ZN10GameObject13getObjectRectEff"));
    #define _getObjectRect(obj) getObjectRect(obj);
    #define _getObjectRectFF(obj, scaleX, scaleY) getObjectRectFF(obj, scaleX, scaleY);
#else
    #define _getObjectRect(obj) obj->getObjectRect();
    #define _getObjectRectFF(obj, scaleX, scaleY) obj->getObjectRect(scaleX, scaleY);
#endif

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto bottomLeft = getEditorGameLayer(parent)->convertToNodeSpace(CCPoint(0,0));
    auto topRight = ccp(bottomLeft.x + winSize.width / scale, bottomLeft.y + winSize.height / scale);

    auto sections = getEditorSections(parent);

    int a1 = floorf(bottomLeft.x / 100);
    int a2 = floorf(topRight.x / 100);
    int a3 = sections->count() - 1;
    auto leftmostSection = std::max(0, a1);
    auto rightmostSection = std::min(a3, a2);
    for (int i = leftmostSection; i <= rightmostSection; i++) {
        auto section = static_cast<CCArray*>(sections->objectAtIndex(i));
        for (int j = 0; j < section->count(); j++) {
            auto object = static_cast<GameObject*>(section->objectAtIndex(j));
            CCRect actualRect;
            auto rect = _getObjectRect(object);
            actualRect.size = rect.size;
            actualRect.origin = ccp(
                object->getPositionX() - actualRect.size.width * object->getAnchorPoint().x,
                object->getPositionY() - actualRect.size.height * object->getAnchorPoint().y
            );
            // no touch triggered support yet
#if GAME_VERSION < GV_1_7
            if (CCRect::CCRectGetMaxY(actualRect) < bottomLeft.y) continue;
            if (CCRect::CCRectGetMinY(actualRect) > topRight.y) continue;
#else
            auto objID = getObjectKey(object);
            if (objID == 29 || objID == 30 || objID == 104 || objID == 105 || objID == 221) continue;
            if (rect.getMaxY() < bottomLeft.y) continue;
            if (rect.getMinY() > topRight.y) continue;
#endif
            switch (getObjectType(object)) {
                case 0:
                    ccDrawColor4B(0, 0, 255, 255);
                    drawRect(parent, actualRect);
                    break;
                case 2:
                    ccDrawColor4B(255, 0, 0, 255);
                #if GAME_VERSION >= GV_1_4
                    if (getRadius(object) > 0) {
                        ccDrawCircle(
                            object->getPosition(),
                            getRadius(object),
                            0, 24, false
                        );
                        break;
                    }
                #endif
                    drawRect(parent, actualRect);
                    break;
                case GameObjectType::ReverseGravityPortal:
                case GameObjectType::NormalGravityPortal:
                case GameObjectType::IconPortal:
                case GameObjectType::ShipPortal:
                case GameObjectType::YellowOrb:
                case GameObjectType::YellowPad:
                // 1.02 only
                case GameObjectType::MirrorPortal:
                case GameObjectType::UnmirrorPortal:
                // 1.11 only
            #if GAME_VERSION >= GV_1_1
                case GameObjectType::BallPortal:
            #endif
            #if GAME_VERSION >= GV_1_3
                case GameObjectType::BlueOrb:
                case GameObjectType::BluePad:
            #endif
            #if GAME_VERSION >= GV_1_4
                case GameObjectType::NormalSizePortal:
                case GameObjectType::MiniSizePortal:
            #endif
            #if GAME_VERSION >= GV_1_5
                case GameObjectType::PinkOrb:
                case GameObjectType::PinkPad:
                case GameObjectType::BirdPortal:
            #endif
            #if GAME_VERSION >= GV_1_6
                case GameObjectType::BreakableBlock:
                case GameObjectType::SecretCoin:
            #endif
            #if GAME_VERSION >= GV_1_7
                case GameObjectType::SpeedPortal:
            #endif
                    ccDrawColor4B(0, 255, 0, 255);
                    drawRect(parent, actualRect);
                    break;
            }
        }
    }
    glLineWidth(1);
    glBlendFunc(originalSrcFunc, originalDestFunc);
}

void LevelEditorLayer_om() {
    Omni::hook("_ZN16LevelEditorLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(LevelEditorLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelEditorLayer_init));
    Omni::hook("_ZN16LevelEditorLayer22createObjectsFromSetupESs",
        reinterpret_cast<void*>(LevelEditorLayer_createObjectsFromSetup),
        reinterpret_cast<void**>(&TRAM_LevelEditorLayer_createObjectsFromSetup));
}