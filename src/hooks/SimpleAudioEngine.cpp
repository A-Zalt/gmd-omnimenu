#include "hook.hpp"
#include "Utils.hpp"

void (*TRAM_setBackgroundMusicTimeJNI)(float time);
void setBackgroundMusicTimeJNI(float time) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.areWeInPlayLayer) return;
    seekBackgroundMusicTo(floorf(time * 1000));
}

void (*playBackgroundMusicJNI_)(const char* path, bool loop);
void playBackgroundMusicJNI_MA(const char* path, bool loop) {
    HaxManager& hax = HaxManager::sharedState();
    CCLog("playBackgroundMusicJNI");
    if (hax.areWeInPlayLayer) return;
    playBackgroundMusicJNI_(path, loop);
}
void (*pauseBackgroundMusicJNI_)();
void pauseBackgroundMusicJNI_MA() {
    HaxManager& hax = HaxManager::sharedState();
    CCLog("pauseBackgroundMusicJNI");
    if (hax.areWeInPlayLayer) return;
    pauseBackgroundMusicJNI_();
}
void (*resumeBackgroundMusicJNI_)();
void resumeBackgroundMusicJNI_MA() {
    HaxManager& hax = HaxManager::sharedState();
    CCLog("resumeBackgroundMusicJNI");
    if (hax.areWeInPlayLayer) return;
    resumeBackgroundMusicJNI_();
}
void (*stopBackgroundMusicJNI_)();
void stopBackgroundMusicJNI_MA() {
    HaxManager& hax = HaxManager::sharedState();
    CCLog("stopBackgroundMusicJNI");
    if (hax.areWeInPlayLayer) return;
    stopBackgroundMusicJNI_();
}
void (*setBackgroundMusicVolumeJNI_)(float volume);
void setBackgroundMusicVolumeJNI_MA(float volume) {
    HaxManager& hax = HaxManager::sharedState();
    CCLog("setBackgroundMusicVolumeJNI");
    if (hax.areWeInPlayLayer) return;
    setBackgroundMusicVolumeJNI_(volume);
}
bool (*isBackgroundMusicPlayingJNI_)();
bool isBackgroundMusicPlayingJNI_MA() {
    HaxManager& hax = HaxManager::sharedState();
    CCLog("isBackgroundMusicPlayingJNI");
    if (hax.areWeInPlayLayer) return false;
    return isBackgroundMusicPlayingJNI_();
}
void (*rewindBackgroundMusicJNI_)();
void rewindBackgroundMusicJNI_MA() {
    HaxManager& hax = HaxManager::sharedState();
    CCLog("rewindBackgroundMusicJNI");
    if (hax.areWeInPlayLayer) return;
    rewindBackgroundMusicJNI_();
}

void SimpleAudioEngine_om() {
    Omni::hook("setBackgroundMusicTimeJNI",
        reinterpret_cast<void*>(setBackgroundMusicTimeJNI),
        reinterpret_cast<void**>(&TRAM_setBackgroundMusicTimeJNI));

    Omni::hook("playBackgroundMusicJNI",      reinterpret_cast<void*>(playBackgroundMusicJNI_MA),      reinterpret_cast<void**>(&playBackgroundMusicJNI_)     );
    Omni::hook("pauseBackgroundMusicJNI",     reinterpret_cast<void*>(pauseBackgroundMusicJNI_MA),     reinterpret_cast<void**>(&pauseBackgroundMusicJNI_)    );
    Omni::hook("resumeBackgroundMusicJNI",    reinterpret_cast<void*>(resumeBackgroundMusicJNI_MA),    reinterpret_cast<void**>(&resumeBackgroundMusicJNI_)   );
    Omni::hook("stopBackgroundMusicJNI",      reinterpret_cast<void*>(stopBackgroundMusicJNI_MA),      reinterpret_cast<void**>(&stopBackgroundMusicJNI_)     );
    Omni::hook("setBackgroundMusicVolumeJNI", reinterpret_cast<void*>(setBackgroundMusicVolumeJNI_MA), reinterpret_cast<void**>(&setBackgroundMusicVolumeJNI_));
    Omni::hook("isBackgroundMusicPlayingJNI", reinterpret_cast<void*>(isBackgroundMusicPlayingJNI_MA), reinterpret_cast<void**>(&isBackgroundMusicPlayingJNI_));
    Omni::hook("rewindBackgroundMusicJNI",    reinterpret_cast<void*>(rewindBackgroundMusicJNI_MA),    reinterpret_cast<void**>(&rewindBackgroundMusicJNI_)   );
}