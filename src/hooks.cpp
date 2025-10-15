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
}