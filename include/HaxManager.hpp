#pragma once

typedef enum {
    Green = 1,
    Yellow = 2,
    Red = 3
} CheatIndicatorColor;

class HaxManager {

public:
    static HaxManager& sharedState() {
        static HaxManager instance;
        return instance;
    }
    bool noClip;
    bool iconHack;
    bool textLengthBypass;
    bool charFilterBypass;
    bool swearBypass;
    bool sliderBypass;
    bool instantComplete;
    bool fastMenu;
    bool verifyHack;
    bool noMirror;
    bool objectLimitHack;
    bool levelEdit;
    bool freeScroll;
    bool zoomBypass;
    bool levelCopying;
    bool pCommand;
    bool restartOnly;
    bool upload100KbFix;

    bool getCheatIndicatorColor() {
        if (noClip || instantComplete || noMirror || pCommand) return CheatIndicatorColor::Red;
        if (levelEdit) return CheatIndicatorColor::Yellow;
        return CheatIndicatorColor::Green;
    }

    bool setAll(bool value) {
        noClip = value;
        iconHack = value;
        textLengthBypass = value;
        charFilterBypass = value;
        swearBypass = value;
        sliderBypass = value;
        instantComplete = value;
        fastMenu = value;
        verifyHack = value;
        noMirror = value;
        objectLimitHack = value;
        levelEdit = value;
        freeScroll = value;
        zoomBypass = value;
        levelCopying = value;
        pCommand = value;
        restartOnly = value;
        upload100KbFix = value;
    }

private:
    HaxManager() {
        setAll(false);
        upload100KbFix = true;
    }

    HaxManager(const HaxManager&) = delete;
    HaxManager& operator=(const HaxManager&) = delete;
    HaxManager(HaxManager&&) = delete;
    HaxManager& operator=(HaxManager&&) = delete;
};