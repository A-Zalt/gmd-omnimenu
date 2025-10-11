#pragma once

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

private:
    HaxManager() {
        noClip = false;
        iconHack = false;
        textLengthBypass = false;
        charFilterBypass = false;
        swearBypass = false;
        sliderBypass = false;
        instantComplete = false;
        fastMenu = false;
        verifyHack = false;
        noMirror = false;
    }

    HaxManager(const HaxManager&) = delete;
    HaxManager& operator=(const HaxManager&) = delete;
    HaxManager(HaxManager&&) = delete;
    HaxManager& operator=(HaxManager&&) = delete;
};