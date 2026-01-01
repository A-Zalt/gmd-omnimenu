#pragma once
#ifdef USE_MINIAUDIO
#define MINIAUDIO_IMPLEMENTATION
#include <cstdlib> 
#include "miniaudio.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <algorithm>
#include "hooks/hook.hpp"

typedef struct {
    AAsset* asset;
} asset_decoder_ctx;

static ma_result on_read(ma_decoder* pDecoder, void* pBufferOut, size_t bytesToRead, size_t* pBytesRead) {
    // __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "onRead");
    asset_decoder_ctx* ctx = (asset_decoder_ctx*)pDecoder->pUserData;

    int result = AAsset_read(ctx->asset, pBufferOut, bytesToRead);

    if (result < 0) {
        *pBytesRead = 0;
        return MA_ERROR;
    }

    *pBytesRead = (size_t)result;
    return MA_SUCCESS;
}
static ma_result on_seek(ma_decoder* pDecoder, ma_int64 offset, ma_seek_origin origin) {
    // __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "onSeek %i", origin);
    asset_decoder_ctx* ctx = (asset_decoder_ctx*)pDecoder->pUserData;

    int whence = (origin == MA_SEEK_CUR)
               ? SEEK_CUR
               : SEEK_SET;

    if (AAsset_seek(ctx->asset, offset, whence) < 0)
        return MA_ERROR;

    return MA_SUCCESS;
}
static ma_result on_tell(ma_decoder* pDecoder, ma_int64* pCursor) {
    // __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "onTell");
    asset_decoder_ctx* ctx = (asset_decoder_ctx*)pDecoder->pUserData;

    off_t pos = AAsset_seek(ctx->asset, 0, SEEK_CUR);
    if (pos < 0)
        return MA_ERROR;

    *pCursor = (ma_int64)pos;
    return MA_SUCCESS;
}


class AudioManager {
protected:
    ma_engine m_audioEngine;
    // std::vector<Sound*> m_soundEffects;
    bool m_initialized;
    // Sound* m_backgroundMusic;
    AAsset* m_backgroundMusic;
    // std::map<std::string, Asset> m_preloadedEffects;
    // std::map<std::string, Asset> m_preloadedMusics;
    AAssetManager* m_assetManager;
    ma_decoder m_decoder;
    ma_sound m_bgmSound;
    float m_timeCache;
    asset_decoder_ctx m_decoderCtx;

    bool m_soundInit;
    bool m_decoderInit;
    bool m_paused;
    // Asset createAsset(const char* filename) {
    //     AAsset* asset = AAssetManager_open(m_assetManager, filename, AASSET_MODE_BUFFER);
    //     if (!asset) return Asset{nullptr, 0};

    //     size_t size = AAsset_getLength(asset);
    //     // CCLog("a4 %s %zu", filename, size);
    //     unsigned char* buffer = (unsigned char*)malloc(size);

    //     AAsset_read(asset, buffer, size);
    //     AAsset_close(asset);

    //     return Asset{buffer, size};
    // }

public:
    bool m_areWeInPlayLayer;
    bool m_playing;
    const char* m_filename;
    float m_volumeCache;

    // void destroySound(Sound* inst) {
    //     __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "destroySound");
    //     if (inst->destroyed) return;
    //     inst->destroyed = true;

    //     ma_sound_stop(&inst->sound);
    //     ma_sound_uninit(&inst->sound);
    //     ma_decoder_uninit(&inst->decoder);

    //     auto it = std::find(m_soundEffects.begin(), m_soundEffects.end(), inst);

    //     if (it != m_soundEffects.end())
    //         m_soundEffects.erase(it);

    //     delete inst;
    // }

    // static void onSoundEnd(void* pUserData, ma_sound* pSound) {
    //     Sound* inst = (Sound*)pUserData;
    //     CCLog("Sound goes byebye");
    //     AudioManager::sharedManager().destroySound(inst);
    // }

    void initializeHooks();

    // void preloadEffect(const char* filename) {
    //     if (!m_initialized) return;
    //     if (m_preloadedEffects.count(filename)) return;
    //     auto asset = createAsset(filename);
    //     m_preloadedEffects[filename] = asset;
    //     // if (!asset) {
    //     //     __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Could not preload sound effect: %s", filename);
    //     //     return;
    //     // }
    // }
    void preloadBackgroundMusic(const char* filename) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "preloadBackgroundMusic %s", filename);
        if (!m_initialized) return;
        // if (m_preloadedMusics.count(filename)) return;
        stopBackgroundMusic();
        m_backgroundMusic = AAssetManager_open(m_assetManager, filename, AASSET_MODE_STREAMING);
        if (!m_backgroundMusic) return;
        m_filename = filename;
        // auto asset = createAsset(filename);
        // m_preloadedMusics[filename] = asset;
        // __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "Preloading music: %s", filename);
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
        if (!m_soundInit || !m_decoderInit || !m_backgroundMusic) return false;
        return ma_sound_is_playing(&m_bgmSound);
    }
    void playBackgroundMusic(const char* filename, bool loop) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "playBackgroundMusic");
        if (!m_initialized) return;
        // if (m_preloadedMusics.count(filename) == 0) preloadBackgroundMusic(filename);

        // auto fx = m_preloadedMusics[filename];
        if (!strcmp(filename, m_filename) && m_playing) {
            resumeBackgroundMusic();
            return;
        }

        if (!m_backgroundMusic) preloadBackgroundMusic(filename);
        
        if (!m_backgroundMusic) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Failed to load file (no asset): %s", filename);
            return;
        }

        if (AAsset_getLength(m_backgroundMusic) < 4) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Failed to load file (no data): %s", filename);
            return;
        }

        AAsset_seek(m_backgroundMusic, 0, SEEK_SET);

        m_decoderCtx = { m_backgroundMusic };
        
    // #if GDPS == GDPS_1_7
        ma_decoder_config config = ma_decoder_config_init(ma_format_unknown, 0, 0);
    // #endif
        auto r1 = ma_decoder_init(on_read, on_seek, on_tell, &m_decoderCtx, &config, &m_decoder);
        if (r1 != MA_SUCCESS) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Decoder failed for %s: %d", filename, r1);
            m_decoderInit = false;
            return;
        }
        m_decoderInit = true;

        if (ma_sound_init_from_data_source(&m_audioEngine, &m_decoder, 0, NULL, NULL, &m_bgmSound) != MA_SUCCESS) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Music init failed for %s", filename);
            m_soundInit = false;
            return;
        }
        m_soundInit = true;
        m_paused = false;
        m_playing = true;

        // ma_sound_set_end_callback(&inst->sound, AudioManager::onSoundEnd, inst);

        ma_sound_set_looping(&m_bgmSound, loop);
        ma_sound_set_volume(&m_bgmSound, m_volumeCache);
        ma_sound_set_start_time_in_milliseconds(&m_bgmSound, 0);

        if (ma_sound_start(&m_bgmSound) != MA_SUCCESS) {
            __android_log_print(ANDROID_LOG_ERROR, "OmniAudioManager", "Music playback failed for %s", filename);
            return;
        }
    }
    // void pauseAllEffects() {
    //     if (!m_initialized) return;
    //     for (Sound* inst : m_soundEffects) {
    //         ma_sound_stop(&inst->sound);
    //     }
    // }
    void pauseBackgroundMusic() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "pauseBackgroundMusic");
        if (!m_initialized || m_paused) return;
        if (m_soundInit && ma_sound_is_playing(&m_bgmSound)) {
            ma_sound_get_cursor_in_seconds(&m_bgmSound, &m_timeCache);
            ma_sound_stop(&m_bgmSound);
            m_paused = true;
        }
    }
    // void resumeAllEffects() {
    //     if (!m_initialized) return;
    //     for (Sound* inst : m_soundEffects) {
    //         ma_sound_start(&inst->sound);
    //     }
    // }
    void resumeBackgroundMusic() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "resumeBackgroundMusic");
        if (!m_initialized || !m_paused) return;
        // ma_sound_seek_to_pcm_frame(&m_backgroundMusic->sound, floorf((m_timeCache * 1000) * (ma_engine_get_sample_rate(&m_audioEngine) / 1000)));
        if (m_soundInit && !ma_sound_is_playing(&m_bgmSound)) {
            // ma_sound_seek_to_second(&m_bgmSound, m_timeCache);
            ma_sound_start(&m_bgmSound);
            m_paused = false;
        }
    }
    // void stopAllEffects() {
    //     if (!m_initialized) return;
    //     for (Sound* inst : m_soundEffects) {
    //         ma_sound_stop(&inst->sound);
    //         destroySound(inst);
    //     }
    //     m_soundEffects.clear();
    // }
    void stopBackgroundMusic() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "stopBackgroundMusic");
        if (!m_initialized || !m_playing) return;
        if (m_soundInit) {
            ma_sound_stop(&m_bgmSound);
            ma_sound_uninit(&m_bgmSound);
            m_soundInit = false;
        }
        if (m_decoderInit) {
            ma_decoder_uninit(&m_decoder);
            m_decoderInit = false;
        }
        if (m_backgroundMusic) {
            AAsset_close(m_backgroundMusic);
            m_backgroundMusic = nullptr;
        }
        m_paused = false;
        m_playing = false;
    }
    // void setEffectsVolume(float volume) {
    //     if (!m_initialized) return;
    //     for (Sound* inst : m_soundEffects) {
    //         ma_sound_set_volume(&inst->sound, volume);
    //     }
    // }
    void setBackgroundMusicVolume(float volume) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "setBackgroundMusicVolume %f", volume);
        if (!m_initialized) return;
        m_volumeCache = volume;
        if (m_soundInit) 
            ma_sound_set_volume(&m_bgmSound, volume);
    }
    void setBackgroundMusicTime(float seconds) {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "setBackgroundMusicTime %f", seconds);
        if (!m_initialized) return;
        if (m_soundInit)
            ma_sound_seek_to_second(&m_bgmSound, seconds);
    }
    void rewindBackgroundMusic() {
        __android_log_print(ANDROID_LOG_INFO, "OmniAudioManager", "rewindBackgroundMusic");
        if (!m_initialized || !m_soundInit) return;
        ma_sound_stop(&m_bgmSound);
        ma_sound_seek_to_pcm_frame(&m_bgmSound, 0);
        ma_sound_start(&m_bgmSound);
        m_paused = false;
    }

    ma_engine* getEngine() {
        return &m_audioEngine;
    }

    ma_result init(JNIEnv* env, jobject assetManager) {
        if (m_initialized) return MA_ERROR;
        m_assetManager = AAssetManager_fromJava(env, assetManager);

        ma_engine_config cfg = ma_engine_config_init();
    // #if GDPS == GDPS_1_7
    //     cfg.sampleRate = 48000;
    // #endif

        ma_result result = ma_engine_init(&cfg, &m_audioEngine);
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
    AudioManager()
        : m_initialized(false)
        , m_backgroundMusic(nullptr)
        , m_filename("")
        , m_assetManager(nullptr)
        , m_soundInit(false)
        , m_decoderInit(false)
        , m_paused(false)
        , m_playing(false)
        , m_volumeCache(1.f)
        , m_timeCache(0.f)
        , m_areWeInPlayLayer(false)
    {}
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
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return;
    am.preloadBackgroundMusic(path);
}
void playBackgroundMusicJNI_MA(const char* path, bool loop) {
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return;
    am.playBackgroundMusic(path, loop);
}
void pauseBackgroundMusicJNI_MA() {
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return;
    am.pauseBackgroundMusic();
}
void resumeBackgroundMusicJNI_MA() {
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return;
    am.resumeBackgroundMusic();
}
void stopBackgroundMusicJNI_MA() {
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return;
    am.stopBackgroundMusic();
}
void setBackgroundMusicVolumeJNI_MA(float volume) {
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return;
    am.setBackgroundMusicVolume(volume);
}
void setBackgroundMusicTimeJNI_MA(float seconds) {
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return;
    am.setBackgroundMusicTime(seconds);
}
bool isBackgroundMusicPlayingJNI_MA() {
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return false;
    return am.isBackgroundMusicPlaying();
}
void rewindBackgroundMusicJNI_MA() {
    auto& am = AudioManager::sharedManager();
    if (am.m_areWeInPlayLayer) return;
    am.rewindBackgroundMusic();
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
#endif // USE_MINIAUDIO