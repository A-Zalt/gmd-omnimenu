#pragma once

#include <cocos2d.h>

class HaxLayer : public cocos2d::CCLayer {

public:
    static HaxLayer* create(bool fromRope);
    static cocos2d::CCScene* scene(bool fromRope);

protected:

    bool init(bool fromRope);

    void onClose(CCObject*);
    void keyBackClicked();

    cocos2d::CCSprite* m_background = nullptr;
    bool m_fromRope = false;

};