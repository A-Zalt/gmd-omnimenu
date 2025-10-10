#ifndef __CCBLOCKLAYER_H__
#define __CCBLOCKLAYER_H__

#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "touch_dispatcher/CCTouch.h"

USING_NS_CC;

class CCBlockLayer : public CCLayerColor {
public:
    bool m_bRemoveOnExit;
    
    virtual ~CCBlockLayer();
    virtual void draw();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    virtual void customSetup();
    virtual void enterLayer();
    virtual void exitLayer();
    virtual void showLayer();
    virtual void hideLayer();
    virtual void layerVisible();
    virtual void layerHidden();
    virtual void enterAnimFinished();
    virtual void disableUI();
    virtual void enableUI();
    virtual void keyBackClicked();
    virtual bool getRemoveOnEdit();
    virtual void setRemoveOnExit();
};

#endif
