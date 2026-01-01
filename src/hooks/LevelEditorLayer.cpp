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

void LevelEditorLayer_om() {
    Omni::hook("_ZN16LevelEditorLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(LevelEditorLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelEditorLayer_init));
    Omni::hook("_ZN16LevelEditorLayer22createObjectsFromSetupESs",
        reinterpret_cast<void*>(LevelEditorLayer_createObjectsFromSetup),
        reinterpret_cast<void**>(&TRAM_LevelEditorLayer_createObjectsFromSetup));
}