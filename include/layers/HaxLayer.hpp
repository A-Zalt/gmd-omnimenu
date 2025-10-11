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

    void onNoClip(CCObject*);
    void onIconHack(CCObject*);
    void onTextLengthBypass(CCObject*);
    void onFilterBypass(CCObject*);
    void onSwearBypass(CCObject*);
    void onLevelEdit(CCObject*);
    void onInstantComplete(CCObject*);
    void onFastMenu(CCObject*);
    void onVerifyHack(CCObject*);
    void onNoMirror(CCObject*);
    void onObjectLimitHack(CCObject*);

    cocos2d::CCSprite* m_background = nullptr;
    bool m_fromRope = false;

private:
    void addCheckbox(float xIdx, float yIdx, const char* text, bool toggle, SEL_MenuHandler callback, CCMenu* menu, CCObject* target);

};