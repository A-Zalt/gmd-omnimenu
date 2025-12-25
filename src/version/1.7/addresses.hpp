/////////////////////////////////////////////////////////
//                         1.7                         //
/*******************************************************/
/*/                      OFFSETS                      /*/
/*******************************************************/
/**/ #define BoomScrollLayer__m_currentScreen  0x18c /**/ // BoomScrollLayer::getCurrentScreen
/**/ #define BoomScrollLayer__m_dynamic        0x124 /**/ // BoomScrollLayer::getTotalPages
/**/ #define BoomScrollLayer__m_dynamicPages   0x120 /**/ // BoomScrollLayer::getTotalPages
/**/ #define BoomScrollLayer__m_pages          0x134 /**/ // BoomScrollLayer::getTotalPages
/**/ #define CCMenuItemSpriteExtra__m_ogScale  0x118 /**/ // CCMenuItemSpriteExtra::setOriginalScale
/**/ #define CCTextInputNode__m_charLimit      0x140 /**/ // CCTextInputNode::getCharLimit
/**/ #define CCTextInputNode__m_inputDelegate  0x13c /**/ // CCTextInputNode::getDelegate
/**/ #define CCTextInputNode__m_textField      0x138 /**/ // CCTextInputNode::getTextField
/**/ #define CCEGLViewProtocol__m_screenBottom  0x30 /**/ // cocos2d::CCEGLViewProtocol::getScreenBottom
/**/ #define CheckpointObject__m_playerPos      0xec /**/ // CheckpointObject::getPlayerPos
/**/ #define ColorPickerPopup__m_colorWheel    0x1ac /**/ // ColorPickerPopup::selectColor
/**/ #define ColorSelectPopup__m_colorWheel    0x1a4 /**/ // ColorSelectPopup::selectColor

/**/ #define DrawGridLayer__m_speedObjects     0x124 /**/ // DrawGridLayer::addToSpeedObjects

/**/ #define EditButtonBar__m_pages             0xec /**/ // EditButtonBar::init
/**/ #define EditLevelLayer__m_level           0x118 /**/ // EditLevelLayer::init
/**/ #define EditorPauseLayer__m_editorLayer   0x1a4 /**/ // EditorPauseLayer::init
/**/ #define EditorUI__m_buttonMenu            0x13c /**/ // EditorUI::setupDeleteMenu
/**/ #define EditorUI__m_createButtonBar       0x114 /**/ // EditorUI::setupCreateMenu (at the bottom)
/**/ #define EditorUI__m_createButtonBars      0x174 /**/ // EditorUI::setupCreateMenu (at the bottom)
/**/ #define EditorUI__m_createButtons         0x18c /**/ // EditorUI::getCreateBtn (at the bottom)
/**/ #define EditorUI__m_editButtonBar         0x118 /**/ // EditorUI::createMoveMenu (at the bottom)
/**/ #define EditorUI__m_editorLayer           0x194 /**/ // EditorUI::init
/**/ #define EditorUI__m_selectedObject        0x1b8 /**/ // EditorUI::selectObject
/**/ #define EditorUI__m_selectedObjects       0x138 /**/ // EditorUI::deselectAll 
/**/ #define EditorUI__m_unkFloat              0x128 /**/ // EditorUI::setupCreateMenu

/**/ #define EditorUI__m_zoomOutButton         0x158 /**/ // EditorUI::init

/**/ #define GameManager__m_autoRetry          0x16c /**/ // GameManager::getAutoRetryLevel
/**/ #define GameManager__m_playLayer          0x114 /**/ // GameManager::getPlayLayer
/**/ #define GameManager__m_playerName         0x12c /**/ // GameManager::getPlayerName
/**/ #define GameManager__m_playerUDID         0x128 /**/ // GameManager::getPlayerUDID

/**/ #define GameManager__m_showProgressBar    0x16d /**/ // GameManager::getShowProgressBar

/**/ #define GameObject__m_objectKey           0x284 /**/ // GameObject::getObjectKey
/**/ #define GameObject__m_particles           0x1e0 /**/ // GameObject::setOpacity
/**/ #define GameObject__m_realPosition        0x244 /**/ // GameObject::getRealPosition
/**/ #define GameObject__m_sectionIdx          0x23c /**/ // GameObject::getSectionIdx
/**/ #define GameObject__m_shouldSpawn         0x240 /**/ // GameObject::getShouldSpawn
/**/ #define GameObject__m_type                0x238 /**/ // GameObject::getType
/**/ #define GameObject__m_useAudioScale       0x254 /**/ // GameObject::getUseAudioScale
/**/ #define GJComment__m_commentID             0xf0 /**/ // GJComment::getCommentID

/**/ #define GJSearchObject__m_type             0xe8 /**/ // GJSearchObject::getType
/**/ #define LeaderboardsLayer__m_boardType    0x118 /**/ // LeaderboardsLayer::selectLeaderboard
/**/ #define LevelBrowserLayer__m_searchObject 0x124 /**/ // LevelBrowserLayer::setSearchObject

/**/ #define LevelCell__m_level                0x150 /**/ // LevelCell::loadFromLevel
/**/ #define LevelEditorLayer__m_batchNode     0x114 /**/ // LevelEditorLayer::createObject (1st function call after GameObject::createObject)
/**/ #define LevelEditorLayer__m_gameLayer     0x12c /**/ // LevelEditorLayer::getGameLayer
/**/ #define LevelEditorLayer__m_gridLayer     0x128 /**/ // LevelEditorLayer::getGridLayer

/**/ #define LevelEditorLayer__m_level         0x130 /**/ // LevelEditorLayer::getLevel

/**/ #define LevelEditorLayer__m_objectCount   0x124 /**/ // LevelEditorLayer::getObjectCount
/**/ #define LevelEditorLayer__m_sections      0x118 /**/ // LevelEditorLayer::getSectionCount
/**/ #define LevelEditorLayer__m_settings      0x134 /**/ // LevelEditorLayer::getLevelSettings
/**/ #define LevelEditorLayer__m_redoArray     0x120 /**/ // LevelEditorLayer::addToRedoList

/**/ #define LevelSettingsObject__m_startSpeed 0x110 /**/ // LevelSettingsObject::getStartSpeed

/**/ #define LevelInfoLayer__m_level           0x130 /**/ // LevelInfoLayer::init
/**/ #define LevelSelectLayer__m_scrollLayer   0x11c /**/ // LevelSelectLayer::onNext (param in BoomScrollLayer::quickUpdate)
/**/ #define LocalLevelManager__m_localLevels   0xf4 /**/ // LocalLevelManager::getLocalLevels
/**/ #define PlayLayer__m_attempts             0x27c /**/ // PlayLayer::getAttempts
/**/ #define PlayLayer__m_attemptLabel         0x1c4 /**/ // PlayLayer::updateAttempts (assigned to a var right after the if statement)
/**/ #define PlayLayer__m_checkpoints          0x12c /**/ // PlayLayer::removeLastCheckpoint
/**/ #define PlayLayer__m_clkTimer             0x288 /**/ // PlayLayer::getClkTimer
/**/ #define PlayLayer__m_hazards              0x168 /**/ // PlayLayer::checkCollisions (the for loop at the bottom which checks for rect intersections and calls destroyPlayer)
/**/ #define PlayLayer__m_isPractice           0x259 /**/ // PlayLayer::getPracticeMode
/**/ #define PlayLayer__m_jumps                0x280 /**/ // PlayLayer::getJumps
/**/ #define PlayLayer__m_lastX                0x1bc /**/ // PlayLayer::destroyPlayer (find the multiplication by 100, and the denominator in the division right before it is this field)
/**/ #define PlayLayer__m_level                0x24c /**/ // PlayLayer::getLevel
/**/ #define PlayLayer__m_pGlitter             0x180 /**/ // PlayLayer::toggleGlitter
/**/ #define PlayLayer__m_playerObject         0x248 /**/ // PlayLayer::getPlayer
/**/ #define PlayLayer__m_sections             0x164 /**/ // PlayLayer::removeObjectFromSection
/**/ #define PlayLayer__m_startPos             0x274 /**/ // PlayLayer::getStartPos
/**/ #define PlayLayer__m_uiLayer              0x244 /**/ // PlayLayer::getUILayer
/**/ #define PlayLayer__m_unkPrac              0x270 /**/ // PlayLayer::togglePracticeMode (set to 1 at the bottom)
/**/ #define PlayerObject__m_gravity           0x300 /**/ // PlayerObject::gravityUp
/**/ #define PlayerObject__m_onGround          0x30d /**/ // PlayerObject::pushButton (if statement that updateJump is called in)
/**/ #define PlayerObject__m_pGround           0x354 /**/ // PlayerObject::deactivateParticle
/**/ #define PlayerObject__m_pShipGround       0x360 /**/ // PlayerObject::playerDestroyed (third stopSystem)
/**/ #define PlayerObject__m_pShipFire         0x358 /**/ // PlayerObject::playerDestroyed (first stopSystem)
/**/ #define PlayerObject__m_pShipLift         0x35c /**/ // PlayerObject::playerDestroyed (second stopSystem)
/**/ #define PlayerObject__m_pLanding          0x36c /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_pLanding2         0x370 /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_unk1              0x318 /**/ // PlayerObject::playerDestroyed (at the top)
/**/ #define PlayerObject__m_unk2              0x384 /**/ // PlayerObject::playerDestroyed (slightly below, set to 1)
/**/ #define PlayerObject__m_xVelocity         0x2f0 /**/ // PlayerObject::speedUp
/**/ #define PlayerObject__m_yStart            0x2f8 /**/ // PlayerObject::yStartUp
/**/ #define UILayer__m_checkpointMenu         0x18c /**/ // UILayer::toggleCheckpointsMenu
/*******************************************************/
/*/                     ADDRESSES                     /*/
/*******************************************************/
/**/ #define basicstring_assign             0x3A1BB8 /**/ // differs between versions :/
/**/ #define global_order_of_arrival        0x4AFB34 /**/ // CCNode::reorderChild
/**/ #define __CCTextInputNode_setCharLimit 0x1726EE /**/
/**/ #define __CCTextInputNode_setProfanity 0x172700 /**/ // CCTextInputNode::setProfanityFilter
/**/ #define __GameManager_isColorUnlocked  0x184560 /**/ // fuck dobbyhook fuck this stupid game fuck everyone kill 7 billion people
/**/ #define __GameManager_isIconUnlocked   0x184460 /**/ // fuck dobbyhook fuck this stupid game fuck everyone kill 7 billion people
/**/ #define __GJGameLevel_setIsVerified    0x1AC23E /**/
/**/ #define clubstep_bypass                0x1BAC9F /**/ // LevelPage::onPlay
/**/ #define __LevelEditorLayer_update_v    0x486664 /**/ // LevelEditorLayer::~
/**/ #define move_x_min                     0x19B8A0 /**/ // EditorUI::moveObject - 00 80 96 43
/**/ #define move_x_max                     0x19B8A4 /**/ // EditorUI::moveObject - 00 60 EA 46
/**/ #define move_y_max                     0x19B8AC /**/ // EditorUI::moveObject - 00 C0 C6 44
/**/ #define move_y_min                     0x19B8A8 /**/ // EditorUI::moveObject - 00 00 B6 42
/**/ #define object_limit                   0x19C9B3 /**/ // EditorUI::onCreate
/**/ #define object_limit_duplicate         0x19B3F5 /**/ // EditorUI::onDuplicate
/**/ #define pause_edit_button              0x1A0702 /**/ // PauseLayer::customSetup
/**/ #define pause_restart_button           0x1A0840 /**/ // PauseLayer::customSetup
/**/ #define place_x_max                    0x19C95C /**/ // EditorUI::onCreateObject - 00 B9 2F 47
/**/ #define place_y_max                    0x19C960 /**/ // EditorUI::onCreateObject - 00 60 9F 44
/**/ #define place_x_min                    0x19C954 /**/ // EditorUI::onCreateObject - 00 80 9D 43
/**/ #define place_y_min                    0x19C958 /**/ // EditorUI::onCreateObject - 00 00 D2 42
/**/ #define zoom_bypass_max                0x19BE9E /**/ // EditorUI::zoomGameLayer
/**/ #define zoom_bypass_min                0x19BEB0 /**/ // EditorUI::zoomGameLayer
/*******************************************************/