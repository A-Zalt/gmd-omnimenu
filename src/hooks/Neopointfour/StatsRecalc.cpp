#ifndef NP4_STATSRECALC_CPP
#define NP4_STATSRECALC_CPP

#include "../hook.hpp"
#include "CCHttpClient.cpp"

bool hasRecalculated = false;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<int> getCompletedMainLevelStars() {
    std::vector<int> stars;
    
    auto glm = GameLevelManager::sharedState();
    
    for (int page = 1; page < 13; page++) {
        auto level = glm->getMainLevel(page);
        if (!level) {
            continue;
        }
        
        int percentage = level->m_nNormalPercent;
        int levelStars = level->m_nStars;
        
        if (percentage == 100) {
            stars.push_back(levelStars);
        }
    }
    
    return stars;
}

void parseAndRecalculateStars(const std::string& response) {
    if (response == "-1") {
        return;
    }
    auto GSM = GameStatsManager::sharedState();
    auto statsDict = getStatsDict(GSM);
    auto zeroStr = cocos2d::CCString::createWithFormat("%i", 0);
    statsDict->setObject(zeroStr, "6");
    statsDict->setObject(zeroStr, "5");
    
    int totalStars = 0;
    int totalDemons = 0;
    size_t pos = 0;
    int levelCount = 0;
    
    while (pos < response.length()) {
        size_t levelEnd = response.find('|', pos);
        if (levelEnd == std::string::npos) levelEnd = response.length();
        
        std::string levelData = response.substr(pos, levelEnd - pos);
        if (levelData.empty()) {
            pos = levelEnd + 1;
            continue;
        }
        
        int levelID = 0;
        int stars = 0;
        int demon = 0;
        
        size_t fieldPos = 0;
        while (fieldPos < levelData.length()) {
            size_t keyEnd = levelData.find(':', fieldPos);
            if (keyEnd == std::string::npos) break;
            
            std::string key = levelData.substr(fieldPos, keyEnd - fieldPos);
            fieldPos = keyEnd + 1;
            
            size_t valueEnd = levelData.find(':', fieldPos);
            if (valueEnd == std::string::npos) valueEnd = levelData.length();
            
            std::string value = levelData.substr(fieldPos, valueEnd - fieldPos);
            fieldPos = valueEnd + 1;
            
            if (key == "1") {
                levelID = atoi(value.c_str());
            } else if (key == "18") {
                stars = atoi(value.c_str());
            } else if (key == "17") {
                demon = atoi(value.c_str());
            }
        }
        
        if (levelID > 0 && stars > 0) {
            levelCount++;
            totalStars += stars;
            
            char keyStr[64];
            sprintf(keyStr, "star_%i", levelID);

            std::string keyString;
            keyString.assign(keyStr);

            auto dict = getCompletedDict(GSM);
            auto starCountStr = cocos2d::CCString::createWithFormat("%i", stars);
            dict->setObject(starCountStr, keyString);
            
            GSM->incrementStat("6", stars);
        }
        if (levelID > 0 && demon > 0){
            totalDemons++;
            char keyStr[64];
            sprintf(keyStr, "demon_%i", levelID);

            std::string keyString;
            keyString.assign(keyStr);

            auto dict = getCompletedDict(GSM);
            auto demonStr = cocos2d::CCString::create("1");
            dict->setObject(demonStr, keyString);

            GSM->incrementStat("5", 1);
        }
        
        pos = levelEnd + 1;
    }

    auto mainLevelStars = getCompletedMainLevelStars();
    int mainStarsTotal = 0;
    for (int stars : mainLevelStars) {
        mainStarsTotal += stars;
        GSM->incrementStat("6", stars);
    }
}

void recalculateStars() {
    auto completedIDs = getCompletedLevelIDs();
    
    if (completedIDs.empty()) {
        return;
    }
    
    std::stringstream ss;
    for (size_t i = 0; i < completedIDs.size(); i++) {
        if (i > 0) ss << ",";
        ss << completedIDs[i];
    }
    std::string completedStr = ss.str();
    
    cocos2d::CCLog("Recalculating stars for %d levels", completedIDs.size());
    
    void* handle = dlopen("libgame.so", RTLD_LAZY);

    typedef void* (*curl_init_t)();
    typedef int (*curl_setopt_t)(void*, int, ...);
    typedef int (*curl_perform_t)(void*);
    typedef void (*curl_cleanup_t)(void*);
    
    auto curl_init = (curl_init_t)dlsym(handle, "curl_easy_init");
    auto curl_setopt = (curl_setopt_t)dlsym(handle, "curl_easy_setopt");
    auto curl_perform = (curl_perform_t)dlsym(handle, "curl_easy_perform");
    auto curl_cleanup = (curl_cleanup_t)dlsym(handle, "curl_easy_cleanup");
    
    if (!curl_init || !curl_setopt || !curl_perform || !curl_cleanup) {
        dlclose(handle);
        return;
    }
    
    void* curl = curl_init();
    if (curl) {
        std::string responseString;
        std::string postData = "completedLevels=" + completedStr;
        
        // CURLOPT_URL
        curl_setopt(curl, 10002, "http://neopointfour.host/database/getGJLevelStars.php");
        // CURLOPT_POSTFIELDS
        curl_setopt(curl, 10015, postData.c_str());
        // CURLOPT_WRITEFUNCTION
        curl_setopt(curl, 20011, WriteCallback);
        // CURLOPT_WRITEDATA
        curl_setopt(curl, 10001, &responseString);
        // CURLOPT_SSL_VERIFYPEER
        curl_setopt(curl, 64, 0L);
        // CURLOPT_TIMEOUT
        curl_setopt(curl, 13, 10L);
        
        int res = curl_perform(curl);
        
        if (res == 0) {
            parseAndRecalculateStars(responseString);
        } else {
        }
        
        curl_cleanup(curl);
    } else {
    }
    
    dlclose(handle);
}

#endif