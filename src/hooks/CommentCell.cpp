#include "hook.hpp"
#if GDPS == GDPS_NEOPOINTFOUR
#include "Neopointfour/GJComment.cpp"

std::string timeToString(time_t input) {
    struct tm timeInfo;
    localtime_r(&input, &timeInfo);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &timeInfo);
    return std::string(buffer);
}

int getCommentDate(void* comment);
#endif

void (*TRAM_CommentCell_loadFromComment)(CCNode* self, CCNode* comment);
void CommentCell_loadFromComment(CCNode* self, CCNode* comment) {
    TRAM_CommentCell_loadFromComment(self, comment);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::COMMENT_IDS)) {
#if GDPS == GDPS_NEOPOINTFOUR
        int commentTimestamp = getCommentDate(comment);
        std::string commentDate = (commentTimestamp > 0) ? timeToString(static_cast<time_t>(commentTimestamp)) : "NA";
        auto idLabel = CCLabelBMFont::create(commentDate.c_str(), "chatFont.fnt");
#else
        auto idLabel = CCLabelBMFont::create(fmt::format("#{}", getCommentID(comment)).c_str(), "chatFont.fnt");
#endif
        idLabel->setAnchorPoint({1, 0.5});
        idLabel->setScale(0.5f);
        idLabel->setPosition(ccp(330, 15));
        idLabel->setColor(ccc3(0, 0, 0));
        idLabel->setOpacity(127);
        idLabel->setTag(6741);
        self->addChild(idLabel, 1000);
    }
#if GAME_VERSION < GV_1_3
    if (hax.getModuleEnabled(ModuleID::COMMENT_OFFSET_FIX)) {
        auto parent = MEMBER_BY_OFFSET(CCNode*, self, CommentCell__m_parent);
        auto textArea = static_cast<CCSprite*>(parent->getChildren()->objectAtIndex(2));
        textArea->setAnchorPoint(ccp(0, 0.5));
        textArea->setPositionX(10);
    }
// #elif GAME_VERSION == GV_1_3
//     auto parent = MEMBER_BY_OFFSET(CCNode*, self, 0x17c);
//     auto textArea = static_cast<CCSprite*>(parent->getChildren()->objectAtIndex(2));
//     CCLog("%f", textArea->getPosition().x);
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

void CommentCell_om() {
    Omni::hook("_ZN11CommentCell15loadFromCommentEP9GJComment",
        reinterpret_cast<void*>(CommentCell_loadFromComment),
        reinterpret_cast<void**>(&TRAM_CommentCell_loadFromComment));
    // Omni::hook("_ZN9LevelCell4drawEv",
    //     reinterpret_cast<void*>(LevelCell_draw),
    //     reinterpret_cast<void**>(&TRAM_LevelCell_draw));
}