#ifndef __EDITORPAUSELAYER_HPP__
#define __EDITORPAUSELAYER_HPP__

#include <cocos2d.h>

#include "layers_scenes_transitions_nodes/FLAlertLayer.hpp"
#include "layers_scenes_transitions_nodes/LevelEditorLayer.hpp"

using namespace cocos2d;

class CCMenuItemSpriteExtra;

class EditorPauseLayer : public CCBlockLayer, public FLAlertLayerProtocol
{
public:
    CCMenuItemSpriteExtra *m_pAudioOnBtn;
    CCMenuItemSpriteExtra *m_pAudioOffBtn;
    CCMenuItemSpriteExtra *m_pBpmOnBtn;
    CCMenuItemSpriteExtra *m_pBpmOffBtn;
    LevelEditorLayer *m_pEditorLayer;

    bool init(LevelEditorLayer*);
    
    virtual ~EditorPauseLayer();
    virtual void customSetup();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
};

#endif
