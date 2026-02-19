#include "hook.hpp"
#include "GJGameLevel.hpp"

void (*TRAM_LevelCell_loadCustomLevelCell)(CCNode* self);
void LevelCell_loadCustomLevelCell(CCNode* self) {
    TRAM_LevelCell_loadCustomLevelCell(self);
    HaxManager& hax = HaxManager::sharedState();
    auto lvl = getCellLevel(self);
    if (hax.getModuleEnabled(ModuleID::LEVEL_IDS_IN_SEARCH)) {
        auto idLabel = CCLabelBMFont::create(fmt::format("#{}", lvl->m_nLevelID).c_str(), "chatFont.fnt");
        idLabel->setAnchorPoint({1, 0.5});
        idLabel->setScale(0.5f);
        idLabel->setPosition(ccp(345, 60));
        idLabel->setColor(ccc3(0, 0, 0));
        idLabel->setOpacity(127);
        idLabel->setTag(6741);
        self->addChild(idLabel, 1000);
    }
#if GDPS == GDPS_NEOPOINTFOUR
    if (hax.originalIDMap.find(lvl) != hax.originalIDMap.end() && hax.originalIDMap[lvl] > 0) {
        
        std::string searchText = "By " + std::string(lvl->m_sUserName);
        
        std::function<CCLabelBMFont*(CCNode*)> findLabel = [&](CCNode* node) -> CCLabelBMFont* {
            CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(node);
            if (label && std::string(label->getString()) == searchText) {
                return label;
            }
            
            CCArray* children = node->getChildren();
            if (children) {
                for (int i = 0; i < children->count(); i++) {
                    CCNode* child = (CCNode*)children->objectAtIndex(i);
                    CCLabelBMFont* found = findLabel(child);
                    if (found) return found;
                }
            }
            return nullptr;
        };
        
        CCLabelBMFont* label = findLabel(self);
        if (label) {
            CCSprite* copyMark = CCSprite::create(COPY_MARK_TEXTURE);
            if (copyMark) {
                CCPoint labelPos = label->getPosition();
                float labelWidth = label->getContentSize().width;
                float labelScale = label->getScale();
                
                float anchorX = label->getAnchorPoint().x;
                float rightEdge = labelPos.x + (labelWidth * labelScale * (1.0f - anchorX));
                
                copyMark->setPosition(ccp(rightEdge + 12.5f, labelPos.y - 2));
                copyMark->setScale(0.9f);
                
                copyMark->_setZOrder(10);
                
                label->getParent()->addChild(copyMark);
            }
        }
    }
    auto val = hax.featureTypeMap[lvl];
    if (val == 0) return;
    CCSprite* featureFrame = CCSprite::create((val > 1) ? MAGIC_TEXTURE : FEATURED_TEXTURE);
    featureFrame->setPosition({26.f, 40.f});
    if (lvl->m_nStars < 1) featureFrame->setPosition({26.f, 35.f}); // Featured no stars
    featureFrame->_setZOrder(-1);
    self->addChild(featureFrame);
#endif
}
// void (*TRAM_LevelCell_draw)(CCNode* self);
// void LevelCell_draw(CCNode* self) {
//     TRAM_LevelCell_draw(self);
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.getModuleEnabled("level_ids_in_list")) {
//         CCNode* child = getCellMainLayer(self)->getChildByTag(6741);
//         child->setVisible(true);
//     }
// }

void LevelCell_om() {
    Omni::hook("_ZN9LevelCell19loadCustomLevelCellEv",
        reinterpret_cast<void*>(LevelCell_loadCustomLevelCell),
        reinterpret_cast<void**>(&TRAM_LevelCell_loadCustomLevelCell));
    // Omni::hook("_ZN9LevelCell4drawEv",
    //     reinterpret_cast<void*>(LevelCell_draw),
    //     reinterpret_cast<void**>(&TRAM_LevelCell_draw));
}