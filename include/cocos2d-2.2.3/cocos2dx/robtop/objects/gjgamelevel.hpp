#pragma once
#ifndef _OBJECTS_GJGAMELEVEL_HPP
#define _OBJECTS_GJGAMELEVEL_HPP

#include <cocos2d.h>

class GJGameLevel : public cocos2d::CCNode {
    public:
        cocos2d::CCDictionary* m_lastBuildSave; // 0xe8
        int m_levelID; // 0xec
        std::string m_levelName; // 0xf0
        std::string m_levelDesc; // 0xf4
        std::string m_levelString; // 0xf8
        std::string m_userName; // 0xfc
        std::string m_recordString; // 0x100
        std::string m_uploadDate; // 0x104
        std::string m_updateDate; // 0x108
        int m_userID; // 0x10c
        int m_difficulty; // 0x110
        int m_audioTrack; // 0x114
        int m_ratings; // 0x118
        int m_ratingsSum; //0x11c
        int m_downloads; // 0x120
        int m_completes; // 0x124
        bool m_isEditable; // 0x128
        bool m_isVerified; // 0x129
        bool m_isUploaded; // 0x12a
        int m_levelVersion; // 0x12c
        int m_gameVersion; // 0x130
        int m_buildVersion; // 0x134
        int m_attempts; // 0x138
        int m_jumps; // 0x13c
        int m_normalPercent; // 0x140
        int m_practicePercent; // 0x144
        int m_likes; // 0x148
        int m_dislikes; // 0x14c
        int m_levelLength; // 0x150
        int m_featured; // 0x154
        bool m_demon; // 0x158
        int m_stars; // 0x15c
        bool m_autoLevel; // 0x160
        int m_coins; // 0x164
        int m_levelSize; // 0x168
        int m_password; // 0x16c
        int m_failedPasswordAttempts; // 0x170
        int m_starRatings; // 0x174
        int m_starRatingsSum; // 0x178
        int m_maxStarRatings; // 0x17c
        int m_minStarRatings; // 0x180
        int m_demonVotes; // 0x184
        bool m_dontSave; // 0x188
        bool m_isHidden; // 0x189
        int m_requiredCoins; // 0x18c
        bool m_isUnlocked; // 0x190
        cocos2d::CCPoint m_lastCameraPos; // 0x194
        float m_lastEditorZoom; // 0x19c
        int m_lastBuildTab; // 0x1a0
        int m_lastBuildPage; // 0x1a4
        GJLevelType m_levelType; // 0x1a8
        int m_m_ID; // 0x1ac
};

#endif
