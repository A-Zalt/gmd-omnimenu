#pragma once

#include <cocos2d.h>

class DS_Dictionary : public cocos2d::CCSprite {
public:
    DS_Dictionary();

    std::string saveRootSubDictToString();
    bool loadRootSubDictFromString(std::string);

    void saveRootSubDictToFile(const char*);
    void loadRootSubDictFromFile(const char*);

#if GDPS == GDPS_NEOPOINTFOUR
    void setIntegerForKey(const char*, int);
    int getIntegerForKey(const char*);
#endif
};
