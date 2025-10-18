#include "hook.hpp"

void (*TRAM_CommentCell_loadFromComment)(CCNode* self, CCNode* comment);
void CommentCell_loadFromComment(CCNode* self, CCNode* comment) {
    TRAM_CommentCell_loadFromComment(self, comment);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled("comment_ids")) {
        auto idLabel = CCLabelBMFont::create(CCString::createWithFormat("#%i", getCommentID(comment))->getCString(), "chatFont.fnt");
        idLabel->setAnchorPoint({1, 0.5});
        idLabel->setScale(0.5f);
        idLabel->setPosition(ccp(330, 15));
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

void CommentCell_om() {
    Omni::hook("_ZN11CommentCell15loadFromCommentEP9GJComment",
        reinterpret_cast<void*>(CommentCell_loadFromComment),
        reinterpret_cast<void**>(&TRAM_CommentCell_loadFromComment));
    // Omni::hook("_ZN9LevelCell4drawEv",
    //     reinterpret_cast<void*>(LevelCell_draw),
    //     reinterpret_cast<void**>(&TRAM_LevelCell_draw));
}