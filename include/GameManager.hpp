#pragma once

#include <cocos2d.h>
#include "PlayLayer.hpp"

class GameRateDelegate {
	virtual void updateRate();
};

class GameManager : public cocos2d::CCNode {
public:
    static GameManager* sharedState();

    // IDA returns poop addresses

    // cocos2d::CCDictionary* m_scoreKeeper; // 0x130
    // cocos2d::CCDictionary* m_valueKeeper; // 0x134
    // void* m_premiumPopup; // 0x57
    // int m_playerFrame; // 0x5F
    // int m_playerColor; // 0x60
    // int m_playerColor2; // 0x61
    // GameRateDelegate* m_rateDelegate; // 0x65
    // bool m_bUnk1; // 0x12F
    // bool m_bUnusedBoomlingsMatchUpLeftover; // 0x13C
    // bool m_bDidSyncAchievements; // 0x14C
    // void* m_levelSelectLayer; // 0x154
    // bool m_bMainMenuActive; // 0x158
    // bool m_bGameCenterEnabled; // 0x160
    // bool m_bFirstSetup; // 0x161
    // bool m_bShowedFirstTutorial; // 0x162
    // std::string m_sPlayerUDID; // 0x164
    // std::string m_sPlayerName; // 0x168
    // bool m_bMusicEnabled; // 0x16C
    // bool m_bFxEnabled; // 0x16D
    // bool m_bDidRateGame; // 0x16E
    // bool m_bWasHigh; // 0x16F
    // bool m_bEditMode; // 0x170
    // void* m_LastScene; // 0x174
    // bool m_bReturnToSearch; // 0x178
    // bool m_bAutoCheckpoints; // 0x188
    // bool m_bShowSongMarkers; // 0x189
    // bool m_bShowBPMMarkers; // 0x18A
    // bool m_bRecordGameplay; // 0x18B
    // bool m_bAutoRetry; // 0x18C
    // bool m_bClickedGarage; // 0x18D
    // bool m_bClickedEditor; // 0x18E
    // bool m_bClickedName; // 0x18F
    // bool m_bShowedEditorGuide; // 0x190
    // bool m_bkEnableTutorial; // 0x198 <-- getFullUnlocked
};
