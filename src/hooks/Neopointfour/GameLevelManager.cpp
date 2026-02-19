#include "../hook.hpp"
#include "CCHttpClient.cpp"

void setCurrentUploadingLevel(GJGameLevel* level);

void (*TRAM_GameLevelManager_uploadLevel)(GameLevelManager* self, GJGameLevel* level);
void GameLevelManager_uploadLevel(GameLevelManager* self, GJGameLevel* level) {
    setCurrentUploadingLevel(level);
    TRAM_GameLevelManager_uploadLevel(self, level);
    setCurrentUploadingLevel(nullptr);
}

void* (*TRAM_GameLevelManager_getStoredOnlineLevels)(void* self, const char* key);
void* GameLevelManager_getStoredOnlineLevels(void* self, const char* key) {
    auto GLM = GameLevelManager::sharedState();
    bool demonFilter = GLM->getBoolForKey("demon_filter");
    
    if (demonFilter && (strcmp(key, "leaderboard_top") == 0 || strcmp(key, "leaderboard_global") == 0)) {
        std::string modifiedKey = std::string(key) + "_demons";
        return TRAM_GameLevelManager_getStoredOnlineLevels(self, modifiedKey.c_str());
    }
    
    return TRAM_GameLevelManager_getStoredOnlineLevels(self, key);
}

bool (*TRAM_GameLevelManager_isTimeValid)(GameLevelManager* self, const char* key, float length);
bool GameLevelManager_isTimeValid(GameLevelManager* self, const char* key, float length) {
    auto GLM = GameLevelManager::sharedState();
    bool demonFilter = GLM->getBoolForKey("demon_filter");
    
    if ((strcmp(key, "leaderboard_top") == 0 || strcmp(key, "leaderboard_global") == 0)) {
        return false; // bandaid fix but I couldn't care less
    }
    
    bool result = TRAM_GameLevelManager_isTimeValid(self, key, length);
    return result;
}

void NP4_GameLevelManager_om() {
    Omni::hook("_ZN16GameLevelManager11uploadLevelEP11GJGameLevel",
        reinterpret_cast<void*>(GameLevelManager_uploadLevel),
        reinterpret_cast<void**>(&TRAM_GameLevelManager_uploadLevel));
    Omni::hook("_ZN16GameLevelManager21getStoredOnlineLevelsEPKc",
        reinterpret_cast<void*>(GameLevelManager_getStoredOnlineLevels),
        reinterpret_cast<void**>(&TRAM_GameLevelManager_getStoredOnlineLevels));
    Omni::hook("_ZN16GameLevelManager11isTimeValidEPKcf",
        reinterpret_cast<void*>(GameLevelManager_isTimeValid),
        reinterpret_cast<void**>(&TRAM_GameLevelManager_isTimeValid));
}