// https://github.com/thelazycat2011/neopointfour/blob/main/jni/src/main.cpp

#include "../hook.hpp"

void (*TRAM_GJGameLevel_encodeWithCoder)(GJGameLevel* self, DS_Dictionary* dsd);
void GJGameLevel_encodeWithCoder(GJGameLevel* self, DS_Dictionary* dsd) {
    HaxManager& hax = HaxManager::sharedState();
    TRAM_GJGameLevel_encodeWithCoder(self, dsd);
    dsd->setIntegerForKey("thelazycat_featureType", hax.featureTypeMap[self]);
    dsd->setIntegerForKey("randomb_uploadTimestamp", hax.uploadTimestampMap[self]);
    dsd->setIntegerForKey("randomb_updateTimestamp", hax.updateTimestampMap[self]);
    dsd->setIntegerForKey("randomb_rateTimestamp", hax.rateTimestampMap[self]);
    dsd->setIntegerForKey("randomb_originalID", hax.originalIDMap[self]);
}

GJGameLevel* (*TRAM_GJGameLevel_createWithCoder)(DS_Dictionary* dsd);
GJGameLevel* GJGameLevel_createWithCoder(DS_Dictionary* dsd) {
    HaxManager& hax = HaxManager::sharedState();
    auto lvl = TRAM_GJGameLevel_createWithCoder(dsd);
    hax.featureTypeMap[lvl] = dsd->getIntegerForKey("thelazycat_featureType");
    hax.uploadTimestampMap[lvl] = dsd->getIntegerForKey("randomb_uploadTimestamp");
    hax.updateTimestampMap[lvl] = dsd->getIntegerForKey("randomb_updateTimestamp");
    hax.rateTimestampMap[lvl] = dsd->getIntegerForKey("randomb_rateTimestamp");
    hax.originalIDMap[lvl] = dsd->getIntegerForKey("randomb_originalID");
    return lvl;
}

GJGameLevel* (*TRAM_GJGameLevel_create)(CCDictionary* dict);
GJGameLevel* GJGameLevel_create(CCDictionary* dict) {
    HaxManager& hax = HaxManager::sharedState();
    auto lvl = TRAM_GJGameLevel_create(dict);
    auto uploadDate = dict->valueForKey("28")->intValue();
    auto updateDate = dict->valueForKey("29")->intValue();
    auto rateDate = dict->valueForKey("32")->intValue();
    auto originalID = dict->valueForKey("30");

    if (dict->valueForKey("42")->intValue() > 0) {
        hax.featureTypeMap[lvl] = 2;
    } else if (dict->valueForKey("19")->intValue() > 0) {
        hax.featureTypeMap[lvl] = 1;
    } else {
        hax.featureTypeMap[lvl] = 0;
    }

    if (uploadDate != 0) {
        hax.uploadTimestampMap[lvl] = uploadDate;
    }
    if (updateDate != 0) {
        hax.updateTimestampMap[lvl] = updateDate;
    }
    if (rateDate != 0) {
        hax.rateTimestampMap[lvl] = rateDate;
    }
    if (originalID->intValue() > 0) {
        hax.originalIDMap[lvl] = originalID->intValue();
    }

    return lvl;
}

void (*TRAM_GJGameLevel_destructor)(GJGameLevel*);
void GJGameLevel_destructor(GJGameLevel* self) {
    HaxManager& hax = HaxManager::sharedState();
    auto val = hax.featureTypeMap.find(self);
    if (val != hax.featureTypeMap.end()) hax.featureTypeMap.erase(val);
    
    auto val2 = hax.originalIDMap.find(self);
    if (val2 != hax.originalIDMap.end()) hax.originalIDMap.erase(val2);
    TRAM_GJGameLevel_destructor(self);
}

int GJGameLevel_getLengthKey(int endX){
    CCLog("getLengthKey - endX %d", endX);
    float seconds = (endX + 340) / 311.0f;
    int v3 = roundf(seconds);
            
    if (v3 <= 9) {
        return 0; // Tiny
    } else if (v3 <= 29) {
        return 1; // Short  
    } else if (v3 <= 59) {
        return 2; // Medium
    } else if (v3 <= 119) {
        return 3; // Long
    } else {
        return 4; // XL
    }
}

std::string GJGameLevel_lengthKeyToString(int key){
    switch (key) {
        case 1: return "Short";
        case 2: return "Medium";
        case 3: return "Long";
        case 4: return "XL";
        default: return "Tiny";
    }
}

void NP4_patch() {
    // Editor song selection
    DobbyCodePatch(reinterpret_cast<void*>(get_address(0x154500)), std::vector<uint8_t>({0x1f}).data(), 1);
    DobbyCodePatch(reinterpret_cast<void*>(get_address(0x1544a4)), std::vector<uint8_t>({0x1f}).data(), 1);
    DobbyCodePatch(reinterpret_cast<void*>(get_address(0x1544a8)), std::vector<uint8_t>({0x1f}).data(), 1);

    // Song info in LevelSelectLayer
    DobbyCodePatch(reinterpret_cast<void*>(get_address(0x182f08)), std::vector<uint8_t>({0x20}).data(), 1);
}

void NP4_GJGameLevel_om() {
    Omni::hook("_ZN11GJGameLevel15encodeWithCoderEP13DS_Dictionary",
        reinterpret_cast<void*>(GJGameLevel_encodeWithCoder),
        reinterpret_cast<void**>(&TRAM_GJGameLevel_encodeWithCoder));
    Omni::hook("_ZN11GJGameLevel15createWithCoderEP13DS_Dictionary",
        reinterpret_cast<void*>(GJGameLevel_createWithCoder),
        reinterpret_cast<void**>(&TRAM_GJGameLevel_createWithCoder));
    Omni::hook("_ZN11GJGameLevel6createEPN7cocos2d12CCDictionaryE",
        reinterpret_cast<void*>(GJGameLevel_create),
        reinterpret_cast<void**>(&TRAM_GJGameLevel_create));
    Omni::hook("_ZN11GJGameLevelD1Ev",
        reinterpret_cast<void*>(GJGameLevel_destructor),
        reinterpret_cast<void**>(&TRAM_GJGameLevel_destructor));
    Omni::hook("_ZN11GJGameLevel12getLengthKeyEi",
        reinterpret_cast<void*>(GJGameLevel_getLengthKey), nullptr);
    Omni::hook("_ZN11GJGameLevel17lengthKeyToStringEi",
        reinterpret_cast<void*>(GJGameLevel_lengthKeyToString), nullptr);

    NP4_patch();
}