#include "../hook.hpp"

void (*TRAM_CCDictionary_setObject)(void* dict, void* obj, const std::string& key);
void CCDictionary_setObject(void* dict, void* obj, const std::string& key) {
    TRAM_CCDictionary_setObject(dict, obj, key); // this fixes an assert error
}

void NP4_CCDictionary_om() {
    Omni::hook("_ZN7cocos2d12CCDictionary9setObjectEPNS_8CCObjectERKSs",
        reinterpret_cast<void*>(CCDictionary_setObject),
        reinterpret_cast<void**>(&TRAM_CCDictionary_setObject));
}