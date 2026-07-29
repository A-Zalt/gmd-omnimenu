#include "hooks/PlayLayer.cpp"
#include "hooks/MenuLayer.cpp"
#include "hooks/GameManager.cpp"
#include "hooks/CCTextInputNode.cpp"
#include "hooks/PauseLayer.cpp"
#include "hooks/EditLevelLayer.cpp"
#include "hooks/EditorUI.cpp"
#include "hooks/CCTransitionFade.cpp"
#include "hooks/LevelInfoLayer.cpp"
#include "hooks/LevelEditorLayer.cpp"
#include "hooks/UILayer.cpp"
#include "hooks/CCString.cpp"
#include "hooks/EditorPauseLayer.cpp"
#include "hooks/CCNode.cpp"
#include "hooks/LevelSelectLayer.cpp"
#include "hooks/LevelCell.cpp"
#if GAME_VERSION > GV_1_0
#include "hooks/CommentCell.cpp"
#endif
#include "hooks/GJGameLevel.cpp"
#include "hooks/GameStatsManager.cpp"
#include "hooks/PlayerObject.cpp"
#include "hooks/CCParticleSystemQuad.cpp"
#include "hooks/GameObject.cpp"
#include "hooks/MyLevelsLayer.cpp"
#if GAME_VERSION < GV_1_5
#include "hooks/CCLabelBMFont.cpp"
#endif
#if GAME_VERSION < GV_1_7
#include "hooks/CCLabelTTF.cpp"
#endif
#include "hooks/CCScheduler.cpp"
#include "hooks/GameSoundManager.cpp"
#include "hooks/CCControlColourPicker.cpp"
#include "hooks/LevelBrowserLayer.cpp"
#include "hooks/GJGarageLayer.cpp"
#include "hooks/EndPortalObject.cpp"
#if GAME_VERSION >= GV_1_3
#include "hooks/LeaderboardsLayer.cpp"
#endif
#include "hooks/EndLevelLayer.cpp"
#include "hooks/DrawGridLayer.cpp"
#if GAME_VERSION >= GV_1_7
#include "hooks/CCMenuItem.cpp"
#endif
// #include "hooks/CCMenuItemSpriteExtra.cpp"

#if GDPS == GDPS_NEOPOINTFOUR
#include "hooks/Neopointfour/LevelTools.cpp"
#include "hooks/Neopointfour/GJGameLevel.cpp"
#include "hooks/Neopointfour/InfoLayer.cpp"
#include "hooks/Neopointfour/GJComment.cpp"
#include "hooks/Neopointfour/LevelSearchLayer.cpp"
#include "hooks/Neopointfour/CCHttpClient.cpp"
#include "hooks/Neopointfour/GameLevelManager.cpp"
#include "hooks/Neopointfour/CCDictionary.cpp"
#include "hooks/Neopointfour/LoadingLayer.cpp"
#elif GDPS == GDPS_1_7
#include "hooks/17GDPS.cpp"
#endif
#include "hooks/AppDelegate.cpp"
#include "hooks/SimpleAudioEngine.cpp"

void initialize_hooks() {
    PlayLayer_om();
    MenuLayer_om();
    GameManager_om();
    CCTextInputNode_om();
    PauseLayer_om();
    EditLevelLayer_om();
    EditorUI_om();
    CCTransitionFade_om();
    LevelInfoLayer_om();
    LevelEditorLayer_om();
    UILayer_om();
    CCString_om();
    EditorPauseLayer_om();
    CCNode_om();
    LevelSelectLayer_om();
    LevelCell_om();
#if GAME_VERSION >= GV_1_1
    CommentCell_om();
#endif
    GJGameLevel_om();
    GameStatsManager_om();
    PlayerObject_om();
    CCParticleSystemQuad_om();
    GameObject_om();
    MyLevelsLayer_om();
#if GAME_VERSION < GV_1_5
    CCLabelBMFont_om();
#endif
#if GAME_VERSION < GV_1_7
    CCLabelTTF_om();
#endif
    CCScheduler_om();
    GameSoundManager_om();
    CCControlColourPicker_om();
    LevelBrowserLayer_om();
#if GAME_VERSION >= GV_1_3
    GJGarageLayer_om();
    LeaderboardsLayer_om();
#endif
    EndPortalObject_om();
    EndLevelLayer_om();
    AppDelegate_om();

#if GDPS == GDPS_NEOPOINTFOUR
    NP4_GJGameLevel_om();
    NP4_LevelTools_om();
    NP4_InfoLayer_om();
    NP4_GJComment_om();
    NP4_LevelSearchLayer_om();
    NP4_CCHttpClient_om();
    NP4_GameLevelManager_om();
    NP4_CCDictionary_om();
    NP4_LoadingLayer_om();
#elif GDPS == GDPS_1_7
    GDPS17_om();
#endif
    SimpleAudioEngine_om();
    
    DrawGridLayer_om();

#if GAME_VERSION >= GV_1_7
    CCMenuItem_om();
#endif
    // CCMenuItemSpriteExtra_om();
}