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
#ifndef USE_MINIAUDIO
#include "hooks/SimpleAudioEngine.cpp"
#endif
#include "hooks/CCControlColourPicker.cpp"
#include "hooks/LevelBrowserLayer.cpp"
#include "hooks/GJGarageLayer.cpp"
#include "hooks/EndPortalObject.cpp"
#if GAME_VERSION >= GV_1_3
#include "hooks/LeaderboardsLayer.cpp"
#endif

#if GDPS == GDPS_NEOPOINTFOUR
#include "hooks/Neopointfour.cpp"
#elif GDPS == GDPS_1_7
#include "hooks/17GDPS.cpp"
#endif

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

#if GDPS == GDPS_NEOPOINTFOUR
    Neopointfour_om();
#elif GDPS == GDPS_1_7
    GDPS17_om();
#endif

#ifndef USE_MINIAUDIO
    SimpleAudioEngine_om();
#endif
}