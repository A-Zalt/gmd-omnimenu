#pragma once

#include <cocos2d.h>
#include "PlayerObject.hpp"
#include "DS_Dictionary.hpp"

typedef enum {
    Local = 1,
    Editor = 2,
    Saved = 3 
} GJLevelType;

class GJGameLevel : public cocos2d::CCNode {
public:
#if GAME_VERSION >= GV_1_7
    cocos2d::CCDictionary* m_obLastBuildSave;
#endif
    int m_nLevelID; // 0x128
    std::string m_sLevelName; // 0x12c
    std::string m_sLevelDesc; // 0x130
    std::string m_sLevelString; // 0x134
    std::string m_sUserName; // 0x138
#if GAME_VERSION >= GV_1_6
    std::string m_sRecordString; // 0x13c
#endif
#if GAME_VERSION >= GV_1_7
    std::string m_sUploadDate;
    std::string m_sUpdateDate;
#endif
    int m_nUserID; // 0x140
    int m_nDifficulty; // 0x144
    int m_nAudioTrack; // 0x148
    int m_nRatings; // 0x14c
    int m_nRatingsSum; // 0x150
    int m_nDownloads; // 0x154
    int m_nCompletes; // 0x158
    bool m_bIsEditable; // 0x15c
    bool m_bIsVerified; // 0x15d
    bool m_bIsUploaded; // 0x15e
    int m_nLevelVersion; // 0x160
    int m_nGameVersion; // 0x164
#if GAME_VERSION >= GV_1_7
    int m_nBuildVersion;
#endif
    int m_nAttempts; // 0x168
#if GAME_VERSION >= GV_1_6
    int m_nJumps; // 0x16c
#endif
    int m_nNormalPercent; // 0x170
    int m_nPracticePercent; // 0x174
    int m_nLikes; // 0x178
    int m_nDislikes; // 0x17c
    int m_nLevelLength; // 0x180
#if GAME_VERSION > GV_1_4
    int m_nFeatureScore; // 0x184
#endif
#if GAME_VERSION > GV_1_2
    bool m_bIsDemon; // 0x180
    int m_nStars; // 0x184
#endif
#if GAME_VERSION >= GV_1_6
    int m_bAutoLevel; // 0x190
    int m_nCoins; // 0x194
#if GAME_VERSION >= GV_1_7
    int m_nLevelSize;
    int m_nPassword;
    int m_nFailedPasswordAttempts;
#endif
    int m_nStarRatings; // 0x198
    int m_nStarRatingsSum; // 0x19c
    int m_nMaxStarRatings; // 0x1a0
    int m_nMinStarRatings; // 0x1a4
    int m_nDemonVotes; // 0x1a8
#endif
#if GAME_VERSION >= GV_1_5
    bool m_bDontSave; // 0x188, 0x1ac
#endif
#if GAME_VERSION >= GV_1_6
    bool m_bIsHidden; // 0x1ad
    int m_nRequiredCoins; // 0x1b0
    bool m_bIsUnlocked; // 0x1b4
#endif
    cocos2d::CCPoint m_obLastCameraPos; // 0x1b8
    float m_fLastEditorZoom; // 0x1d8
#if GAME_VERSION >= GV_1_7
    int m_nLastBuildTab;
#endif
#if GAME_VERSION >= GV_1_5
    int m_nLastBuildPage; // 0x1dc
#endif
    GJLevelType m_eLevelType; // 0x1e0
    int m_nM_ID; // 0x1e4

    static GJGameLevel* create();
    static GJGameLevel* create(cocos2d::CCDictionary dict);
    static GJGameLevel* createWithCoder(DS_Dictionary* dict);
    static int getLengthKey(int seconds);
    static std::string lengthKeyToString(int key);
    virtual bool init();
    virtual void setIsVerified(bool isVerified);
    int getAverageDifficulty();
};
