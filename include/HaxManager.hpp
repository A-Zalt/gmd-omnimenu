#pragma once

#include <cocos2d.h>
#include "Module.hpp"
#include "CCMenuItemSpriteExtra.hpp"

enum class CheatIndicatorColor {
    Green,
    Yellow,
    Orange,
    Red
};

class HaxManager {

public:
    static HaxManager& sharedState() {
        static HaxManager instance;
        return instance;
    }
    // bool noClip;
    // bool iconHack;
    // bool textLengthBypass;
    // bool charFilterBypass;
    // bool swearBypass;
    // bool sliderBypass;
    // bool instantComplete;
    // bool fastMenu;
    // bool verifyHack;
    // bool noMirror;
    // bool objectLimitHack;
    // bool levelEdit;
    // bool freeScroll;
    // bool zoomBypass;
    // bool levelCopying;
    // bool pCommand;
    // bool restartOnly;
    // bool upload100KbFix;
    // bool cheatIndicator;
    // bool showPercentage;
    // bool viewAttempts;
    // bool objectCounter;
    // bool practiceMusic;
    // bool forceVisibility;
    std::map<std::string, Module*> modules;
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
    bool hasCheated;

    Module* getModule(const char* id) {
        return modules.at(std::string(id));
    }
    bool getModuleEnabled(const char* id) {
        return getModule(id)->enabled;
    }

    CheatIndicatorColor getCheatIndicatorColor() {
        if (getModuleEnabled("noclip") || 
        getModuleEnabled("instant_complete") || 
        getModuleEnabled("no_mirror") || 
        getModuleEnabled("pcommand") || 
        getModuleEnabled("force_visibility")) return CheatIndicatorColor::Red;
        if (hasCheated) return CheatIndicatorColor::Orange;
        if (getModuleEnabled("level_edit")) return CheatIndicatorColor::Yellow;
        return CheatIndicatorColor::Green;
    }

    void setCheating(bool val) {
        hasCheated = val;
    }

    // bool setAll(bool value) {
        
    // }

private:
    HaxManager() {
        modules.insert(std::pair<std::string, Module*>("cheat_indicator", new Module(
                "Cheat Indicator", 
                "Adds a dot that indicates whether you're using any unfair hacks.", 
                false, ModuleCategory::Gameplay, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("instant_complete", new Module(
                "Instant Complete", 
                "Completes the level as soon as it is entered.", 
                false, ModuleCategory::Gameplay, [](bool _){
                    HaxManager& hax = HaxManager::sharedState();
                    if (_) hax.setCheating(true);
                })));
        modules.insert(std::pair<std::string, Module*>("noclip", new Module(
                "NoClip", 
                "Prevents the player from dying.", 
                false, ModuleCategory::Gameplay, [](bool _){
                    HaxManager& hax = HaxManager::sharedState();
                    if (_) hax.setCheating(true);
                })));
        modules.insert(std::pair<std::string, Module*>("no_mirror", new Module(
                "No Mirror", 
                "Disables mirror portals.", 
                false, ModuleCategory::Gameplay, [](bool _){
                    HaxManager& hax = HaxManager::sharedState();
                    if (_) hax.setCheating(true);
                })));
        modules.insert(std::pair<std::string, Module*>("pcommand", new Module(
                "pCommand", 
                "Re-enables the unused \"pCommand\" functionality, which allows you to alter your speed, gravity and jump height. (Note: Some creative liberties had to be taken with the controls. This module is not entirely accurate to how pCommand actually worked.)", 
                false, ModuleCategory::Gameplay, [](bool _){
                    HaxManager& hax = HaxManager::sharedState();
                    if (_) hax.setCheating(true);
                })));
        modules.insert(std::pair<std::string, Module*>("practice_music", new Module(
                "Practice Music Hack", 
                "Plays the normal level music in practice mode.", 
                false, ModuleCategory::Gameplay, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("show_percentage", new Module(
                "Show Percentage", 
                "Displays the percentage the player is currently at.", 
                false, ModuleCategory::Gameplay, [](bool _){})));



        modules.insert(std::pair<std::string, Module*>("free_scroll", new Module(
                "Free Scroll", 
                "Removes the constraints for camera position in the editor.", 
                false, ModuleCategory::Editor, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("level_copy", new Module(
                "Level Copying", 
                "Adds a button to copy any level.", 
                false, ModuleCategory::Editor, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("level_edit", new Module(
                "Level Edit", 
                "Allows you to locally edit any level, as well as adding the restart button to all levels.", 
                false, ModuleCategory::Editor, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("object_counter", new Module(
                "Object Counter", 
                "Displays the object count of the level in the editor pause menu.", 
                false, ModuleCategory::Editor, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("object_hack", new Module(
                "Object Limit Bypass", 
                "Sets the object limit to 16,384. (Note: 16k Fix is not yet available. It will be added in a future update.)", 
                false, ModuleCategory::Editor, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("verify_bypass", new Module(
                "Verify Bypass", 
                "Allows you to upload any level without verifying it.", 
                false, ModuleCategory::Editor, [](bool _){})));



        modules.insert(std::pair<std::string, Module*>("char_filter_bypass", new Module(
                "Character Filter Bypass", 
                "Allows you to use any character in any textbox.", 
                false, ModuleCategory::Bypass, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("swear_filter_bypass", new Module(
                "Swear Filter Bypass", 
                "Allows you to use profanities in textboxes.", 
                false, ModuleCategory::Bypass, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("text_length_bypass", new Module(
                "Text Length Bypass", 
                "Allows you to write an unlimited amount of characters in any textbox.", 
                false, ModuleCategory::Bypass, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("icon_hack", new Module(
                "Unlock Icons", 
                "Unlocks all icons and colors in the Icon Kit.", 
                false, ModuleCategory::Bypass, [](bool _){})));



        modules.insert(std::pair<std::string, Module*>("view_level_stats", new Module(
                "View Level Stats", 
                "Adds a menu button that displays some information about the level, such as your attempt count on it.", 
                false, ModuleCategory::Informational, [](bool _){})));



        modules.insert(std::pair<std::string, Module*>("100_kb_fix", new Module(
                "100 KB Fix", 
                "Fixes a bug in Cocos2d where CCStrings always allocate 100 KB, instead allocating a dynamic buffer size. This fixes large levels being cut off on upload.", 
                true, ModuleCategory::Universal, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("fast_menu", new Module(
                "Fast Menu", 
                "Makes fade transitions instant.", 
                false, ModuleCategory::Universal, [](bool _){})));
        modules.insert(std::pair<std::string, Module*>("force_visibility", new Module(
                "Force Visibility", 
                "Makes all existing nodes visible.", 
                false, ModuleCategory::Universal, [](bool _){
                    HaxManager& hax = HaxManager::sharedState();
                    if (_) hax.setCheating(true);
                })));

        lastCategory = ModuleCategory::Gameplay;
        hasCheated = false;
    }

    HaxManager(const HaxManager&) = delete;
    HaxManager& operator=(const HaxManager&) = delete;
    HaxManager(HaxManager&&) = delete;
    HaxManager& operator=(HaxManager&&) = delete;
};