#pragma once
#define _POSIX_C_SOURCE 200112L
#include <cstdlib> 
#include "miniaudio.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <algorithm>
#include "hooks/hook.hpp"

struct Sound {
    ma_sound sound;
    void* buffer;
    ma_decoder decoder;
    bool destroyed;
};

struct Asset {
    unsigned char* data = nullptr;
    size_t size = 0;

    void destroy() {
        free(data);
        data = nullptr;
        size = 0;
    }

    Asset() = default;
    Asset(unsigned char* d, size_t s)
        : data(d), size(s) {}
};

class AudioManager {
protected:
    ma_engine m_audioEngine;
    std::vector<Sound*> m_soundEffects;
    bool m_initialized;
    Sound* m_backgroundMusic;
    std::map<std::string, Asset> m_preloadedEffects;
    std::map<std::string, Asset> m_preloadedMusics;
    AAssetManager* m_assetManager;
    float m_timeCache;
    float m_volumeCache;

    Asset createAsset(const char* filename) {
        AAsset* asset = AAssetManager_open(m_assetManager, filename, AASSET_MODE_BUFFER);
        if (!asset) return Asset{nullptr, 0};

        size_t size = AAsset_getLength(asset);
        // CCLog("a4 %s %zu", filename, size);
        unsigned char* buffer = (unsigned char*)malloc(size);

        AAsset_read(asset, buffer, size);
        AAsset_close(asset);

        return Asset{buffer, size};
    }

public:

    void destroySound(Sound* inst) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "destroySound");
        if (inst->destroyed) return;
        inst->destroyed = true;

        ma_sound_stop(&inst->sound);
        ma_sound_uninit(&inst->sound);
        ma_decoder_uninit(&inst->decoder);

        auto it = std::find(m_soundEffects.begin(), m_soundEffects.end(), inst);

        if (it != m_soundEffects.end())
            m_soundEffects.erase(it);

        delete inst;
    }

    // static void onSoundEnd(void* pUserData, ma_sound* pSound) {
    //     Sound* inst = (Sound*)pUserData;
    //     CCLog("Sound goes byebye");
    //     AudioManager::sharedManager().destroySound(inst);
    // }

    void initializeHooks();

    void preloadEffect(const char* filename) {
        if (!m_initialized) return;
        if (m_preloadedEffects.count(filename)) return;
        auto asset = createAsset(filename);
        m_preloadedEffects[filename] = asset;
        // if (!asset) {
        //     __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Could not preload sound effect: %s", filename);
        //     return;
        // }
    }
    void preloadBackgroundMusic(const char* filename) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "preloadBackgroundMusic %s", filename);
        if (!m_initialized) return;
        if (m_preloadedMusics.count(filename)) return;
        auto asset = createAsset(filename);
        m_preloadedMusics[filename] = asset;
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "Preloading music: %s", filename);
    }

    // void playEffect(const char* filename, float volume) {
    //     if (!m_initialized) return;
    //     if (m_preloadedEffects.count(filename) == 0) preloadEffect(filename);

    //     auto fx = m_preloadedEffects[filename];
        
    //     ma_decoder decoder;
        
    //     ma_decoder_config config = ma_decoder_config_init(ma_format_unknown, 0, 0);
    //     auto r1 = ma_decoder_init_memory(fx.data, fx.size, &config, &decoder);
    //     // __android_log_print(ANDROID_LOG_DEBUG, "OmniAudioManager", "First 4 bytes: %02x %02x %02x %02x",
    //     //     fx.data[0], fx.data[1], fx.data[2], fx.data[3]);
    //     if (r1 != MA_SUCCESS) {
    //         __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Decoder failed for %s: %i", filename, r1);
    //         return;
    //     }

    //     auto* inst = new Sound();
    //     inst->buffer = fx.data;
    //     inst->decoder = decoder;

    //     // ma_sound_config cfg = ma_sound_config_init();
    //     // cfg.pUserData = inst;
    //     // cfg.onEnd = onSoundEnd;

    //     if (ma_sound_init_from_data_source(&m_audioEngine, &inst->decoder, 0, NULL, &inst->sound) != MA_SUCCESS) {
    //         __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Sound init failed for %s", filename);
    //         return;
    //     }

    //     ma_sound_set_end_callback(&inst->sound, AudioManager::onSoundEnd, inst);

    //     ma_sound_set_volume(&inst->sound, volume);

    //     if (ma_sound_start(&inst->sound) != MA_SUCCESS) {
    //         __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Sound playback failed for %s", filename);
    //         return;
    //     }
    //     m_soundEffects.push_back(inst);
    // }
    // void playEffect(const char* filename) {
    //     playEffect(filename, 1);
    // }
    bool isBackgroundMusicPlaying() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "isBackgroundMusicPlaying");
        if (!m_initialized) return false;
        if (!m_backgroundMusic) return false;
        if (m_backgroundMusic->destroyed) return false;
        return ma_sound_is_playing(&m_backgroundMusic->sound);
    }
    void playBackgroundMusic(const char* filename, bool loop) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "playBackgroundMusic");
        if (!m_initialized) return;
        if (m_preloadedMusics.count(filename) == 0) preloadBackgroundMusic(filename);

        auto fx = m_preloadedMusics[filename];

        if (fx.size < 4) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Failed to load file: %s", filename);
            return;
        }
        
        auto* inst = new Sound();
        inst->buffer = fx.data;
        
        ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 0, 0);
        auto r1 = ma_decoder_init_memory(fx.data, fx.size, &config, &inst->decoder);
        __android_log_print(ANDROID_LOG_DEBUG, "OmniAudioManager", "First 3 bytes: %02x %02x %02x",
            fx.data[0], fx.data[1], fx.data[2]);
        if (r1 != MA_SUCCESS) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Decoder failed for %s: %d", filename, r1);
            return;
        }

        if (ma_sound_init_from_data_source(&m_audioEngine, &inst->decoder, 0, NULL, NULL, &inst->sound) != MA_SUCCESS) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Music init failed for %s", filename);
            return;
        }

        // ma_sound_set_end_callback(&inst->sound, AudioManager::onSoundEnd, inst);

        ma_sound_set_looping(&inst->sound, loop);
        ma_sound_set_volume(&inst->sound, m_volumeCache);
        ma_sound_set_start_time_in_milliseconds(&inst->sound, 0);

        if (ma_sound_start(&inst->sound) != MA_SUCCESS) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Music playback failed for %s", filename);
            return;
        }
        m_backgroundMusic = inst;
    }
    void pauseAllEffects() {
        if (!m_initialized) return;
        for (Sound* inst : m_soundEffects) {
            ma_sound_stop(&inst->sound);
        }
    }
    void pauseBackgroundMusic() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "pauseBackgroundMusic");
        if (!m_initialized) return;
        if (!m_backgroundMusic || m_backgroundMusic->destroyed) return;
        ma_sound_stop(&m_backgroundMusic->sound);
    }
    void resumeAllEffects() {
        if (!m_initialized) return;
        for (Sound* inst : m_soundEffects) {
            ma_sound_start(&inst->sound);
        }
    }
    void resumeBackgroundMusic() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "resumeBackgroundMusic");
        if (!m_initialized) return;
        if (!m_backgroundMusic || m_backgroundMusic->destroyed) return;
        // ma_sound_seek_to_pcm_frame(&m_backgroundMusic->sound, floorf((m_timeCache * 1000) * (ma_engine_get_sample_rate(&m_audioEngine) / 1000)));
        ma_sound_start(&m_backgroundMusic->sound);
    }
    void stopAllEffects() {
        if (!m_initialized) return;
        for (Sound* inst : m_soundEffects) {
            ma_sound_stop(&inst->sound);
            destroySound(inst);
        }
        m_soundEffects.clear();
    }
    void stopBackgroundMusic() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "stopBackgroundMusic");
        if (!m_initialized) return;
        if (!m_backgroundMusic || m_backgroundMusic->destroyed) return;
        ma_sound_stop(&m_backgroundMusic->sound);
        destroySound(m_backgroundMusic);
        m_backgroundMusic = nullptr;
    }
    void setEffectsVolume(float volume) {
        if (!m_initialized) return;
        for (Sound* inst : m_soundEffects) {
            ma_sound_set_volume(&inst->sound, volume);
        }
    }
    void setBackgroundMusicVolume(float volume) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "setBackgroundMusicVolume %f", volume);
        if (!m_initialized) return;
        m_volumeCache = volume;
        if (!m_backgroundMusic || m_backgroundMusic->destroyed) return;
        ma_sound_set_volume(&m_backgroundMusic->sound, volume);
    }
    void setBackgroundMusicTime(float seconds) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "setBackgroundMusicTime %f", seconds);
        if (!m_initialized) return;
        m_timeCache = seconds;
        if (!m_backgroundMusic || m_backgroundMusic->destroyed || !isBackgroundMusicPlaying()) return;
        ma_sound_seek_to_second(&m_backgroundMusic->sound, seconds);
    }
    void rewindBackgroundMusic() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "rewindBackgroundMusic");
        if (!m_initialized) return;
        if (!m_backgroundMusic || m_backgroundMusic->destroyed) return;
        ma_sound_stop(&m_backgroundMusic->sound);
        ma_sound_seek_to_pcm_frame(&m_backgroundMusic->sound, 0);
        ma_sound_start(&m_backgroundMusic->sound);
    }

    ma_engine* getEngine() {
        return &m_audioEngine;
    }

    ma_result init(JNIEnv* env, jobject assetManager) {
        if (m_initialized) return MA_ERROR;
        m_assetManager = AAssetManager_fromJava(env, assetManager);
        ma_result result = ma_engine_init(NULL, &m_audioEngine);
        if (result != MA_SUCCESS) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Could not initialize engine");
            return result;
        }
        m_initialized = true;
        initializeHooks();
        return MA_SUCCESS;
    }

    static AudioManager& sharedManager() {
        static AudioManager instance;
        return instance;
    }
private:
    AudioManager() {
        m_initialized = false;
        m_volumeCache = 1;
        m_timeCache = 0;
    }
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;
};

// void preloadEffectJNI(const char* path) {
//     AudioManager::sharedManager().preloadEffect(path);
// }
// unsigned int playEffectJNI(const char* path, bool bLoop, float playbackRate, float weirdVolumeThingy, float volume) {
//     AudioManager::sharedManager().playEffect(path, volume);
//     return 1;
// }
// void stopAllEffectsJNI() {
//     AudioManager::sharedManager().stopAllEffects();
// }
// void pauseAllEffectsJNI() {
//     AudioManager::sharedManager().pauseAllEffects();
// }
// void resumeAllEffectsJNI() {
//     AudioManager::sharedManager().resumeAllEffects();
// }
// void setEffectsVolumeJNI(float volume) {
//     AudioManager::sharedManager().setEffectsVolume(volume);
// }

void preloadBackgroundMusicJNI_MA(const char* path) {
    AudioManager::sharedManager().preloadBackgroundMusic(path);
}
void playBackgroundMusicJNI_MA(const char* path, bool loop) {
    AudioManager::sharedManager().playBackgroundMusic(path, loop);
}
void pauseBackgroundMusicJNI_MA() {
    AudioManager::sharedManager().pauseBackgroundMusic();
}
void resumeBackgroundMusicJNI_MA() {
    AudioManager::sharedManager().resumeBackgroundMusic();
}
void stopBackgroundMusicJNI_MA() {
    AudioManager::sharedManager().stopBackgroundMusic();
}
void setBackgroundMusicVolumeJNI_MA(float volume) {
    AudioManager::sharedManager().setBackgroundMusicVolume(volume);
}
void setBackgroundMusicTimeJNI_MA(float seconds) {
    AudioManager::sharedManager().setBackgroundMusicTime(seconds);
}
bool isBackgroundMusicPlayingJNI_MA() {
    return AudioManager::sharedManager().isBackgroundMusicPlaying();
}
void rewindBackgroundMusicJNI_MA() {
    AudioManager::sharedManager().rewindBackgroundMusic();
}


void AudioManager::initializeHooks() {
    // Omni::hook("preloadEffectJNI",    reinterpret_cast<void*>(preloadEffectJNI),    nullptr);
    // Omni::hook("playEffectJNI",       reinterpret_cast<void*>(playEffectJNI),       nullptr);
    // Omni::hook("pauseAllEffectsJNI",  reinterpret_cast<void*>(pauseAllEffectsJNI),  nullptr);
    // Omni::hook("resumeAllEffectsJNI", reinterpret_cast<void*>(resumeAllEffectsJNI), nullptr);
    // Omni::hook("setEffectsVolumeJNI", reinterpret_cast<void*>(setEffectsVolumeJNI), nullptr);

    Omni::hook("preloadBackgroundMusicJNI",   reinterpret_cast<void*>(preloadBackgroundMusicJNI_MA),   nullptr);
    Omni::hook("playBackgroundMusicJNI",      reinterpret_cast<void*>(playBackgroundMusicJNI_MA),      nullptr);
    Omni::hook("pauseBackgroundMusicJNI",     reinterpret_cast<void*>(pauseBackgroundMusicJNI_MA),     nullptr);
    Omni::hook("resumeBackgroundMusicJNI",    reinterpret_cast<void*>(resumeBackgroundMusicJNI_MA),    nullptr);
    Omni::hook("stopBackgroundMusicJNI",      reinterpret_cast<void*>(stopBackgroundMusicJNI_MA),      nullptr);
    Omni::hook("setBackgroundMusicVolumeJNI", reinterpret_cast<void*>(setBackgroundMusicVolumeJNI_MA), nullptr);
    Omni::hook("setBackgroundMusicTimeJNI",   reinterpret_cast<void*>(setBackgroundMusicTimeJNI_MA),   nullptr);
    Omni::hook("isBackgroundMusicPlayingJNI", reinterpret_cast<void*>(isBackgroundMusicPlayingJNI_MA), nullptr);
    Omni::hook("rewindBackgroundMusicJNI",    reinterpret_cast<void*>(rewindBackgroundMusicJNI_MA),    nullptr);
}