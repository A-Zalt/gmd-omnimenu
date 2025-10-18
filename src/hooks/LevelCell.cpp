#include "hook.hpp"
#include "GJGameLevel.hpp"

void (*TRAM_LevelCell_loadCustomLevelCell)(CCNode* self);
void LevelCell_loadCustomLevelCell(CCNode* self) {
    TRAM_LevelCell_loadCustomLevelCell(self);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("level_ids_in_list")) {
        auto idLabel = CCLabelBMFont::create(CCString::createWithFormat("#%i", getLevelID(getCellLevel(self)))->getCString(), "chatFont.fnt");
        idLabel->setAnchorPoint({1, 0.5});
        idLabel->setScale(0.5f);
        idLabel->setPosition(ccp(345, 60));
        idLabel->setColor(ccc3(0, 0, 0));
        idLabel->setOpacity(127);
        idLabel->setTag(6741);
        getCellMainLayer(self)->addChild(idLabel, 1000);
    }
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