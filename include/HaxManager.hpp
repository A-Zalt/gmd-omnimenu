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

private:
    HaxManager() {
        noClip = false;
        iconHack = false;
        textLengthBypass = false;
        charFilterBypass = false;
    }

    HaxManager(const HaxManager&) = delete;
    HaxManager& operator=(const HaxManager&) = delete;
    HaxManager(HaxManager&&) = delete;
    HaxManager& operator=(HaxManager&&) = delete;
};