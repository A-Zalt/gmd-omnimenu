#pragma once

#include <cocos2d.h>

class UILayer : public cocos2d::CCLayer {
public:
    static UILayer* create(void*);
    bool init(void*);

    void pCommand(CCNode*);

    void speedUp();
    void speedDown();
    void gravityUp();
    void gravityDown();
    void yStartUp();
    void yStartDown();
};
