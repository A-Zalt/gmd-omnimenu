#pragma once

#include <cocos2d.h>
#include "Module.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "constants.hpp"
#include "../version/VersionUtils.hpp"
#include <sys/stat.h>
#include <errno.h>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "GJGameLevel.hpp"
#include "MyLevelsLayer.hpp"
#include "LevelBrowserLayer.hpp"
#include "PauseLayer.hpp"
#include "SpeedhackInput.hpp"
#include "RespawnTimeInput.hpp"

enum class CheatIndicatorColor {
    Green,
    Yellow,
    Orange,
    Red
};



enum ModuleID {
    _100_KB_FIX,
    _16K_FIX,
    ACCURATE_PERCENTAGE,
    AUTO_SAFE_MODE,
    CHARACTER_FILTER_BYPASS,
    CHEAT_INDICATOR,
    COMMENT_IDS,
    COMMENT_OFFSET_FIX,
    CONFIRM_EXIT,
    COPY_PASTE,
    CUSTOM_RESPAWN_TIME,
    DELETE_SELECTED,
    DELETE_START_POS,
    DEMONS_IN_GARAGE,
    OBJECT_COUNTER, // Editor Pause Info
    EXTRA_EDIT_BUTTONS,
    FAST_MENU,
    FONT_OFFSET_FIX,
    FORCE_VISIBILITY,
    // FPS_BYPASS,
#if GAME_VERSION >= GV_1_7
    LEVEL_COPYING,
#endif
    FREE_BUILD,
    FREE_SCROLL,
    GDSHARE,
    GOLDEN_BEST,
#if GAME_VERSION < GV_1_7
    LEVEL_COPYING,
#endif
    LEVEL_EDIT,
    HIDE_ATTEMPTS,
    HIDE_CHECKPOINT_BUTTONS,
    HIDE_PAUSE_MENU,
    INSTANT_COMPLETE,
    INPUT_BUG_FIX,
    JUMP_HACK,

    LABEL_ATTEMPT_COUNT_SESSION,
    LABEL_ATTEMPT_COUNT_TOTAL,
    LABEL_BEST_RUN,
    LABEL_CLICKS,
    LABEL_CLOCK,
    LABEL_FPS,
    LABEL_FRAMES,
    LABEL_JUMPS,
    LABEL_NOCLIP_ACCURACY,
    LABEL_NOCLIP_DEATHS,
    LABEL_PCOMMAND,
    LABEL_PLAYER_POSITION,
    LABEL_PLAYER_ROTATION,
    LABEL_SPEEDHACK,
    LABEL_TIME_SPENT,

    LEVEL_IDS_IN_SEARCH,
    MUSIC_BUG_FIX,
    NOCLIP,
    NOCLIP_TINT_ON_DEATH,
    NO_DEATH_EFFECT,
    NO_GLOW,
    NO_MIRROR,
    NO_PULSE,
    NO_ROTATION,
    NO_SHAKE,
    NO_SHIP_TINT,
    OBJ_COLOR_FIX,
    OBJECT_LIMIT_BYPASS,
    PAGE_REFRESH,

    PARTICLE_BACKGROUND,
    PARTICLE_BURST,
    PARTICLE_DEATH_EFFECT,
    PARTICLE_DESTRUCTIBLE_BLOCKS,
    PARTICLE_END_WALL,
    PARTICLE_FIREWORKS,
    PARTICLE_GROUND,
    PARTICLE_LANDING,
    PARTICLE_LEVEL_COMPLETION,
    PARTICLE_OBJECTS,
    PARTICLE_SECRET_COINS,
    PARTICLE_SHIP_FIRE,
    PARTICLE_SHIP_GROUND,
    PARTICLE_SHIP_LIFT,
    PARTICLE_SPEED_PORTALS,

    PCOMMAND,
    PIG_SPOOFING,
    PRACTICE_MUSIC_HACK,
    RGB_COLOR_INPUTS,
    ROTATION_BUG_FIX,
    SAFE_MODE,
    SHOW_DIFFICULTY,
    SHOW_PERCENTAGE,
    SHOW_PERCENTAGE_DECIMAL,
    SHOW_OBJECT_INFO,
    SHOW_RESTART_BUTTON,
    SPEEDHACK,
    START_POS_SWITCHER,
    SWEAR_FILTER_BYPASS,
    TEXT_LENGTH_BYPASS,
    NO_TRAIL, // Trail Always Off
    TRAIL_ALWAYS_ON,
    UNLISTED_OBJECTS,
    UNLOCK_CLUBSTEP,
    UNLOCK_ICONS,
    VERIFY_BYPASS,
    VIEW_LEVEL_STATS,
    ZOOM_BYPASS,
    MODULE_COUNT
};

class HaxManager {

public:
    static HaxManager& sharedState() {
        static HaxManager instance;
        return instance;
    }
    // std::map<std::string, Module*> modules;
    std::array<Module, MODULE_COUNT> modules;
    cocos2d::CCLabelBMFont* cheatIndicatorLabel;
    cocos2d::CCLabelBMFont* percentageLabel;
    ModuleCategory lastCategory;
    CCMenu* pMenu;
    CCMenuItemSpriteExtra* pButton1;
    CCMenuItemSpriteExtra* pButton2;
    CCMenuItemSpriteExtra* pButton3;
    CCMenuItemSpriteExtra* pButton4;
    CCMenuItemSpriteExtra* pButton5;
    CCMenuItemSpriteExtra* pButton6;
    CCLabelBMFont* uiLabel;
    bool hasCheated;
    bool instantComped;
    bool quitPlayLayer;
    int pSpeedModified;
    int pGravityModified;
    int pYStartModified;
    // jobject activity;
    GJGameLevel* gdShareLevel;
    MyLevelsLayer* myLevelsLayer;
    LevelBrowserLayer* levelBrowserLayer;
    float bestRunStart;
    float bestRunEnd;
    int frames;
    int fps;
    float fpsCounter;
    int clicks;
    int deaths;
    int frameCount;
    int lastDeadFrame;
    CCNode* spSwitcherParent;
    int startPosIndex;
    CCLabelBMFont* switcherLabel;
    float startPercent;
    bool checkpointsInNormalMode;
    CCLayerColor* noclipTint;
    int deadFrames;
    float noclipAccuracy;
    bool completed;
    int gdShareMessageID;
    int32_t gdShareData;
    CCLabelBMFont* editorObjectInfo;
    float ntOpacity;
    CCArray* startPositions;

#if GAME_VERSION > GV_1_4
    bool blockVerify;
#endif
#if GDPS == GDPS_NEOPOINTFOUR
    std::map<GJGameLevel*, int> featureTypeMap;
#endif
    float timeScale;
    bool dead;
    // int fpsBypass;
    // double originalAnimInterval;
    bool hasInitialized;
    PauseLayer* pauseLayer;
    SpeedhackInput* speedInputWidget;
    CCMenu* eyeMenu;
    // bool updatedMusic;
    bool hasTouchedTheEye;
    float respawnTime;
    RespawnTimeInput* respawnInput;
    bool customRespawn;

    bool getModuleEnabled(ModuleID id) {
        return modules[id].enabled;
    }
    void setModuleEnabled(ModuleID id, bool enabled) {
        modules[id].enabled = enabled;
        modules[id].onToggle(enabled);
    }
    bool getLeftLILButtons() {
        return getModuleEnabled(ModuleID::LEVEL_COPYING) || getModuleEnabled(ModuleID::GDSHARE);
    }

    CheatIndicatorColor getCheatIndicatorColor() {
        if (getModuleEnabled(ModuleID::NOCLIP) || 
        getModuleEnabled(ModuleID::INSTANT_COMPLETE) || 
        checkpointsInNormalMode || 
        getModuleEnabled(ModuleID::JUMP_HACK) || 
        getModuleEnabled(ModuleID::NO_MIRROR) || 
        pSpeedModified != 0 || pGravityModified != 0 || pYStartModified != 0 ||
        hasTouchedTheEye || 
        (getModuleEnabled(ModuleID::SPEEDHACK)) && timeScale != 1) return CheatIndicatorColor::Red;
        if (hasCheated) return CheatIndicatorColor::Orange;
#ifndef FORCE_AUTO_SAFE_MODE
        if (getModuleEnabled(ModuleID::LEVEL_EDIT)) return CheatIndicatorColor::Yellow;
#endif 
        return CheatIndicatorColor::Green;
    }

    bool getShowLabel() {
        return (
            getModuleEnabled(ModuleID::LABEL_ATTEMPT_COUNT_SESSION) ||
            getModuleEnabled(ModuleID::LABEL_ATTEMPT_COUNT_TOTAL) ||
            getModuleEnabled(ModuleID::LABEL_BEST_RUN) ||
            getModuleEnabled(ModuleID::LABEL_CLICKS) ||
            getModuleEnabled(ModuleID::LABEL_CLOCK) ||
            getModuleEnabled(ModuleID::LABEL_FPS) ||
            getModuleEnabled(ModuleID::LABEL_FRAMES) ||
            getModuleEnabled(ModuleID::LABEL_JUMPS) ||
            (getModuleEnabled(ModuleID::NOCLIP) && 
            (getModuleEnabled(ModuleID::LABEL_NOCLIP_ACCURACY) ||
            getModuleEnabled(ModuleID::LABEL_NOCLIP_DEATHS))) ||
            getModuleEnabled(ModuleID::LABEL_PCOMMAND) ||
            getModuleEnabled(ModuleID::LABEL_PLAYER_POSITION) ||
            getModuleEnabled(ModuleID::LABEL_PLAYER_ROTATION) ||
            getModuleEnabled(ModuleID::LABEL_TIME_SPENT) ||
            (getModuleEnabled(ModuleID::SPEEDHACK) && getModuleEnabled(ModuleID::LABEL_SPEEDHACK))
        );
    }

    void setCheating(bool val) {
        hasCheated = val;
    }
#if GAME_VERSION > GV_1_4
    void blockVerifySet(bool val) {
        setBlockVerify(val);
        blockVerify = val;
    }
#endif

    void loadSettingsFromFile() {
        // originalAnimInterval = CCDirector::sharedDirector()->getAnimationInterval();
        FILE* fp = fopen(MENU_SETTINGS_PATH MENU_SETTINGS, "rb");
        if (!fp) {
            cocos2d::CCLog("unable to open settings file for reading");
            makeDirectory();
            return;
        }
        char readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

        rapidjson::Document doc;
        doc.ParseStream(is);

        fclose(fp);

        if (doc.HasMember("pref_timeScale") && doc["pref_timeScale"].IsFloat()) {
            timeScale = doc["pref_timeScale"].GetFloat();
        }
        if (doc.HasMember("pref_respawnTime") && doc["pref_respawnTime"].IsFloat()) {
            respawnTime = doc["pref_respawnTime"].GetFloat();
        }
        for (Module& record : modules) {
            if (!record.exists) continue;
            if (doc.HasMember(record.id) && doc[record.id].IsBool()) {
                record.setEnabled(doc[record.id].GetBool());
            }
        }
        hasInitialized = true;
    }
    void makeDirectory() {
        int status = mkdir(MENU_SETTINGS_PATH, static_cast<mode_t>(0755));
        if (status != 0 && errno != EEXIST) {
            cocos2d::CCLog("could not make dir: %i", errno);
        }
    }
#if GAME_VERSION > GV_1_4
    void update(float dt) {
        if (isSafeMode()) {
            if (!blockVerify) blockVerifySet(true);
        } else if (blockVerify) {
            blockVerifySet(false);
        }
    }
#endif
    void saveSettingsToFile() {
        makeDirectory();
        FILE* fp = fopen(MENU_SETTINGS_PATH MENU_SETTINGS, "wb");

        if (!fp) {
            cocos2d::CCLog("unable to open settings file for writing");
            return;
        } else {
            std::map<std::string, Module*>::iterator it;
            rapidjson::Document document;
            document.SetObject();
            rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
            int key = 0;
            for (const auto& record : modules) {
                if (!record.exists) {
                    key++;
                    continue;
                }
                rapidjson::Value jsonKey(record.id, static_cast<rapidjson::SizeType>(strlen(record.id)), allocator);
                document.AddMember(jsonKey, getModuleEnabled(static_cast<ModuleID>(key)), allocator);
                key++;
            }

            rapidjson::Value jk("pref_timeScale", static_cast<rapidjson::SizeType>(strlen("pref_timeScale")), allocator);
            document.AddMember(jk, timeScale, allocator);
            rapidjson::Value jk2("pref_respawnTime", static_cast<rapidjson::SizeType>(strlen("pref_respawnTime")), allocator);
            document.AddMember(jk2, respawnTime, allocator);

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            document.Accept(writer);
            const char* output = buffer.GetString();

            fprintf(fp, "%s", output);
            fclose(fp);
        }
    }
    bool isSafeMode() {
        if (getModuleEnabled(ModuleID::SAFE_MODE)) return true;
#ifndef FORCE_AUTO_SAFE_MODE
        if (!getModuleEnabled(ModuleID::AUTO_SAFE_MODE)) return false;
#endif
        CheatIndicatorColor color = getCheatIndicatorColor();
        if (color == CheatIndicatorColor::Red || color == CheatIndicatorColor::Orange)
            return true;

        return false;
    }

    CCArray* getStartPositions() {
        if (startPositions) return startPositions;
        if (!getPlayLayer()) return nullptr;
        startPositions = getStartPositions_();
        startPositions->retain();
        return startPositions;
    }

    /* 
    Resets nearly all variables in the HaxManager. Called upon the creation of HaxManager, and upon the destruction of UILayer. 
    */
    void resetValues() {
        hasCheated = false;
        cheatIndicatorLabel = nullptr;
        percentageLabel = nullptr;
        pButton1 = nullptr;
        pButton2 = nullptr;
        pButton3 = nullptr;
        pButton4 = nullptr;
        pButton5 = nullptr;
        pButton6 = nullptr;
        uiLabel = nullptr;
        pMenu = nullptr;
        spSwitcherParent = nullptr;
        pSpeedModified = 0;
        pGravityModified = 0;
        pYStartModified = 0;
        bestRunStart = 0;
        bestRunEnd = 0;
        startPercent = 0;
        clicks = 0;
        deaths = 0;
        frameCount = 0;
        lastDeadFrame = -1;
        startPosIndex = -1;
        switcherLabel = nullptr;
        checkpointsInNormalMode = false;
        noclipTint = nullptr;
        deadFrames = 0;
        noclipAccuracy = 100;
        completed = false;
        ntOpacity = 0;
        startPositions = nullptr;
        dead = false;
        pauseLayer = nullptr;
        speedInputWidget = nullptr;
        // updatedMusic = false;
        eyeMenu = nullptr;
        hasTouchedTheEye = false;
        respawnInput = nullptr;
        customRespawn = false;
    }

private:
    HaxManager() {
        modules[ModuleID::CONFIRM_EXIT] = Module(
            "confirm_exit",
            "Confirm Exit", 
            "Gives a confirmation popup when exiting the level.", 
            false, ModuleCategory::Player, [](bool _){});
        modules[ModuleID::CUSTOM_RESPAWN_TIME] = Module(
            "custom_respawn_time",
            "Custom Respawn Time", 
            "Allows you to modify the respawn time after a death.", 
            false, ModuleCategory::Player, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                if (hax.respawnInput) hax.respawnInput->setVisible(_);
            });
        modules[ModuleID::INSTANT_COMPLETE] = Module(
            "instant_complete",
            "Instant Complete", 
            "Completes the level as soon as it is entered.", 
            false, ModuleCategory::Player, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                if (_) hax.setCheating(true);
            });
        modules[ModuleID::JUMP_HACK] = Module(
            "jump_hack",
            "Jump Hack", 
            "Allows the player to jump in the air indefinitely.", 
            false, ModuleCategory::Player, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                if (_) hax.setCheating(true);
            });
        modules[ModuleID::MUSIC_BUG_FIX] = Module(
            "music_bug_fix",
            "Music Bug Fix", 
            "Makes music seeking work properly more often.", 
            true, ModuleCategory::Player, [](bool _){});
        modules[ModuleID::NOCLIP] = Module(
            "noclip",
            "NoClip", 
            "Prevents the player from dying.", 
            false, ModuleCategory::Player, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                if (_) hax.setCheating(true);
            });
        modules[ModuleID::NO_MIRROR] = Module(
            "no_mirror",
            "No Mirror", 
            "Disables mirror portals.", 
            false, ModuleCategory::Player, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                if (_) hax.setCheating(true);
            });
        modules[ModuleID::PCOMMAND] = Module(
            "pcommand",
            "pCommand", 
            "Re-enables the unused \"pCommand\" functionality, which allows you to alter your speed, gravity and jump height. (NOTE: Some creative liberties had to be taken with the controls. This module is not entirely accurate to how pCommand actually worked.)", 
            false, ModuleCategory::Player, [](bool _){});
#if GAME_VERSION < GV_1_2
        modules[ModuleID::ROTATION_BUG_FIX] = Module(
            "rotation_bug_fix",
            "Rotation Bug Fix", 
            "Makes it so hitboxes rotate properly along with the object. (module by akqanile/Adelfa)\nNOTE: toggling this mid-attempt counts as cheating.", 
            false, ModuleCategory::Player, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                hax.setCheating(true);
            });
#endif
        modules[ModuleID::PRACTICE_MUSIC_HACK] = Module(
            "practice_music",
            "Practice Music Hack", 
            "Plays the normal level music in practice mode.", 
            false, ModuleCategory::Player, [](bool _){});
        modules[ModuleID::SHOW_RESTART_BUTTON] = Module(
            "show_restart_button",
            "Show Restart Button", 
            "Adds the restart button to the pause menu of all levels", 
            false, ModuleCategory::Player, [](bool _){
                setRestartButton(_);
            });
        modules[ModuleID::SPEEDHACK] = Module(
            "speedhack",
            "Speedhack", 
            "Lets you change the speed the game runs at.", 
            false, ModuleCategory::Player, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                if (_) {
                    if (hax.pauseLayer) {
                        if (!hax.speedInputWidget) hax.pauseLayer->createSpeedhack();
                        else {
                            hax.speedInputWidget->setVisible(true);
                            hax.speedInputWidget->setScale(1);
                        }
                    }
                    CCDirector::sharedDirector()->getScheduler()->setTimeScale(hax.timeScale);
                    if (hax.timeScale != 1) hax.setCheating(true);
                } else {
                    if (hax.pauseLayer && hax.speedInputWidget) {
                        hax.speedInputWidget->setVisible(false);
                        hax.speedInputWidget->setScale(0);
                    }
                    CCDirector::sharedDirector()->getScheduler()->setTimeScale(1);
                }
            });
        modules[ModuleID::START_POS_SWITCHER] = Module(
            "start_pos_switcher",
            "Start Pos Switcher", 
            "Adds the ability to switch between start positions on the fly.", 
            false, ModuleCategory::Player, [](bool _){});


#if GAME_VERSION >= GV_1_7
        modules[ModuleID::ACCURATE_PERCENTAGE] = Module(
            "accurate_percentage",
            "Golden Best", 
            "Makes the Show Percentage label account for speed portals.", 
            false, ModuleCategory::Visual, [](bool _){});
#endif
        modules[ModuleID::GOLDEN_BEST] = Module(
            "golden_best",
            "Golden Best", 
            "Makes the Show Percentage label gold if you are past your record on the level.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::HIDE_ATTEMPTS] = Module(
            "hide_attempts",
            "Hide Attempts", 
            "Hides the attempts label while playing.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::HIDE_CHECKPOINT_BUTTONS] = Module(
            "hide_checkpoint_buttons",
            "Hide Checkpoint Buttons", 
            "Makes the practice checkpoint buttons significantly lower opacity.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::HIDE_PAUSE_MENU] = Module(
            "hide_pause_menu",
            "Hide Pause Menu", 
            "Adds a button to the pause menu that hides it, letting you see the level better.", 
            false, ModuleCategory::Visual, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                if (hax.pauseLayer) {
                    if (_) {
                        if (!hax.eyeMenu) hax.pauseLayer->createEye();
                        else hax.eyeMenu->setVisible(true);
                    } else {
                        if (hax.eyeMenu) hax.eyeMenu->setVisible(false);
                    }
                }
            });
        modules[ModuleID::NOCLIP_TINT_ON_DEATH] = Module(
            "noclip_tint_on_death",
            "NoClip Tint On Death", 
            "Tints the screen red when the player dies with NoClip enabled.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::NO_DEATH_EFFECT] = Module(
            "no_death_effect",
            "No Death Effect", 
            "Disables the death effect, leaving the player visible at the position it died.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::NO_GLOW] = Module(
            "no_glow",
            "No Glow", 
            "Disables object glow. (Note: Will only apply after re-entering the level)", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::NO_PULSE] = Module(
            "no_pulse",
            "No Pulse", 
            "Disables object pulses.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::NO_ROTATION] = Module(
            "no_rotation",
            "No Rotation", 
            "Prevents the player from rotating.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::NO_SHAKE] = Module(
            "no_shake",
            "No Shake", 
            "Disables camera shake when completing a level.", 
            false, ModuleCategory::Visual, [](bool _){});
#if GAME_VERSION < GV_1_4
        modules[ModuleID::NO_SHIP_TINT] = Module(
            "no_ship_tint",
            "No Ship Tint", 
            "Removes the dark tint of the player's ship.", 
            false, ModuleCategory::Visual, [](bool _){
                setNoShipTint(_);
            });
#endif
#if GAME_VERSION == GV_1_4
        modules[ModuleID::OBJ_COLOR_FIX] = Module(
            "obj_color_fix",
            "Obj Color Fix", 
            "Fixes the Object color not resetting to white after death.", 
            false, ModuleCategory::Visual, [](bool _){});
#endif
        modules[ModuleID::SHOW_PERCENTAGE] = Module(
            "show_percentage",
            "Show Percentage", 
            "Displays the percentage the player is currently at.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::SHOW_PERCENTAGE_DECIMAL] = Module(
            "show_percentage_decimal",
            "Decimal Percentage", 
            "Puts 3 decimal places after the percentage if you have Show Percentage enabled and in the Best Run label.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::NO_TRAIL] = Module(
            "no_trail",
            "Trail Always Off", 
            "Disables the player's trail at all times.", 
            false, ModuleCategory::Visual, [](bool _){});
        modules[ModuleID::TRAIL_ALWAYS_ON] = Module(
            "trail_always_on",
            "Trail Always On", 
            "Makes the player's trail visible at all times.", 
            false, ModuleCategory::Visual, [](bool _){});



        // 16k fix code: https://github.com/cierra-kb/legacy-starry-sky/blob/main/src/modules/editor.cpp
#if GAME_VERSION < GV_1_8
        modules[ModuleID::_16K_FIX] = Module(
            "16k_fix",
            "16K Fix (Read Desc)", 
            "Fixes a bug where only 16,384 objects can render in the editor by culling the objects. (module by akqanile/Adelfa)\nNOTE: this can be potentially incompatible with vanilla layering, and makes the editor way laggier on dense levels.", 
            false, ModuleCategory::Editor, [](bool _){});
#endif
// #if GAME_VERSION < GV_1_6
//         modules.insert(std::pair<std::string, Module*>("back_button_pause", new Module(
//                 "Back Button Pause", 
//                 "Opens the editor pause menu when pressing the back button on the keypad.", 
//                 false, ModuleCategory::Editor, [](bool _){})));
// #endif
#if GAME_VERSION < GV_1_5
        modules[ModuleID::COPY_PASTE] = Module(
            "copy_paste",
            "Copy & Paste", 
            "Adds a button that duplicates the selected objects.", 
            false, ModuleCategory::Editor, [](bool _){});
#endif
        modules[ModuleID::DELETE_SELECTED] = Module(
            "delete_selected",
#if GAME_VERSION < GV_1_5
            "Delete Selected", 
            "Adds a button that removes the selected objects.", 
#else
            "Delete Selected (2.0)",
            "Moves Delete Selected next to the Undo/Redo buttons.",
#endif
            false, ModuleCategory::Editor, [](bool _){});
#if GAME_VERSION < GV_1_6
        modules[ModuleID::DELETE_START_POS] = Module(
            "delete_start_pos",
            "Delete Start Pos", 
            "Adds a button that removes all start positions in the level.", 
            false, ModuleCategory::Editor, [](bool _){});
#endif
        modules[ModuleID::OBJECT_COUNTER] = Module(
            "object_counter",
            "Editor Pause Info", 
#if GAME_VERSION < GV_1_6
            "Displays the object count and length of the level in the editor pause menu.", 
#else
            "Displays the length of the level in the editor pause menu.", 
#endif
            false, ModuleCategory::Editor, [](bool _){});
        modules[ModuleID::EXTRA_EDIT_BUTTONS] = Module(
            "extra_edit_buttons",
            "Extra Edit Buttons", 
            "Adds extra buttons to the edit tab that allow you to control objects more precisely.", 
            false, ModuleCategory::Editor, [](bool _){});
        modules[ModuleID::FREE_BUILD] = Module(
            "free_build",
            "Free Build", 
            "Removes the constraints for placing and moving objects in the editor.", 
            false, ModuleCategory::Editor, [](bool _){
                setFreeBuild(_);
            });
        modules[ModuleID::FREE_SCROLL] = Module(
            "free_scroll",
            "Free Scroll", 
            "Removes the constraints for camera position in the editor.", 
            false, ModuleCategory::Editor, [](bool _){});
        modules[ModuleID::LEVEL_COPYING] = Module(
            "level_copy",
#if GAME_VERSION < GV_1_7
            "Level Copying", 
            "Adds a button to copy any level.",
#else
            "Free Copy", 
            "Allows you to freely copy any level.",
#endif
            false, ModuleCategory::Editor, [](bool _){});
#ifndef FORCE_AUTO_SAFE_MODE
        modules[ModuleID::LEVEL_EDIT] = Module(
            "level_edit",
            "Level Edit", 
            "Allows you to locally edit any level.", 
            false, ModuleCategory::Editor, [](bool _){
                setEditButton(_);
            });
#endif
        modules[ModuleID::OBJECT_LIMIT_BYPASS] = Module(
            "object_hack",
            "Object Limit Bypass", 
#if GAME_VERSION < GV_1_5
            "Sets the object limit to the 32-bit integer limit, or to 16,384 if 16k Fix is not enabled.",
#else
            "Removes the object limit.",
#endif
            false, ModuleCategory::Editor, [](bool _){
                if (_) {
                    HaxManager& hax = HaxManager::sharedState();
                    if (hax.getModuleEnabled(ModuleID::_16K_FIX))
                        setObjectLimit(2147483646);
                    else
                        setObjectLimit(INCREASED_OBJECT_LIMIT - 1);
                } else
                    setObjectLimit(OBJECT_LIMIT);
            });
        modules[ModuleID::RGB_COLOR_INPUTS] = Module(
            "rgb_color_inputs",
            "RGB Color Inputs", 
            "Allows you to directly input the RGB values in color selection menus.", 
            false, ModuleCategory::Editor, [](bool _){});
        modules[ModuleID::SHOW_OBJECT_INFO] = Module(
            "show_object_info",
            "Show Object Info", 
            "Displays info about selected object(s).", 
            false, ModuleCategory::Editor, [](bool _){});
        modules[ModuleID::UNLISTED_OBJECTS] = Module(
            "unlisted_objects",
            "Unlisted Objects", 
            "Adds unlisted objects to the editor, allowing them to be used in levels.", 
            false, ModuleCategory::Editor, [](bool _){});
#ifndef FORCE_AUTO_SAFE_MODE
        modules[ModuleID::VERIFY_BYPASS] = Module(
            "verify_bypass",
            "Verify Bypass", 
            "Allows you to upload any level without verifying it.", 
            false, ModuleCategory::Editor, [](bool _){});
#endif
        modules[ModuleID::ZOOM_BYPASS] = Module(
            "zoom_bypass",
            "Zoom Bypass", 
            "Removes editor zoom restrictions.", 
            false, ModuleCategory::Editor, [](bool _){
                setZoomBypass(_);
            });



        modules[ModuleID::CHARACTER_FILTER_BYPASS] = Module(
            "char_filter_bypass",
            "Character Filter Bypass", 
            "Allows you to use any character in any textbox.", 
            false, ModuleCategory::Bypass, [](bool _){});
        modules[ModuleID::SWEAR_FILTER_BYPASS] = Module(
            "swear_filter_bypass",
            "Swear Filter Bypass", 
            "Allows you to use profanities in textboxes.", 
            false, ModuleCategory::Bypass, [](bool _){
#if GAME_VERSION > GV_1_4
                setProfanityBypass(_);
#endif                       
            });
        modules[ModuleID::TEXT_LENGTH_BYPASS] = Module(
            "text_length_bypass",
            "Text Length Bypass", 
            "Allows you to write an unlimited amount of characters in any textbox.", 
            false, ModuleCategory::Bypass, [](bool _){
#if GAME_VERSION > GV_1_4
                setCharLimitBypass(_);
#endif
            });
#if GAME_VERSION >= GV_1_6
        modules[ModuleID::UNLOCK_CLUBSTEP] = Module(
            "unlock_clubstep",
            "Unlock Clubstep", 
            "Allows you to play Clubstep without getting 20 secret coins.", 
            false, ModuleCategory::Bypass, [](bool _){
                setClubstepBypass(_);
            });
#endif
        modules[ModuleID::UNLOCK_ICONS] = Module(
            "unlock_icons",
            "Unlock Icons", 
            "Unlocks all icons and colors in the Icon Kit.", 
            false, ModuleCategory::Bypass, [](bool _){
#if GAME_VERSION >= GV_1_4
                setIconHack(_);
#endif
            });



#if GAME_VERSION > GV_1_0
        modules[ModuleID::COMMENT_IDS] = Module(
            "comment_ids",
            "Comment IDs", 
            "Displays comment IDs in comment cells.", 
            false, ModuleCategory::Informational, [](bool _){});
#endif
#if GAME_VERSION > GV_1_2
        modules[ModuleID::DEMONS_IN_GARAGE] = Module(
            "demons_in_garage",
            "Demons in Garage", 
            "Displays your demon count in the icon kit (garage). (module by Hris69)", 
            false, ModuleCategory::Informational, [](bool _){});
#endif
        modules[ModuleID::LEVEL_IDS_IN_SEARCH] = Module(
            "level_ids_in_list",
            "Level IDs in Search", 
            "Displays level IDs in level cells.", 
            false, ModuleCategory::Informational, [](bool _){});
#if GAME_VERSION < GV_1_3
        modules[ModuleID::SHOW_DIFFICULTY] = Module(
            "show_difficulty",
            "Show Difficulty", 
            "Displays the level's difficulty on its page.", 
            false, ModuleCategory::Informational, [](bool _){});
#endif
        modules[ModuleID::VIEW_LEVEL_STATS] = Module(
            "view_level_stats",
#if GAME_VERSION < GV_1_6
            "View Level Stats", 
            "Adds a menu button that displays some information about the level, such as your attempt count on it.",
#else
            "View More Stats", 
            "Displays more stats about online levels in the infobox, and adds the ability to view stats for local levels.",
#endif
            false, ModuleCategory::Informational, [](bool _){});



        modules[ModuleID::_100_KB_FIX] = Module(
            "100_kb_fix",
            "100 KB Fix", 
            "Fixes a poor design choice in Cocos2d where CCStrings always allocate 100 KB, instead allocating a dynamic buffer size. This fixes large levels being cut off on upload (for versions before 1.5), as well as potentially increasing performance. (module by akqanile/Adelfa)", 
            true, ModuleCategory::Universal, [](bool _){});
#ifndef FORCE_AUTO_SAFE_MODE
        modules[ModuleID::AUTO_SAFE_MODE] = Module(
            "auto_safe_mode",
            "Auto Safe Mode",
            "Prevents any progress on any level from being saved if you have cheats enabled.", 
            false, ModuleCategory::Universal, [](bool _){});
#endif
#if GAME_VERSION > GV_1_0 && GAME_VERSION < GV_1_3
        modules[ModuleID::COMMENT_OFFSET_FIX] = Module(
            "comment_offset_fix",
            "Comment Offset Fix",
            "Fixes comment text being offset to the right (or to the left if the device has a small resolution).", 
            true, ModuleCategory::Universal, [](bool _){});
#endif
        // modules.insert(std::pair<std::string, Module*>("eeffoc", new Module(
        //         "Eeffoc",
        //         "!ynnuf si hcihW", 
        //         false, ModuleCategory::Universal, [](bool _){})));
        modules[ModuleID::FAST_MENU] = Module(
            "fast_menu",
            "Fast Menu", 
            "Makes fade transitions instant.", 
            false, ModuleCategory::Universal, [](bool _){});
#if GAME_VERSION < GV_1_5
        modules[ModuleID::FONT_OFFSET_FIX] = Module(
            "font_offset_fix",
            "Font Offset Fix", 
            "Fixes a pre-1.5 bug where text labels get offset further downwards the higher your resolution is.", 
            true, ModuleCategory::Universal, [](bool _){});
#endif
        modules[ModuleID::FORCE_VISIBILITY] = Module(
            "force_visibility",
            "Force Visibility", 
            "Makes all existing nodes visible.", 
            false, ModuleCategory::Universal, [](bool _){
                HaxManager& hax = HaxManager::sharedState();
                if (_) hax.setCheating(true);
            });
        // modules[ModuleID::FPS_BYPASS] = Module(
        //     "fps_bypass",
        //     "FPS Bypass", 
        //     "Lets you increase your FPS past your device's refresh rate.", 
        //     false, ModuleCategory::Universal, [](bool _){
        //         HaxManager& hax = HaxManager::sharedState();
        //         if (_) {
        //             CCDirector::sharedDirector()->setAnimationInterval(1 / hax.fpsBypass);
        //         } else {
        //             CCDirector::sharedDirector()->setAnimationInterval(hax.originalAnimInterval);
        //         }
        //     });
        modules[ModuleID::GDSHARE] = Module(
            "gdshare",
            "GDShare", 
            "Adds buttons to convert a level to a .gmd file and vice-versa. May not work on older Android versions.", 
            false, ModuleCategory::Universal, [](bool _){});
#if GAME_VERSION < GV_1_2
        modules[ModuleID::INPUT_BUG_FIX] = Module(
            "input_bug_fix",
            "Input Bug Fix", 
            "Fixes a pre-1.2 bug where typing more than 1 character doesn't work on certain keyboards.", 
            true, ModuleCategory::Universal, [](bool _){});
#endif
        modules[ModuleID::PAGE_REFRESH] = Module(
            "page_refresh",
            "Page Refresh", 
            "Adds a refresh button to the search page and leaderboards.", 
            false, ModuleCategory::Universal, [](bool _){});
#ifdef PING_SPOOFING
        modules[ModuleID::PIG_SPOOFING] = Module(
            "ping_spoofing",
            "Pig Spoofing", 
            "Self-explanatory.", 
            false, ModuleCategory::Universal, [](bool _){});
#endif
        modules[ModuleID::SAFE_MODE] = Module(
            "safe_mode",
            "Safe Mode",
#ifndef FORCE_AUTO_SAFE_MODE
            "Prevents any progress on any level from being saved.", 
#else
            "Prevents any progress on any level from being saved. (Note: this build has Auto Safe Mode enabled forcibly, which means the effects of this module are automatically applied if you are using any cheats)",
#endif 

            false, ModuleCategory::Universal, [](bool _){});



        modules[ModuleID::CHEAT_INDICATOR] = Module(
            "cheat_indicator",
            "Cheat Indicator", 
            "Adds a dot that indicates whether any unfair hacks are currently enabled.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_ATTEMPT_COUNT_SESSION] = Module(
            "label_attempts_session",
            "Attempt Count (Session)", 
            "Displays the current attempt count.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_ATTEMPT_COUNT_TOTAL] = Module(
            "label_attempts_total",
            "Attempt Count (Total)", 
            "Displays the total attempt count.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_BEST_RUN] = Module(
            "label_best_run",
            "Best Run (Session)", 
            "Displays the best percentage achieved this session.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_CLICKS] = Module(
            "label_clicks",
            "Clicks", 
            "Displays the amount of clicks this attempt.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_CLOCK] = Module(
            "label_clock",
            "Clock", 
            "Displays the current time.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_FPS] = Module(
            "label_fps",
            "FPS", 
            "Displays the current frames per second.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_FRAMES] = Module(
            "label_frames",
            "Frames", 
            "Displays the amount of frames that have passed since the start of the attempt.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_JUMPS] = Module(
            "label_jumps",
            "Jumps", 
            "Displays the current jump count.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_NOCLIP_ACCURACY] = Module(
            "label_noclip_accuracy",
            "NoClip Accuracy", 
            "If NoClip is enabled, displays how accurately the level has been played thus far.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_NOCLIP_DEATHS] = Module(
            "label_noclip_deaths",
            "NoClip Deaths", 
            "If NoClip is enabled, displays the amount of times you would have died.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_PCOMMAND] = Module(
            "label_pcommand",
            "pCommand", 
            "Displays the pCommand values.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_PLAYER_POSITION] = Module(
            "label_player_position",
            "Player Position", 
            "Displays the player's X and Y coordinates.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_PLAYER_ROTATION] = Module(
            "label_player_rotation",
            "Player Rotation", 
            "Displays the player's rotation.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_SPEEDHACK] = Module(
            "label_speedhack",
            "Speedhack Value", 
            "Displays the speed the speedhack is set to.", 
            false, ModuleCategory::Label, [](bool _){});
        modules[ModuleID::LABEL_TIME_SPENT] = Module(
            "label_time_spent",
            "Time Spent", 
            "Displays the amount of time spent this session.", 
            false, ModuleCategory::Label, [](bool _){});



        modules[ModuleID::PARTICLE_BACKGROUND] = Module(
            "particle_background",
            "Background",
            "Toggles the visibility of the particles that appear all over the background in the ship or UFO gamemode.", 
            true, ModuleCategory::Particles, [](bool _){});
#if GAME_VERSION >= GV_1_5
        modules[ModuleID::PARTICLE_BURST] = Module(
            "particle_burst",
            "Burst",
            "Toggles the visibility of the particles that appear when the UFO jumps.", 
            true, ModuleCategory::Particles, [](bool _){});
#endif
        modules[ModuleID::PARTICLE_DEATH_EFFECT] = Module(
            "particle_death_effect",
            "Death Effect",
            "Toggles the visibility of the particles that appear when the player dies.", 
            true, ModuleCategory::Particles, [](bool _){});
#if GAME_VERSION >= GV_1_6
        modules[ModuleID::PARTICLE_DESTRUCTIBLE_BLOCKS] = Module(
            "particle_death_effect",
            "Destructible Blocks",
            "Toggles the visibility of the particles that appear when colliding with destructible blocks.", 
            true, ModuleCategory::Particles, [](bool _){});
#endif
        // modules.insert(std::pair<std::string, Module*>("particle_end_wall", new Module(
        //         "End Wall",
        //         "Toggles the visibility of the particles that are emitted by the end wall.", 
        //         true, ModuleCategory::Particles, [](bool _){})));
        modules[ModuleID::PARTICLE_END_WALL] = Module(
            "particle_end_wall",
            "End Wall",
            "Toggles the visibility of the particles that are emitted by the end wall.", 
            true, ModuleCategory::Particles, [](bool _){});
        modules[ModuleID::PARTICLE_FIREWORKS] = Module(
            "particle_fireworks",
            "Fireworks",
            "Toggles the visibility of the particles that are emitted by the fireworks during the level completion animation.", 
            true, ModuleCategory::Particles, [](bool _){});
        modules[ModuleID::PARTICLE_GROUND] = Module(
            "particle_ground",
            "Ground",
            "Toggles the visibility of the particles that appear when the cube or ball is dragging across the ground.", 
            true, ModuleCategory::Particles, [](bool _){});
        modules[ModuleID::PARTICLE_LEVEL_COMPLETION] = Module(
            "particle_level_completion",
            "Level Completion",
            "Toggles the visibility of the particles that are emitted during the level completion animation.", 
            true, ModuleCategory::Particles, [](bool _){});
        modules[ModuleID::PARTICLE_LANDING] = Module(
            "particle_landing",
            "Landing",
            "Toggles the visibility of the particles that appear when the cube or ball lands on the ground.", 
            true, ModuleCategory::Particles, [](bool _){});
        modules[ModuleID::PARTICLE_OBJECTS] = Module(
            "particle_objects",
            "Objects",
            "Toggles the visibility of the particles that are emitted by objects, such as portals, pads and orbs.", 
            true, ModuleCategory::Particles, [](bool _){});
#if GAME_VERSION >= GV_1_6
        modules[ModuleID::PARTICLE_SECRET_COINS] = Module(
            "particle_secret_coins",
            "Secret Coins",
            "Toggles the visibility of the particles that appear when colliding with secret coins.", 
            true, ModuleCategory::Particles, [](bool _){});
#endif
        modules[ModuleID::PARTICLE_SHIP_FIRE] = Module(
            "particle_ship_fire",
            "Ship Fire",
            "Toggles the visibility of the particles that appear behind the ship.", 
            true, ModuleCategory::Particles, [](bool _){});
        modules[ModuleID::PARTICLE_SHIP_GROUND] = Module(
            "particle_ship_ground",
            "Ship Ground",
            "Toggles the visibility of the particles that appear when the ship is dragging across the ground.", 
            true, ModuleCategory::Particles, [](bool _){});
        modules[ModuleID::PARTICLE_SHIP_LIFT] = Module(
            "particle_ship_lift",
            "Ship Lift",
            "Toggles the visibility of the particles that appear when the ship is flying.", 
            true, ModuleCategory::Particles, [](bool _){});
#if GAME_VERSION >= GV_1_7
        modules[ModuleID::PARTICLE_SPEED_PORTALS] = Module(
            "particle_speed_portals",
            "Speed Portals",
            "Toggles the visibility of the particles that appear when a speed portal is activated.", 
            true, ModuleCategory::Particles, [](bool _){});
#endif

        lastCategory = ModuleCategory::Player;
#if GAME_VERSION > GV_1_4
        blockVerify = false;
#endif
        gdShareMessageID = 0;
        gdShareData = 0;
        timeScale = 1;
        respawnTime = 0.5;
        hasInitialized = false;
        // fpsBypass = 240;
        // originalAnimInterval = 1 / 60;

        resetValues();
    }

    HaxManager(const HaxManager&) = delete;
    HaxManager& operator=(const HaxManager&) = delete;
    HaxManager(HaxManager&&) = delete;
    HaxManager& operator=(HaxManager&&) = delete;
};