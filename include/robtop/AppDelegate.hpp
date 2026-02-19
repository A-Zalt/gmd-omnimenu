#pragma once
#include <cocos2d.h>

class AppDelegate : public cocos2d::CCApplicationProtocol {
public:
    static AppDelegate* get();
    void trySaveGame();
};