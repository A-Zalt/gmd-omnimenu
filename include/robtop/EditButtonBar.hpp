#pragma once

#include <cocos2d.h>

class EditButtonBar : public cocos2d::CCNode {
public:
#if GAME_VERSION < GV_1_6
    static EditButtonBar* create(CCArray*, CCPoint);
#elif GAME_VERSION == GV_1_6
    static EditButtonBar* create(CCArray*, CCPoint, bool);
#else
    static EditButtonBar* create(CCArray*, CCPoint, int, bool);
    void goToPage(int page);
#endif
};
