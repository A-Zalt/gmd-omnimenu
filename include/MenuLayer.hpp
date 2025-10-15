#pragma once

#include <FLAlertLayer.hpp>

class MenuLayer : public cocos2d::CCLayer, public FLAlertLayerProtocol {
public:
    static MenuLayer* node();
    static cocos2d::CCScene* scene() ;

    void onMenuInfo();

    virtual bool init();
    virtual void keyBackClicked();
    virtual void googlePlaySignedIn();
    virtual void FLAlert_Clicked(FLAlertLayer* alert, bool btn2);
};
