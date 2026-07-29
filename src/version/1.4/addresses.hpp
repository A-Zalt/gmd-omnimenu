/////////////////////////////////////////////////////////
//                         1.4                         //
/*******************************************************/
/*/                      OFFSETS                      /*/
/*******************************************************/
/**/ #define BoomScrollLayer__m_currentScreen  0x234 /**/ // BoomScrollLayer::getCurrentScreen
/**/ #define BoomScrollLayer__m_dynamic        0x154 /**/ // BoomScrollLayer::getTotalPages
/**/ #define BoomScrollLayer__m_dynamicPages   0x150 /**/ // BoomScrollLayer::getTotalPages
/**/ #define BoomScrollLayer__m_pages          0x164 /**/ // BoomScrollLayer::getTotalPages
/**/ #define CCMenuItemSpriteExtra__m_ogScale  0x150 /**/ // CCMenuItemSpriteExtra::setOriginalScale
/**/ #define CCTextInputNode__m_charLimit      0x170 /**/ // CCTextInputNode::getCharLimit
/**/ #define CCTextInputNode__m_inputDelegate  0x16c /**/ // CCTextInputNode::getDelegate
/**/ #define CCTextInputNode__m_textField      0x168 /**/ // CCTextInputNode::getTextField
/**/ #define CCEGLViewProtocol__m_screenBottom  0xe4 /**/ // cocos2d::CCEGLViewProtocol::getScreenBottom
/**/ #define CheckpointObject__m_isFlipped     0x158 /**/ // CheckpointObject::getIsFlipped
/**/ #define CheckpointObject__m_playerPos     0x12c /**/ // CheckpointObject::getPlayerPos
/**/ #define ColorPickerPopup__m_colorWheel    0x1cc /**/ // ColorPickerPopup::selectColor
/**/ #define ColorSelectPopup__m_colorWheel    0x1cc /**/ // ColorSelectPopup::selectColor
/**/ #define ColorSelectPopup__m_duration      0x1e4 /**/ // ColorSelectPopup::sliderChanged
/**/ #define ColorSelectPopup__m_durLabel      0x1d4 /**/ // ColorSelectPopup::updateDurLabel
/**/ #define ColorSelectPopup__m_slider        0x1d8 /**/ // ColorSelectPopup::init
/**/ #define EditButtonBar__m_buttons          0x12c /**/ // EditButtonBar::init
/**/ #define EditLevelLayer__m_level           0x148 /**/ // EditLevelLayer::init
/**/ #define EditorPauseLayer__m_editorLayer   0x1c8 /**/ // EditorPauseLayer::init
/**/ #define EditorUI__m_buttonMenu            0x184 /**/ // EditorUI::setupDeleteMenu
/**/ #define EditorUI__m_createButtonBar       0x144 /**/ // EditorUI::setupCreateMenu (at the bottom)
/**/ #define EditorUI__m_createButtons         0x1bc /**/ // EditorUI::getCreateBtn (at the bottom)
/**/ #define EditorUI__m_editButtonBar         0x148 /**/ // EditorUI::createMoveMenu (at the bottom)
/**/ #define EditorUI__m_editorLayer           0x1c4 /**/ // EditorUI::init
/**/ #define EditorUI__m_selectedObject        0x248 /**/ // EditorUI::selectObject
/**/ #define EditorUI__m_selectedObjects       0x180 /**/ // EditorUI::deselectAll 
/**/ #define EditorUI__m_touch                 0x228 /**/ // EditorUI::onDelete
/**/ #define EditorUI__m_unkFloat              0x158 /**/ // EditorUI::setupCreateMenu (at the bottom)
/**/ #define GameLevelManager__m_settingsDict  0x12c /**/ // GameLevelManager::setBoolForKey
/**/ #define GameManager__m_autoRetry          0x194 /**/ // GameManager::getAutoRetryLevel
/**/ #define GameManager__m_playLayer          0x150 /**/ // GameManager::getPlayLayer
/**/ #define GameManager__m_playerName         0x168 /**/ // GameManager::getPlayerName

/**/ #define GameManager__m_playerShip         0x184 /**/ // <--- needed for 1.4 only - GameManager::getPlayerShip

/**/ #define GameManager__m_playerUDID         0x164 /**/ // GameManager::getPlayerUDID
/**/ #define GameObject__m_objectKey           0x328 /**/ // GameObject::getObjectKey
/**/ #define GameObject__m_particles           0x288 /**/ // GameObject::setOpacity

/**/ #define GameObject__m_radius              0x2b4 /**/ // GameObject::getRadius

/**/ #define GameObject__m_realPosition        0x2d4 /**/ // GameObject::getRealPosition
/**/ #define GameObject__m_sectionIdx          0x2cc /**/ // GameObject::getSectionIdx
/**/ #define GameObject__m_shouldSpawn         0x2d0 /**/ // GameObject::getShouldSpawn
/**/ #define GameObject__m_type                0x2c8 /**/ // GameObject::getType
/**/ #define GameObject__m_useAudioScale       0x2fc /**/ // GameObject::getUseAudioScale
/**/ #define GameStatsManager__m_completedDict 0x12c /**/ // GameStatsManager::completedLevel
/**/ #define GameStatsManager__m_statsDict     0x128 /**/ // GameStatsManager::getStat
/**/ #define GJComment__m_commentID            0x130 /**/ // GJComment::getCommentID
/**/ #define GJSearchObject__m_page            0x138 /**/ // GJSearchObject::getPage
/**/ #define GJSearchObject__m_type            0x128 /**/ // GJSearchObject::getType
/**/ #define InfoLayer__m_mainLayer            0x1c8 /**/ // InfoLayer::init
/**/ #define LeaderboardsLayer__m_boardType    0x148 /**/ // LeaderboardsLayer::selectLeaderboard
/**/ #define LevelBrowserLayer__m_searchObject 0x154 /**/ // LevelBrowserLayer::setSearchObject
/**/ #define LevelBrowserLayer__m_total        0x15c /**/ // LevelBrowserLayer::updateLevelsLabel
/**/ #define LevelCell__m_level                0x180 /**/ // LevelCell::loadFromLevel
/**/ #define LevelEditorLayer__m_batchNode     0x140 /**/ // LevelEditorLayer::createObject (1st function call after GameObject::createObject)
/**/ #define LevelEditorLayer__m_gameLayer     0x158 /**/ // LevelEditorLayer::getGameLayer
/**/ #define LevelEditorLayer__m_gridLayer     0x150 /**/ // LevelEditorLayer::removeObject
/**/ #define LevelEditorLayer__m_level         0x15c /**/ // LevelEditorLayer::getLevel
/**/ #define LevelEditorLayer__m_objectCount   0x154 /**/ // LevelEditorLayer::getObjectCount
/**/ #define LevelEditorLayer__m_sections      0x144 /**/ // LevelEditorLayer::getSectionCount
/**/ #define LevelEditorLayer__m_settings      0x160 /**/ // LevelEditorLayer::getLevelSettings
/**/ #define LevelEditorLayer__m_redoArray     0x14c /**/ // LevelEditorLayer::addToRedoList
/**/ #define LevelInfoLayer__m_level           0x154 /**/ // LevelInfoLayer::init
/**/ #define LevelSelectLayer__m_scrollLayer   0x14c /**/ // LevelSelectLayer::onNext (param in BoomScrollLayer::quickUpdate)
/**/ #define LevelSettingsObject__m_audioTrack 0x130 /**/ // LevelSettingsObject::getAudioTrack
/**/ #define LocalLevelManager__m_localLevels  0x130 /**/ // LocalLevelManager::getLocalLevels
/**/ #define PlayLayer__m_attempts             0x29c /**/ // PlayLayer::getAttempts
/**/ #define PlayLayer__m_attemptLabel         0x1e8 /**/ // PlayLayer::updateAttempts (assigned to a var right after the if statement)
/**/ #define PlayLayer__m_cameraPos            0x244 /**/ // PlayLayer::getCameraPos
/**/ #define PlayLayer__m_checkpoints          0x154 /**/ // PlayLayer::removeLastCheckpoint
/**/ #define PlayLayer__m_clkTimer             0x2a4 /**/ // PlayLayer::getClkTimer
/**/ #define PlayLayer__m_hazards              0x188 /**/ // PlayLayer::checkCollisions (the for loop at the bottom which checks for rect intersections and calls destroyPlayer)
/**/ #define PlayLayer__m_isFlipped            0x22c /**/ // PlayLayer::getIsFlipped
/**/ #define PlayLayer__m_isPractice           0x265 /**/ // PlayLayer::getPracticeMode
/**/ #define PlayLayer__m_jumps                0x2a0 /**/ // PlayLayer::getJumps
/**/ #define PlayLayer__m_lastX                0x1e0 /**/ // PlayLayer::destroyPlayer (find the multiplication by 100, and the denominator in the division right before it is this field)
/**/ #define PlayLayer__m_level                0x240 /**/ // PlayLayer::getLevel
/**/ #define PlayLayer__m_pGlitter             0x1a0 /**/ // PlayLayer::toggleGlitter
/**/ #define PlayLayer__m_playerObject         0x23c /**/ // PlayLayer::getPlayer
/**/ #define PlayLayer__m_sections             0x184 /**/ // PlayLayer::removeObjectFromSection
/**/ #define PlayLayer__m_shouldRunDelayReset  0x2aa /**/ // PlayLayer::delayedResetLevel
/**/ #define PlayLayer__m_startPos             0x27c /**/ // PlayLayer::getStartPos
/**/ #define PlayLayer__m_uiLayer              0x238 /**/ // PlayLayer::getUILayer
/**/ #define PlayLayer__m_unkPrac              0x27b /**/ // PlayLayer::togglePracticeMode (set to 1 at the bottom)
/**/ #define PlayerObject__m_gravity           0x370 /**/ // PlayerObject::gravityUp

/**/ #define PlayerObject__m_gravityFlipped    0x406 /**/ // <--- needed for 1.4 only - PlayerObject::flipGravity
/**/ #define PlayerObject__m_flyMode           0x404 /**/ // <--- needed for 1.4 only - PlayerObject::toggleFlyMode

/**/ #define PlayerObject__m_onGround          0x37d /**/ // PlayerObject::pushButton (if statement that updateJump is called in)

/**/ #define PlayerObject__m_playerScale       0x408 /**/ // <--- needed for 1.4 only - PlayerObject::getPlayerScale

/**/ #define PlayerObject__m_pGround           0x3e0 /**/ // PlayerObject::deactivateParticle
/**/ #define PlayerObject__m_pShipGround       0x3ec /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pShipFire         0x3e4 /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pShipLift         0x3e8 /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pLanding          0x3f4 /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_pLanding2         0x3f8 /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_unk1              0x388 /**/ // PlayerObject::playerDestroyed (at the top)
/**/ #define PlayerObject__m_unk2              0x407 /**/ // PlayerObject::playerDestroyed (slightly below, set to 1)
/**/ #define PlayerObject__m_xVelocity         0x360 /**/ // PlayerObject::speedUp
/**/ #define PlayerObject__m_yStart            0x368 /**/ // PlayerObject::yStartUp
/**/ #define UILayer__m_checkpointMenu         0x1b4 /**/ // UILayer::toggleCheckpointsMenu
/*******************************************************/
/*/                     ADDRESSES                     /*/
/*******************************************************/
/**/ #define _0_1f                          0x322AEB /**/ // string search: %0.1f, the "1"
/**/ #define basicstring_assign             0x2E87D0 /**/ // differs between versions :/
/**/ #define global_order_of_arrival        0x4544CC /**/ // CCNode::reorderChild
/**/ #define iconkit_colors                 0x176DC0 /**/
/**/ #define iconkit_colors2                0x176CA4 /**/
/**/ #define iconkit_colors3                0x176C98 /**/
/**/ #define __GameManager_isColorUnlocked  0x144B74 /**/ // fuck dobbyhook fuck this stupid game fuck everyone kill 7 billion people
/**/ #define __GameManager_isIconUnlocked   0x144B04 /**/ // fuck dobbyhook fuck this stupid game fuck everyone kill 7 billion people
/**/ #define move_x_max                     0x159174 /**/ // EditorUI::moveObject - 00 60 EA 46
/**/ #define move_y_max                     0x15917C /**/ // EditorUI::moveObject - 00 40 A1 44
/**/ #define move_y_min                     0x159178 /**/ // EditorUI::moveObject - 00 00 B6 42
/**/ #define object_limit                   0x15BEA8 /**/ // EditorUI::onCreate
/**/ #define pause_edit_button              0x15D69E /**/ // PauseLayer::customSetup
/**/ #define pause_restart_button           0x15D758 /**/ // PauseLayer::customSetup
/**/ #define place_x_max                    0x15BCB4 /**/ // EditorUI::onCreateObject - 00 42 EA 46
/**/ #define place_y_max                    0x15BCB8 /**/ // EditorUI::onCreateObject - 00 60 9F 44
/**/ #define place_x_min                    0x15BCAC /**/ // EditorUI::onCreateObject - 00 80 9D 43
/**/ #define place_y_min                    0x15BCB0 /**/ // EditorUI::onCreateObject - 00 00 D2 42
/**/ #define search_query_length_fix        0x117822 /**/ // BLX sprintf, GameToolbox::getResponse, "HTTP Status Code: %d, tag = %s"
/**/ #define zoom_bypass_max_1              0x1594D4 /**/ // EditorUI::zoomGameLayer
/**/ #define zoom_bypass_max_2              0x15960E /**/ // EditorUI::zoomGameLayer
/**/ #define zoom_bypass_min                0x15961C /**/ // EditorUI::zoomGameLayer
/*******************************************************/