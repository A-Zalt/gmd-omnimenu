#ifndef __FLALERTLAYER_HPP__
#define __FLALERTLAYER_HPP__

#include <cocos2d.h>

using namespace cocos2d;

class FLAlertLayer;
class FLAlertLayerProtocol
{
public:
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
};

class FLAlertLayer : public CCLayerColor
{
public:
    CCMenu *m_pMenu;
    void* unk;
    void* unk2;
    FLAlertLayerProtocol *m_pPParent;
    CCNode *m_pTargetScene;
    bool m_bReverseKeyBack;
    CCLayer *m_pInternalLayer;

    static FLAlertLayer* create(FLAlertLayerProtocol*, const char* title, const char* desc, const char* bnt1, const char* bnt2, float width);
    // virtual ~FLAlertLayer();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    virtual bool show();
    virtual void keyBackClicked();

    virtual FLAlertLayerProtocol* getPParent() const;
    virtual void setPParent(FLAlertLayerProtocol*);

    virtual CCNode* getTargetScene() const;
    virtual void setTargetScene(CCNode*);

    virtual bool getReverseKeyBack() const;
    virtual void setReverseKeyBack(bool);
    virtual CCLayer* getInternalLayer() const;
};

#endif
