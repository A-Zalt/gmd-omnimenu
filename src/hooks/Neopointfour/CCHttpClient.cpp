#ifndef NP4_CCHTTPCLIENT_CPP
#define NP4_CCHTTPCLIENT_CPP

#include "../hook.hpp"

GJGameLevel* currentUploadingLevel = nullptr;

void setCurrentUploadingLevel(GJGameLevel* level) {
    currentUploadingLevel = level;
}

std::vector<int> getCompletedLevelIDs() {
    std::vector<int> completedIDs;
    
    auto GSM = GameStatsManager::sharedState();
    
    auto completedDict = getCompletedDict(GSM);
    
    cocos2d::CCArray* keys = completedDict->allKeys();
    if (!keys) {
        return completedIDs;
    }
    
    for (unsigned int i = 0; i < keys->count(); i++) {
        cocos2d::CCString* key = static_cast<cocos2d::CCString*>(keys->objectAtIndex(i));
        if (!key) continue;
        
        const char* keyStr = key->getCString();
        if (!keyStr) continue;
        
        if (keyStr[0] == 'c' && keyStr[1] == '_') {
            int levelID = atoi(keyStr + 2);
            if (levelID > 0) {
                completedIDs.push_back(levelID);
            }
        }
    }
    
    return completedIDs;
}

std::vector<int> getCompletedMainLevels() {
    std::vector<int> completedIDs;
    auto glm = GameLevelManager::sharedState();
    
    for (int page = 1; page < 13; page++) {
        GJGameLevel* level = glm->getMainLevel(page);
        if (!level) {
            continue;
        }
        int percentage = level->m_nNormalPercent;
        if (percentage == 100) {
            completedIDs.push_back(page);
        }
    }
    
    return completedIDs;
}

std::vector<int> getCompletedMainLevelsPractice() {
    std::vector<int> completedIDs;
    auto glm = GameLevelManager::sharedState();
    
    for (int page = 1; page < 13; page++) {
        GJGameLevel* level = glm->getMainLevel(page);
        if (!level) {
            continue;
        }
        int percentage = level->m_nPracticePercent;
        if (percentage == 100) {
            completedIDs.push_back(page);
        }
    }
    
    return completedIDs;
}

void (*TRAM_CCHttpClient_send)(cocos2d::extension::CCHttpClient* self, cocos2d::extension::CCHttpRequest* request);
void CCHttpClient_send(cocos2d::extension::CCHttpClient* self, cocos2d::extension::CCHttpRequest* request) {
    const char* url = request->getUrl();
    HaxManager& hax = HaxManager::sharedState();
    if (currentUploadingLevel) {
        if (url && strstr(url, "uploadGJLevel.php") != nullptr) {
            if (hax.originalIDMap.find(currentUploadingLevel) != hax.originalIDMap.end()) {
                int originalID = hax.originalIDMap[currentUploadingLevel];
                if (originalID > 0) {
                    char* data = request->getRequestData();
                    int dataSize = request->getRequestDataSize();
                    
                    if (data && dataSize > 0) {
                        std::string postData(data, dataSize);
                        
                        std::stringstream ss;
                        ss << postData << "&original=" << originalID;
                        std::string newData = ss.str();
                        
                        request->setRequestData(newData.c_str(), newData.length());
                    } else {
                    }
                } else {
                }
            } else {
            }
        }
    } else {
    }

    if (url && strstr(url, "getGJScores.php") != nullptr) {
        char* data = request->getRequestData();
        int dataSize = request->getRequestDataSize();
        
        if (data && dataSize > 0) {
            std::string postData(data, dataSize);
            auto GLM = GameLevelManager::sharedState();
                      
            bool demonFilter = GLM->getBoolForKey("demon_filter");
                
            if (demonFilter) {
                postData += "&stat=1";
            }

            request->setRequestData(postData.c_str(), postData.length());
        } else {
        }
    }
    
    if (url && strstr(url, "getGJLevels.php") != nullptr) {
        char* data = request->getRequestData();
        int dataSize = request->getRequestDataSize();
        
        if (data && dataSize > 0) {
            std::string postData(data, dataSize);
            auto GLM = GameLevelManager::sharedState();
            
            if (postData.find("type=5") == std::string::npos && postData.find("type=6") == std::string::npos) {
                bool uncompletedFilter = GLM->getBoolForKey("uncompleted_filter");
                bool completedFilter = GLM->getBoolForKey("completed_filter");
                bool songFilter = GLM->getBoolForKey("song_filter");
                bool noStarFilter = GLM->getBoolForKey("nostar_filter");
                
                if (uncompletedFilter) {
                    postData += "&uncompleted=1";
                }
                
                if (completedFilter) {
                    postData += "&onlyCompleted=1";
                }
                if(completedFilter || uncompletedFilter) {
                    std::vector<int> completedIDs = getCompletedLevelIDs();
                    if (!completedIDs.empty()) {
                        postData += "&completedLevels=(";
                        
                        for (size_t i = 0; i < completedIDs.size(); i++) {
                            if (i > 0) postData += ",";
                            std::stringstream ss;
                            ss << completedIDs[i];
                            postData += ss.str();
                        }
                        
                        postData += ")";
                    }
                }
                
                if (songFilter) {
                    auto dict = getSettingsDict(GLM);
                    std::string keyStr("selected_song_index");
                    auto value = dict->valueForKey(keyStr);
                    int songID = 0;
                    if (value) {
                        songID = value->intValue() + 1;
                    } else {
                    }
                    
                    std::stringstream ss;
                    ss << "&song=" << songID;
                    postData += ss.str();
                }
                
                if (noStarFilter) {
                    postData += "&noStar=1";
                }
                
                request->setRequestData(postData.c_str(), postData.length());
            } else {
            }
        } else {
        }
    }
    
    if (url && strstr(url, "updateGJUserScore.php") != nullptr) {
        char* data = request->getRequestData();
        int dataSize = request->getRequestDataSize();
        
        if (data && dataSize > 0) {
            std::string postData(data, dataSize);
            
            std::vector<int> completedIDs = getCompletedLevelIDs();
            if (!completedIDs.empty()) {
                postData += "&completedLevels=(";
                
                for (size_t i = 0; i < completedIDs.size(); i++) {
                    if (i > 0) postData += ",";
                    std::stringstream ss;
                    ss << completedIDs[i];
                    postData += ss.str();
                }
                
                postData += ")";
            }
            std::vector<int> completedMainIDs = getCompletedMainLevels();
            if (!completedMainIDs.empty()) {
                postData += "&completedMainLevels=(";
                
                for (size_t i = 0; i < completedMainIDs.size(); i++) {
                    if (i > 0) postData += ",";
                    std::stringstream ss;
                    ss << completedMainIDs[i];
                    postData += ss.str();
                }
                
                postData += ")";
            }

            std::vector<int> completedMainIDsPractice = getCompletedMainLevelsPractice();
            if (!completedMainIDsPractice.empty()) {
                postData += "&completedMainLevelsPractice=(";
                
                for (size_t i = 0; i < completedMainIDsPractice.size(); i++) {
                    if (i > 0) postData += ",";
                    std::stringstream ss;
                    ss << completedMainIDsPractice[i];
                    postData += ss.str();
                }
                
                postData += ")";
            }

            auto GSM = GameStatsManager::sharedState();
            auto statsDict = getStatsDict(GSM);

            auto jumpsValue = statsDict->valueForKey("1");
            auto attemptsValue = statsDict->valueForKey("2");

            if (jumpsValue && attemptsValue) {
                auto jumps = jumpsValue->intValue();
                auto attempts = attemptsValue->intValue();
                std::stringstream statsStream;
                statsStream << "&attempts=" << attempts << "&jumps=" << jumps;
                postData += statsStream.str();
            } else {
            }

            auto GM = GameManager::sharedState();
            auto ship = getPlayerShip(GM);
            std::stringstream shipStream;
            shipStream << "&accShip=" << ship;
            postData += shipStream.str();
            
            request->setRequestData(postData.c_str(), postData.length());
        } else {
        }
    }

    TRAM_CCHttpClient_send(self, request);
}

void NP4_CCHttpClient_om() {
    Omni::hook("_ZN7cocos2d9extension12CCHttpClient4sendEPNS0_13CCHttpRequestE",
        reinterpret_cast<void*>(CCHttpClient_send),
        reinterpret_cast<void**>(&TRAM_CCHttpClient_send));
}

#endif