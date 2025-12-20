/////////////////////////////////////////////////////////
//                         1.1                         //
/*******************************************************/
/*/                      OFFSETS                      /*/
/*******************************************************/
/**/ #define BoomScrollLayer__m_currentScreen  0x234 /**/ // BoomScrollLayer::getCurrentScreen
/**/ #define BoomScrollLayer__m_dynamic        0x154 /**/ // BoomScrollLayer::getTotalPages
/**/ #define BoomScrollLayer__m_dynamicPages   0x150 /**/ // BoomScrollLayer::getTotalPages
/**/ #define BoomScrollLayer__m_pages          0x164 /**/ // BoomScrollLayer::getTotalPages
/**/ #define CCMenuItemSpriteExtra__m_ogScale  0x150 /**/ // CCMenuItemSpriteExtra::setOriginalScale

/**/ #define CCTextInputNode__m_allowedChars   0x150 /**/ // CCTextInputNode::setAllowedChars (1.0 and 1.1)

/**/ #define CCTextInputNode__m_charLimit      0x170 /**/ // CCTextInputNode::getCharLimit
/**/ #define CCTextInputNode__m_inputDelegate  0x16c /**/ // CCTextInputNode::getDelegate
/**/ #define CCTextInputNode__m_textField      0x168 /**/ // CCTextInputNode::getTextField
/**/ #define CCEGLViewProtocol__m_screenBottom  0xe4 /**/ // cocos2d::CCEGLViewProtocol::getScreenBottom
/**/ #define CheckpointObject__m_playerPos     0x12c /**/ // CheckpointObject::getPlayerPos
/**/ #define ColorPickerPopup__m_colorWheel    0x1cc /**/ // ColorPickerPopup::selectColor
/**/ #define ColorSelectPopup__m_colorWheel    0x1cc /**/ // ColorSelectPopup::selectColor
/**/ #define CommentCell__m_parent             0x17c /**/ // CommentCell::loadFromComment (calls the 0xFC virtual on it, which is removeAllChildrenWithCleanup)
/**/ #define EditButtonBar__m_buttons          0x12c /**/ // EditButtonBar::init
/**/ #define EditLevelLayer__m_level           0x148 /**/ // EditLevelLayer::init
/**/ #define EditorPauseLayer__m_editorLayer   0x1c8 /**/ // EditorPauseLayer::init
/**/ #define EditorUI__m_buttonMenu            0x184 /**/ // EditorUI::setupDeleteMenu
/**/ #define EditorUI__m_createButtonBar       0x144 /**/ // EditorUI::setupCreateMenu (at the bottom)
/**/ #define EditorUI__m_createButtons         0x1bc /**/ // EditorUI::getCreateBtn (at the bottom)
/**/ #define EditorUI__m_editButtonBar         0x148 /**/ // EditorUI::createMoveMenu (at the bottom)

/**/ #define EditorUI__m_editObjectButton      0x1ac /**/ // EditorUI::createMoveMenu (at the bottom) (pre-1.4)

/**/ #define EditorUI__m_editorLayer           0x1c4 /**/ // EditorUI::init
/**/ #define EditorUI__m_selectedObject        0x248 /**/ // EditorUI::selectObject
/**/ #define EditorUI__m_selectedObjects       0x180 /**/ // EditorUI::deselectAll 
/**/ #define EditorUI__m_unkFloat              0x158 /**/ // EditorUI::setupCreateMenu (at the bottom)
/**/ #define GameManager__m_playLayer          0x150 /**/ // GameManager::getPlayLayer
/**/ #define GameManager__m_playerName         0x168 /**/ // GameManager::getPlayerName
/**/ #define GameManager__m_playerUDID         0x164 /**/ // GameManager::getPlayerUDID
/**/ #define GameObject__m_objectKey           0x310 /**/ // GameObject::getObjectKey
/**/ #define GameObject__m_particles           0x280 /**/ // GameObject::setOpacity
/**/ #define GameObject__m_realPosition        0x2c4 /**/ // GameObject::getRealPosition
/**/ #define GameObject__m_sectionIdx          0x2bc /**/ // GameObject::getSectionIdx
/**/ #define GameObject__m_shouldSpawn         0x2c0 /**/ // GameObject::getShouldSpawn
/**/ #define GameObject__m_type                0x2b8 /**/ // GameObject::getType
/**/ #define GameObject__m_useAudioScale       0x2eb /**/ // GameObject::getUseAudioScale
/**/ #define GJComment__m_commentID            0x130 /**/ // GJComment::getCommentID
/**/ #define GJSearchObject__m_type            0x128 /**/ // GJSearchObject::getType
/**/ #define LevelBrowserLayer__m_searchObject 0x154 /**/ // LevelBrowserLayer::setSearchObject
/**/ #define LevelCell__m_level                0x180 /**/ // LevelCell::loadFromLevel
/**/ #define LevelEditorLayer__m_batchNode     0x140 /**/ // LevelEditorLayer::createObject (1st function call after GameObject::createObject)
/**/ #define LevelEditorLayer__m_gameLayer     0x158 /**/ // LevelEditorLayer::getGameLayer
/**/ #define LevelEditorLayer__m_gridLayer     0x150 /**/ // LevelEditorLayer::removeObject
/**/ #define LevelEditorLayer__m_objectCount   0x154 /**/ // LevelEditorLayer::getObjectCount
/**/ #define LevelEditorLayer__m_sections      0x144 /**/ // LevelEditorLayer::getSectionCount
/**/ #define LevelEditorLayer__m_settings      0x160 /**/ // LevelEditorLayer::getLevelSettings
/**/ #define LevelEditorLayer__m_redoArray     0x14c /**/ // LevelEditorLayer::addToRedoList
/**/ #define LevelInfoLayer__m_level           0x154 /**/ // LevelInfoLayer::init
/**/ #define LevelSelectLayer__m_scrollLayer   0x14c /**/ // LevelSelectLayer::onNext (param in BoomScrollLayer::quickUpdate)
/**/ #define LocalLevelManager__m_localLevels  0x130 /**/ // LocalLevelManager::getLocalLevels
/**/ #define PlayLayer__m_attempts             0x288 /**/ // PlayLayer::getAttempts
/**/ #define PlayLayer__m_attemptLabel         0x1e0 /**/ // PlayLayer::updateAttempts (assigned to a var right after the if statement)
/**/ #define PlayLayer__m_checkpoints          0x154 /**/ // PlayLayer::removeLastCheckpoint
/**/ #define PlayLayer__m_clkTimer             0x290 /**/ // PlayLayer::getClkTimer
/**/ #define PlayLayer__m_hazards              0x188 /**/ // PlayLayer::checkCollisions (the for loop at the bottom which checks for rect intersections and calls destroyPlayer)
/**/ #define PlayLayer__m_isPractice           0x255 /**/ // PlayLayer::getPracticeMode
/**/ #define PlayLayer__m_jumps                0x28c /**/ // PlayLayer::getJumps
/**/ #define PlayLayer__m_lastX                0x1d8 /**/ // PlayLayer::destroyPlayer (find the multiplication by 100, and the denominator in the division right before it is this field)
/**/ #define PlayLayer__m_level                0x230 /**/ // PlayLayer::getLevel
/**/ #define PlayLayer__m_pGlitter             0x1a0 /**/ // PlayLayer::toggleGlitter
/**/ #define PlayLayer__m_playerObject         0x22c /**/ // PlayLayer::getPlayer
/**/ #define PlayLayer__m_sections             0x184 /**/ // PlayLayer::removeObjectFromSection
/**/ #define PlayLayer__m_startPos             0x268 /**/ // PlayLayer::getStartPos
/**/ #define PlayLayer__m_uiLayer              0x228 /**/ // PlayLayer::getUILayer
/**/ #define PlayLayer__m_unkPrac              0x267 /**/ // PlayLayer::togglePracticeMode (set to 1 at the bottom)
/**/ #define PlayerObject__m_gravity           0x350 /**/ // PlayerObject::gravityUp
/**/ #define PlayerObject__m_onGround          0x35e /**/ // PlayerObject::pushButton (if statement that updateJump is called in)
/**/ #define PlayerObject__m_pGround           0x3b8 /**/ // PlayerObject::deactivateParticle
/**/ #define PlayerObject__m_pShipGround       0x3c4 /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pShipFire         0x3bc /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pShipLift         0x3c0 /**/ // PlayerObject::update (man idk how to describe where to find it)
/**/ #define PlayerObject__m_pLanding          0x3cc /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_pLanding2         0x3d0 /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_unk1              0x368 /**/ // PlayerObject::playerDestroyed (at the top)
/**/ #define PlayerObject__m_unk2              0x3df /**/ // PlayerObject::playerDestroyed (slightly below, set to 1)
/**/ #define PlayerObject__m_xVelocity         0x340 /**/ // PlayerObject::speedUp
/**/ #define PlayerObject__m_yStart            0x348 /**/ // PlayerObject::yStartUp
/*******************************************************/
/*/                     ADDRESSES                     /*/
/*******************************************************/
/**/ #define basicstring_assign             0x2D1FD8 /**/ // differs between versions :/
/**/ #define global_order_of_arrival        0x3F7E6C /**/ // CCNode::reorderChild
/**/ #define move_x_max                     0x14E374 /**/ // EditorUI::moveObject - 00 60 EA 46
/**/ #define move_y_max                     0x14E37C /**/ // EditorUI::moveObject - 00 40 A1 44
/**/ #define move_y_min                     0x14E378 /**/ // EditorUI::moveObject - 00 00 B6 42
/**/ #define object_limit                   0x150210 /**/ // EditorUI::onCreate
/**/ #define pause_edit_button              0x1528E6 /**/ // PauseLayer::customSetup
/**/ #define pause_restart_button           0x1529A0 /**/ // PauseLayer::customSetup
/**/ #define place_x_max                    0x15001C /**/ // EditorUI::onCreateObject - 00 42 EA 46
/**/ #define place_y_max                    0x150020 /**/ // EditorUI::onCreateObject - 00 60 9F 44
/**/ #define place_x_min                    0x150014 /**/ // EditorUI::onCreateObject - 00 80 9D 43
/**/ #define place_y_min                    0x150018 /**/ // EditorUI::onCreateObject - 00 00 D2 42
/**/ #define ship_tint                      0x143B64 /**/ // PlayerObject::setColor
/**/ #define zoom_bypass_max_1              0x14E6D4 /**/ // EditorUI::zoomGameLayer
/**/ #define zoom_bypass_max_2              0x14E80E /**/ // EditorUI::zoomGameLayer
/**/ #define zoom_bypass_min                0x14E81C /**/ // EditorUI::zoomGameLayer
/*******************************************************/