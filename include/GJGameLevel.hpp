/*

CLASS(ES): GJGameLevel, GJLevelType
VERSION: 1.22

*/

#pragma once

#include <cocos2d.h>
#include "PlayerObject.hpp"

typedef enum {
    Local = 1,
    Editor = 2,
    Saved = 3 
} GJLevelType;

class GJGameLevel : cocos2d::CCNode {
public:
    int m_nLevelID; // 0x128
    std::string m_sLevelName; // 0x12c
    std::string m_sLevelDesc; // 0x130
    std::string m_sLevelString; // 0x134
    std::string m_sUserName; // 0x138
    int m_nUserID; // 0x13c
    int m_nDifficulty; // 0x140
    int m_nAudioTrack; // 0x144
    int m_nRatings; // 0x148
    int m_nRatingsSum; // 0x14c
    int m_nDownloads; // 0x150
    int m_nCompletes; // 0x154
    bool m_bIsEditable; // 0x158
    bool m_bIsVerified; // 0x159
    bool m_bIsUploaded; // 0x15a
    int m_nLevelVersion; // 0x15c
    int m_nGameVersion; // 0x160
    int m_nAttempts; // 0x164
    int m_nNormalPercent; // 0x168
    int m_nPracticePercent; // 0x16c
    int m_nLikes; // 0x170
    int m_nDislikes; // 0x174
    int m_nLevelLength; // 0x178
    cocos2d::CCPoint m_obLastCameraPos; // 0x17c
    float m_fLastEditorZoom; // 0x19c
    GJLevelType m_eLevelType; // 0x1a0
    int m_nM_ID; // 0x1a4

    static GJGameLevel* create();
    static GJGameLevel* create(cocos2d::CCDictionary dict);
    virtual bool init();
    virtual void setIsVerified(bool isVerified);
};
