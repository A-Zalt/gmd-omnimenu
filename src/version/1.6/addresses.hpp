/////////////////////////////////////////////////////////
//                         1.6                         //
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
/**/ #define EditorUI__m_createButtons         0x1c8 /**/ // EditorUI::getCreateBtn (at the bottom)
/**/ #define EditorUI__m_editButtonBar         0x148 /**/ // EditorUI::createMoveMenu (at the bottom)
/**/ #define EditorUI__m_editorLayer           0x1d0 /**/ // EditorUI::init
/**/ #define EditorUI__m_selectedObject        0x254 /**/ // EditorUI::selectObject
/**/ #define EditorUI__m_selectedObjects       0x180 /**/ // EditorUI::deselectAll 
/**/ #define EditorUI__m_unkFloat              0x158 /**/ // EditorUI::setupCreateMenu (at the bottom)

/**/ #define EditorUI__m_zoomOutButton         0x1a0 /**/ // EditorUI::init

/**/ #define GameManager__m_autoRetry          0x1a4 /**/ // GameManager::getAutoRetryLevel
/**/ #define GameManager__m_playLayer          0x150 /**/ // GameManager::getPlayLayer
/**/ #define GameManager__m_playerName         0x168 /**/ // GameManager::getPlayerName
/**/ #define GameManager__m_playerUDID         0x164 /**/ // GameManager::getPlayerUDID

/**/ #define GameManager__m_showProgressBar    0x1a5 /**/ // GameManager::getShowProgressBar

/**/ #define GameObject__m_objectKey           0x32c /**/ // GameObject::getObjectKey
/**/ #define GameObject__m_particles           0x288 /**/ // GameObject::setOpacity

/**/ #define GameObject__m_radius              0x2b4 /**/ // GameObject::getRadius

/**/ #define GameObject__m_realPosition        0x2d4 /**/ // GameObject::getRealPosition
/**/ #define GameObject__m_sectionIdx          0x2cc /**/ // GameObject::getSectionIdx
/**/ #define GameObject__m_shouldSpawn         0x2d0 /**/ // GameObject::getShouldSpawn
/**/ #define GameObject__m_type                0x2c8 /**/ // GameObject::getType
/**/ #define GameObject__m_useAudioScale       0x2fc /**/ // GameObject::getUseAudioScale
/**/ #define GJComment__m_commentID            0x130 /**/ // GJComment::getCommentID

/**/ #define GJSearchObject__m_type            0x128 /**/ // GJSearchObject::getType
/**/ #define LeaderboardsLayer__m_boardType    0x148 /**/ // LeaderboardsLayer::selectLeaderboard
/**/ #define LevelBrowserLayer__m_searchObject 0x154 /**/ // LevelBrowserLayer::setSearchObject

/**/ #define LevelCell__m_level                0x180 /**/ // LevelCell::loadFromLevel
/**/ #define LevelEditorLayer__m_batchNode     0x140 /**/ // LevelEditorLayer::createObject (1st function call after GameObject::createObject)
/**/ #define LevelEditorLayer__m_gameLayer     0x158 /**/ // LevelEditorLayer::getGameLayer
/**/ #define LevelEditorLayer__m_gridLayer     0x154 /**/ // LevelEditorLayer::removeObject
/**/ #define LevelEditorLayer__m_objectCount   0x150 /**/ // LevelEditorLayer::getObjectCount
/**/ #define LevelEditorLayer__m_sections      0x144 /**/ // LevelEditorLayer::getSectionCount
/**/ #define LevelEditorLayer__m_settings      0x160 /**/ // LevelEditorLayer::getLevelSettings
/**/ #define LevelEditorLayer__m_redoArray     0x14c /**/ // LevelEditorLayer::addToRedoList
/**/ #define LevelInfoLayer__m_level           0x154 /**/ // LevelInfoLayer::init
/**/ #define LevelSelectLayer__m_scrollLayer   0x14c /**/ // LevelSelectLayer::onNext (param in BoomScrollLayer::quickUpdate)
/**/ #define LocalLevelManager__m_localLevels  0x130 /**/ // LocalLevelManager::getLocalLevels
/**/ #define PlayLayer__m_attempts             0x2d8 /**/ // PlayLayer::getAttempts
/**/ #define PlayLayer__m_attemptLabel         0x1e4 /**/ // PlayLayer::updateAttempts (assigned to a var right after the if statement)
/**/ #define PlayLayer__m_cameraPos            0x27c /**/ // PlayLayer::getCameraPos
/**/ #define PlayLayer__m_checkpoints          0x158 /**/ // PlayLayer::removeLastCheckpoint
/**/ #define PlayLayer__m_clkTimer             0x2e4 /**/ // PlayLayer::getClkTimer
/**/ #define PlayLayer__m_hazards              0x18c /**/ // PlayLayer::checkCollisions (the for loop at the bottom which checks for rect intersections and calls destroyPlayer)
/**/ #define PlayLayer__m_isFlipped            0x264 /**/ // PlayLayer::getIsFlipped
/**/ #define PlayLayer__m_isPractice           0x29d /**/ // PlayLayer::getPracticeMode
/**/ #define PlayLayer__m_jumps                0x2dc /**/ // PlayLayer::getJumps
/**/ #define PlayLayer__m_lastX                0x1dc /**/ // PlayLayer::destroyPlayer (find the multiplication by 100, and the denominator in the division right before it is this field)
/**/ #define PlayLayer__m_level                0x278 /**/ // PlayLayer::getLevel
/**/ #define PlayLayer__m_pGlitter             0x1a4 /**/ // PlayLayer::toggleGlitter
/**/ #define PlayLayer__m_playerObject         0x274 /**/ // PlayLayer::getPlayer
/**/ #define PlayLayer__m_progressBar          0x214 /**/ // PlayLayer::init - slidergroove2.png
/**/ #define PlayLayer__m_sections             0x188 /**/ // PlayLayer::removeObjectFromSection
/**/ #define PlayLayer__m_startPos             0x2b8 /**/ // PlayLayer::getStartPos
/**/ #define PlayLayer__m_uiLayer              0x270 /**/ // PlayLayer::getUILayer
/**/ #define PlayLayer__m_unkPrac              0x2b4 /**/ // PlayLayer::togglePracticeMode (set to 1 at the bottom)
/**/ #define PlayerObject__m_gravity           0x3a0 /**/ // PlayerObject::gravityUp
/**/ #define PlayerObject__m_onGround          0x3ad /**/ // PlayerObject::pushButton (if statement that updateJump is called in)
/**/ #define PlayerObject__m_pGround           0x410 /**/ // PlayerObject::deactivateParticle
/**/ #define PlayerObject__m_pShipGround       0x41c /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pShipFire         0x414 /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pShipLift         0x418 /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pLanding          0x428 /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_pLanding2         0x42c /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_unk1              0x3b8 /**/ // PlayerObject::playerDestroyed (at the top)
/**/ #define PlayerObject__m_unk2              0x43c /**/ // PlayerObject::playerDestroyed (slightly below, set to 1)
/**/ #define PlayerObject__m_xVelocity         0x390 /**/ // PlayerObject::speedUp
/**/ #define PlayerObject__m_yStart            0x398 /**/ // PlayerObject::yStartUp
/**/ #define UILayer__m_checkpointMenu         0x1b4 /**/ // UILayer::toggleCheckpointsMenu
/*******************************************************/
/*/                     ADDRESSES                     /*/
/*******************************************************/
/**/ #define _0_1f                          0x30140B /**/ // string search: %0.1f, the "1"
/**/ #define basicstring_assign             0x2B4854 /**/ // differs between versions :/
/**/ #define global_order_of_arrival        0x46E02C /**/ // CCNode::reorderChild
/**/ #define __CCTextInputNode_setCharLimit 0x12AD6E /**/
/**/ #define __CCTextInputNode_setProfanity 0x12AD80 /**/ // CCTextInputNode::setProfanityFilter
/**/ #define __GameManager_isColorUnlocked  0x13BE2C /**/ // fuck dobbyhook fuck this stupid game fuck everyone kill 7 billion people
/**/ #define __GameManager_isIconUnlocked   0x13BD8C /**/ // fuck dobbyhook fuck this stupid game fuck everyone kill 7 billion people
/**/ #define __GJGameLevel_setIsVerified    0x15DA4C /**/
/**/ #define clubstep_bypass                0x16A247 /**/ // LevelPage::onPlay
/**/ #define move_x_min                     0x14EE10 /**/ // EditorUI::moveObject - 00 80 96 43
/**/ #define move_x_max                     0x14EE14 /**/ // EditorUI::moveObject - 00 60 EA 46
/**/ #define move_y_max                     0x14EE1C /**/ // EditorUI::moveObject - 00 40 A1 44
/**/ #define move_y_min                     0x14EE18 /**/ // EditorUI::moveObject - 00 00 B6 42
/**/ #define object_limit                   0x150051 /**/ // EditorUI::onCreate
/**/ #define object_limit_duplicate         0x14E983 /**/ // EditorUI::onDuplicate
/**/ #define pause_edit_button              0x153260 /**/ // PauseLayer::customSetup
/**/ #define pause_restart_button           0x1533BC /**/ // PauseLayer::customSetup
/**/ #define place_x_max                    0x14FFFC /**/ // EditorUI::onCreateObject - 00 42 EA 46
/**/ #define place_y_max                    0x150000 /**/ // EditorUI::onCreateObject - 00 60 9F 44
/**/ #define place_x_min                    0x14FFF4 /**/ // EditorUI::onCreateObject - 00 80 9D 43
/**/ #define place_y_min                    0x14FFF8 /**/ // EditorUI::onCreateObject - 00 00 D2 42
/**/ #define zoom_bypass_max                0x14F566 /**/ // EditorUI::zoomGameLayer
/**/ #define zoom_bypass_min                0x14F578 /**/ // EditorUI::zoomGameLayer
/*******************************************************/