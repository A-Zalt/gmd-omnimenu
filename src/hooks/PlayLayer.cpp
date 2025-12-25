#include "hook.hpp"
#include "PlayLayer.hpp"
#include "UILayer.hpp"
#include "Utils.hpp"

void (*TRAM_PlayLayer_destroyPlayer)(PlayLayer* self);
void PlayLayer_destroyPlayer(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NOCLIP) || hax.getModuleEnabled(ModuleID::INSTANT_COMPLETE)) {
        if (hax.lastDeadFrame < hax.frameCount)
            hax.deadFrames++;

        if (hax.lastDeadFrame < hax.frameCount - 1 && !hax.completed)
            hax.deaths++;

        hax.lastDeadFrame = hax.frameCount;
        getPlayLayerHazards(self)->removeAllObjects(); // the humble noclip lag fix
        return;
    }
    float brDiff = hax.bestRunEnd - hax.bestRunStart;
    float currRun = getCurrentPercentageF(self);
    if (currRun - hax.startPercent > brDiff) {
        hax.bestRunStart = hax.startPercent;
        hax.bestRunEnd = currRun;
    }
    hax.dead = true;
    TRAM_PlayLayer_destroyPlayer(self);
    if (hax.getModuleEnabled(ModuleID::PRACTICE_MUSIC_HACK) && getPlayLayerPractice(self)) {
        auto audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
        audioEngine->pauseBackgroundMusic();
    }
    if (hax.getModuleEnabled(ModuleID::CUSTOM_RESPAWN_TIME) && !hax.completed) {
#if GAME_VERSION >= GV_1_6
        // 1.6 and above start using a tag for this action, so it's trivial to stop it
        self->stopActionByTag(16);
#else
        // However, before 1.5, there is no trivial way to stop the action, since it doesn't get assigned a tag.
    #if GAME_VERSION > GV_1_0
        // The ability to disable Auto-Retry was added in 1.11
        if (!getAutoRetry()) return;
        auto seq2 = CCSequence::create(
            CCDelayTime::create(1.05),
            CCCallFunc::create(self, callfunc_selector(PlayLayer::turnOffTheThing)),
            nullptr
        );
        seq2->setTag(17);
        self->runAction(seq2);
    #endif

        // What we'll do is hook delayedResetLevel and make it stop execution if this is still true
        // Our custom action will call a function that sets this variable to false and then calls resetLevel
        // However, on 1.0 there is no delayedResetLevel
        // So we'll have to check this variable in resetLevel
        // But that also means that any other call to resetLevel will have to first set this variable to false
        // Which is why there is a 1.0-exclusive hook of PauseLayer::onRestart
        hax.customRespawn = true;

#endif // GAME_VERSION >= GV_1_6
        auto seq = CCSequence::create(
            CCDelayTime::create(hax.respawnTime),
#if GAME_VERSION < GV_1_6
            CCCallFunc::create(self, callfunc_selector(PlayLayer::customResetLevel)),
#else
            CCCallFunc::create(self, callfunc_selector(PlayLayer::delayedResetLevel)),
#endif
            nullptr);
        
        // In 1.5-, this is necessary to identify the action in resetLevel and stop it if it exists
        // In 1.6+, it's necessary for obvious reasons
        seq->setTag(16);
        self->runAction(seq);
    }
}

#if GAME_VERSION < GV_1_6
void PlayLayer::customResetLevel() {
    HaxManager& hax = HaxManager::sharedState();
    PlayLayer::resetLevelLogic(this);
#if GAME_VERSION >= GV_1_1
    setShouldRunDelayedReset(this, false);
#endif
}
#if GAME_VERSION >= GV_1_1
void (*TRAM_PlayLayer_delayedResetLevel)(PlayLayer* self);
void PlayLayer_delayedResetLevel(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.customRespawn) return;
    TRAM_PlayLayer_delayedResetLevel(self);
}
#endif // GAME_VERSION >= GV_1_1
#endif // GAME_VERSION < GV_1_6

void (*TRAM_PlayLayer_togglePracticeMode)(PlayLayer* self, bool toggle);
void PlayLayer_togglePracticeMode(PlayLayer* self, bool toggle) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::PRACTICE_MUSIC_HACK) && getPlayLayerPractice(self) != toggle) {
        // recreated function basically
        setPlayLayerPractice(self, toggle);
        UILayer* uiLayer = getUILayer(self);
        uiLayer->toggleCheckpointsMenu(toggle);
        if (!toggle) {
            cocos2d::CCArray* checkpoints = getPlayLayerCheckpoints(self);
            while (checkpoints->count() > 0) {
                self->removeLastCheckpoint();
            }
            MEMBER_BY_OFFSET(bool, self, PlayLayer__m_unkPrac) = true;
            self->resetLevel();
        }
    } else TRAM_PlayLayer_togglePracticeMode(self, toggle);
    if (hax.spSwitcherParent) {
        if (toggle) hax.spSwitcherParent->setPosition(ccp(hax.spSwitcherParent->getPositionX(), 85));
        else hax.spSwitcherParent->setPosition(ccp(hax.spSwitcherParent->getPositionX(), 23));
    }
}

void instantComplete(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    PlayerObject* player = getPlayer(self); // PlayLayer::getPlayer
    player->lockPlayer();
    self->levelComplete();
    hax.instantComped = true;
}

void (*TRAM_PlayLayer_levelComplete)(PlayLayer* self);
void PlayLayer_levelComplete(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    float brDiff = hax.bestRunEnd - hax.bestRunStart;
    if (100 - hax.startPercent > brDiff) {
        hax.bestRunStart = hax.startPercent;
        hax.bestRunEnd = 100;
    }
    hax.completed = true;
    TRAM_PlayLayer_levelComplete(self);
    // bandaid fix
    if (hax.getModuleEnabled(ModuleID::PRACTICE_MUSIC_HACK) && getPlayLayerPractice(self)) {
        auto audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
        audioEngine->resumeBackgroundMusic();
    }
}

void (*TRAM_PlayLayer_resetLevel)(PlayLayer* self);
void PlayLayer_resetLevel(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
#if GAME_VERSION < GV_1_1
    if (hax.customRespawn) return;
    PlayLayer::resetLevelLogic(self);
}
void PlayLayer::turnOffTheThing() {
    HaxManager& hax = HaxManager::sharedState();
    hax.customRespawn = false;
}
void PlayLayer::resetLevelLogic(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
#endif
    if (hax.getCheatIndicatorColor() == CheatIndicatorColor::Orange) hax.hasCheated = false;
    hax.instantComped = false;
    hax.lastDeadFrame = -1;
    hax.frameCount = 0;
    hax.deaths = 0;
    hax.clicks = 0;
    hax.deadFrames = 0;
    hax.noclipAccuracy = 100;
    hax.completed = false;
    hax.dead = false;
    hax.hasTouchedTheEye = false;
    if (hax.getModuleEnabled(ModuleID::PRACTICE_MUSIC_HACK) && getPlayLayerPractice(self)) {
        auto audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
        int seekTime = 0;
        CCPoint startPos = getStartPos(self);
#if GAME_VERSION < GV_1_7
        seekTime = floorf((startPos.x / 311.58f) * 1000.f);
#else
        seekTime = floorf(self->timeForXPos(startPos.x, true) * 1000);
#endif
        CCNode* lastCheckpoint = self->getLastCheckpoint();
        if (lastCheckpoint != nullptr) {
            CCPoint lastCheckpointPos = getCheckpointPosition(lastCheckpoint);
#if GAME_VERSION < GV_1_7
            seekTime = floorf((lastCheckpointPos.x / 311.58f) * 1000.f);
#else
            seekTime = floorf(self->timeForXPos(lastCheckpointPos.x, true) * 1000);
#endif
        }
        if (seekTime > 0) {
            JNIEnv* env = getEnv();
            if (env) {
                seekBackgroundMusicTo(seekTime);
            } else {
                cocos2d::CCLog("Failed to get Java Env");
                audioEngine->setBackgroundMusicTime(static_cast<float>(seekTime) / 1000.f);
            }
        } else {
            audioEngine->setBackgroundMusicTime(0.f);
        }
        audioEngine->resumeBackgroundMusic();
    }
    if (hax.percentageLabel) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        hax.percentageLabel->setFntFile("bigFont.fnt");
        hax.percentageLabel->setScale(0.5f);
        hax.percentageLabel->setPositionY(winSize.height - 7.5);
    }
    auto respawnAction = self->getActionByTag(16);
    if (respawnAction) {
        self->stopAction(respawnAction);
    }
    TRAM_PlayLayer_resetLevel(self);
    // if (hax.getModuleEnabled(ModuleID::MUSIC_BUG_FIX) && !getPlayLayerPractice(self)) {
    //     hax.updatedMusic = false;
    // }
#if GAME_VERSION == GV_1_4
    if (hax.getModuleEnabled(ModuleID::OBJ_COLOR_FIX)) {
        self->tintObjects(ccWHITE, 0.f);
    }
#endif
    hax.startPercent = getCurrentPercentageF();
    if (hax.getModuleEnabled(ModuleID::INSTANT_COMPLETE)) {
        instantComplete(self);
    }
}

void (*TRAM_PlayLayer_onQuit)(PlayLayer* self);
void PlayLayer_onQuit(PlayLayer* self) {
    TRAM_PlayLayer_onQuit(self);
    HaxManager& hax = HaxManager::sharedState();
    hax.quitPlayLayer = true;
}

void (*TRAM_PlayLayer_toggleFlipped)(void* self, bool p1, bool p2);
void PlayLayer_toggleFlipped(void* self, bool p1, bool p2) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_MIRROR)) return;
    TRAM_PlayLayer_toggleFlipped(self, p1, p2);
}

void (*TRAM_PlayLayer_update)(PlayLayer* self, float dt);
void PlayLayer_update(PlayLayer* self, float dt) {
    HaxManager& hax = HaxManager::sharedState();
    TRAM_PlayLayer_update(self, dt);
    // if (!hax.updatedMusic && hax.getModuleEnabled(ModuleID::MUSIC_BUG_FIX)) {
    //     float seekTime = getPlayer()->getPositionX() / 311.58f;
    //     if (seekTime > 0) {
    //         CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicTime(seekTime);
    //     }
    //     hax.updatedMusic = true;
    // }
    hax.frameCount++;
    if (hax.quitPlayLayer) return;
    UILayer* uiLayer = getUILayer(self);
    // force visibility
    if (!getPlayLayerPractice(self) && getPlayLayerCheckpoints(self)->count() > 0) {
        hax.checkpointsInNormalMode = true;
        hax.hasCheated = true;
    } else {
        hax.checkpointsInNormalMode = false;
    }
    if (hax.getModuleEnabled(ModuleID::CHEAT_INDICATOR)) {
        if (!hax.cheatIndicatorLabel || hax.cheatIndicatorLabel == nullptr) {
            uiLayer->createCheatIndicator();
        } else if (!hax.cheatIndicatorLabel->isVisible())
            hax.cheatIndicatorLabel->setVisible(true);

        switch (hax.getCheatIndicatorColor()) {
            case CheatIndicatorColor::Green:
                hax.cheatIndicatorLabel->setColor(ccGREEN);
                break;
            case CheatIndicatorColor::Yellow:
                hax.cheatIndicatorLabel->setColor(ccYELLOW);
                break;
            case CheatIndicatorColor::Orange:
                hax.cheatIndicatorLabel->setColor(ccORANGE);
                break;
            case CheatIndicatorColor::Red:
                hax.cheatIndicatorLabel->setColor(ccRED);
                break;
            default:
                hax.cheatIndicatorLabel->setColor(ccWHITE);
                break;
        };
    } else {
        if (hax.cheatIndicatorLabel && hax.cheatIndicatorLabel != nullptr && hax.cheatIndicatorLabel->isVisible()) 
            hax.cheatIndicatorLabel->setVisible(false);
    }
    if (hax.getModuleEnabled(ModuleID::SHOW_PERCENTAGE)) {
        if (!hax.percentageLabel || hax.percentageLabel == nullptr) {
            uiLayer->createPercentageLabel();
        } else if (!hax.percentageLabel->isVisible()) {
            hax.percentageLabel->setVisible(true);
        }
        if (hax.getModuleEnabled(ModuleID::SHOW_PERCENTAGE_DECIMAL)) {
            hax.percentageLabel->setString(CCString::createWithFormat("%.3f%%", getCurrentPercentageF(self))->getCString());
        } else {
            hax.percentageLabel->setString(CCString::createWithFormat("%i%%", getCurrentPercentage(self))->getCString());
        }
        auto sp = getStartPos(self);
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        if (hax.getModuleEnabled(ModuleID::GOLDEN_BEST) && !getPlayLayerPractice(self) && sp.x == 0 && sp.y == 105) {
            if (getCurrentPercentage() > getPlayLayerLevel(self)->m_nNormalPercent) {
                if (strcmp(hax.percentageLabel->getFntFile(), "goldFont.fnt")) {
                    hax.percentageLabel->setFntFile("goldFont.fnt");
                    hax.percentageLabel->setScale(0.68f);
                    hax.percentageLabel->setPositionY(winSize.height - 7);
                }
            } else if (!hax.dead && strcmp(hax.percentageLabel->getFntFile(), "bigFont.fnt")) {
                hax.percentageLabel->setFntFile("bigFont.fnt");
                hax.percentageLabel->setScale(0.5f);
                hax.percentageLabel->setPositionY(winSize.height - 7.5);
            }
        } else {
            if (strcmp(hax.percentageLabel->getFntFile(), "bigFont.fnt")) {
                hax.percentageLabel->setFntFile("bigFont.fnt");
                hax.percentageLabel->setScale(0.5f);
                hax.percentageLabel->setPositionY(winSize.height - 7.5);
            }
        }
#if GAME_VERSION >= GV_1_5
        if (getShowProgressBar()) {
            hax.percentageLabel->setPositionX(winSize.width / 2 + 110);
        } else {
            hax.percentageLabel->setPositionX(winSize.width / 2);
        }
#endif
    } else {
        if (hax.percentageLabel && hax.percentageLabel != nullptr && hax.percentageLabel->isVisible())
            hax.percentageLabel->setVisible(false);
    }
    if (hax.getModuleEnabled(ModuleID::PCOMMAND)) {
        if (!hax.pMenu || !hax.pButton1 || !hax.pButton2 || !hax.pButton3 || !hax.pButton4 || !hax.pButton5 || !hax.pButton6) {
            uiLayer->createPCommand();
        } else if (!hax.pMenu->isVisible()) {
            hax.pMenu->setVisible(true);
        }
    } else {
        if (hax.pMenu && hax.pMenu->isVisible()) {
            hax.pMenu->setVisible(false);
        }
    }
    if (hax.getShowLabel()) {
        if (!hax.uiLabel || hax.uiLabel == nullptr) {
            uiLayer->createLabel();
        } else if (!hax.uiLabel->isVisible()) {
            hax.uiLabel->setVisible(true);
        }
        uiLayer->updateLabel();
    } else {
        if (hax.uiLabel && hax.uiLabel->isVisible())
            hax.uiLabel->setVisible(false);
    }
    if (hax.getModuleEnabled(ModuleID::START_POS_SWITCHER)) {
        if (hax.startPosIndex == -2) {
            hax.startPositions = getStartPositions_(self);
            hax.startPositions->retain();
            hax.startPosIndex = hax.startPositions->count() - 1;
        }
        if (!hax.spSwitcherParent) {
            uiLayer->createSwitcher();
        } else if (!hax.spSwitcherParent->isVisible()) {
            hax.spSwitcherParent->setVisible(true);
        }
    } else {
        if (hax.spSwitcherParent && hax.spSwitcherParent->isVisible())
            hax.spSwitcherParent->setVisible(false);
    }
    if (hax.getModuleEnabled(ModuleID::INSTANT_COMPLETE) && !hax.instantComped) {
        instantComplete(self);
    }
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_BACKGROUND)) {
        auto p = getBGParticles(self);
        if (p && p != nullptr) p->stopSystem();
    }
    if (hax.getModuleEnabled(ModuleID::HIDE_ATTEMPTS)) {
        getAttemptLabel(self)->setVisible(false);
    } else {
        getAttemptLabel(self)->setVisible(true);
    }
    if (hax.getModuleEnabled(ModuleID::NOCLIP_TINT_ON_DEATH) && hax.getModuleEnabled(ModuleID::NOCLIP)) {
        if (hax.lastDeadFrame < hax.frameCount - 1 || hax.completed) {
            hax.ntOpacity -= 1500 * dt;
            if (hax.ntOpacity < 0) hax.ntOpacity = 0;
        } else {
            hax.ntOpacity += 1500 * dt;
            if (hax.ntOpacity > 100) hax.ntOpacity = 100;
        }
        int intOp = static_cast<int>(hax.ntOpacity);
        int currOp = hax.noclipTint->getOpacity();
        if (currOp != intOp) {
            hax.noclipTint->setOpacity(intOp);
        }
    } else {
        if (hax.noclipTint->getOpacity() > 0) {
            hax.noclipTint->setOpacity(0);
        }
    }
    auto chMenu = getCheckpointMenu(getUILayer(self));
    if (chMenu->isVisible()) {
        if (hax.getModuleEnabled(ModuleID::HIDE_CHECKPOINT_BUTTONS)) {
            if (chMenu->getOpacity() > 60) chMenu->setOpacity(60);
        } else if (chMenu->getOpacity() < 255)
            chMenu->setOpacity(255);
    }
    if (hax.frameCount > 0 && !hax.completed) {
        hax.noclipAccuracy = static_cast<float>(hax.frameCount - hax.deadFrames) / static_cast<float>(hax.frameCount) * 100;
    }
}
bool (*TRAM_PlayLayer_init)(PlayLayer* self, GJGameLevel* level);
bool PlayLayer_init(PlayLayer* self, GJGameLevel* level) {
    if (!TRAM_PlayLayer_init(self, level)) return false;
    HaxManager& hax = HaxManager::sharedState();
    hax.startPercent = getCurrentPercentageF();
    if (hax.getModuleEnabled(ModuleID::HIDE_ATTEMPTS)) {
        getAttemptLabel(self)->setVisible(false);
    }
    if (hax.getCheatIndicatorColor() == CheatIndicatorColor::Red || hax.getCheatIndicatorColor() == CheatIndicatorColor::Orange) {
        hax.setCheating(true);
    }
    hax.quitPlayLayer = false;
    hax.startPosIndex = -2;
    if (hax.getModuleEnabled(ModuleID::START_POS_SWITCHER)) {
        hax.startPositions = getStartPositions_(self);
        hax.startPositions->retain();
        hax.startPosIndex = hax.startPositions->count() - 1;
        getUILayer(self)->createSwitcher();
    }
    return true;
}
void (*TRAM_PlayLayer_shakeCamera)(PlayLayer* self, float duration);
void PlayLayer_shakeCamera(PlayLayer* self, float duration) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::NO_SHAKE)) return;
    TRAM_PlayLayer_shakeCamera(self, duration);
}
#if GAME_VERSION >= GV_1_5
void (*TRAM_PlayLayer_toggleProgressbar)(PlayLayer* self);
void PlayLayer_toggleProgressbar(PlayLayer* self) {
    TRAM_PlayLayer_toggleProgressbar(self);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.percentageLabel) {
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        if (getShowProgressBar()) {
            hax.percentageLabel->setAnchorPoint({0, 0.5});
            hax.percentageLabel->setPositionX(winSize.width / 2 + 110);
        } else {
            hax.percentageLabel->setAnchorPoint({0.5, 0.5});
            hax.percentageLabel->setPositionX(winSize.width / 2);
        }
    }
}
void (*TRAM_PlayLayer_showNewBest)(PlayLayer* self); 
void PlayLayer_showNewBest(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.isSafeMode()) return;
    TRAM_PlayLayer_showNewBest(self);
}
#endif
// CCParticleSystemQuad* (*TRAM_PlayLayer_createParticle)(void* self, int a1, int a2, const char* file, int a4, tCCPositionType a5);
// CCParticleSystemQuad* PlayLayer_createParticle(void* self, int a1, int a2, const char* file, int a4, tCCPositionType a5) {
//     auto particle = TRAM_PlayLayer_createParticle(self, a1, a2, file, a4, a5);
//     HaxManager& hax = HaxManager::sharedState();
//     if (!hax.getModuleEnabled("particle_end_wall") && !strcmp(file, "endEffectPortal.plist")) {
//         particle->setVisible(false);
//     }
//     return particle;
// }
// void (*TRAM_PlayLayer_toggleGlitter)(PlayLayer* self, bool toggle);
// void PlayLayer_toggleGlitter(PlayLayer* self, bool toggle) {
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.getModuleEnabled("particle_background")) return TRAM_PlayLayer_toggleGlitter(self, false);
//     TRAM_PlayLayer_toggleGlitter(self, toggle);
// }

// CCParticleSystemQuad* (*TRAM_PlayLayer_createParticle)(void* self, int a1, const char* a2, int a3, tCCPositionType type);
// CCParticleSystemQuad* PlayLayer_createParticle(void* self, int a1, const char* a2, int a3, tCCPositionType type) {
//     CCParticleSystemQuad* particles = TRAM_PlayLayer_createParticle(self, a1, a2, a3, type);
//     HaxManager& hax = HaxManager::sharedState();
//     if (hax.getModuleEnabled("no_particles")) particles->setVisible(false);
//     return particles;
// }
#if GAME_VERSION >= GV_1_6
void (*TRAM_PlayLayer_spawnParticle)(PlayLayer* self, const char* particleName, int a3, tCCPositionType type, CCPoint position);
void PlayLayer_spawnParticle(PlayLayer* self, const char* particleName, int a3, tCCPositionType type, CCPoint position) {
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_DESTRUCTIBLE_BLOCKS) && !strcmp(particleName, "glassDestroy01.plist"))
        return;

    if (!hax.getModuleEnabled(ModuleID::PARTICLE_SECRET_COINS) && !strcmp(particleName, "coinPickupEffect.plist"))
        return;

    TRAM_PlayLayer_spawnParticle(self, particleName, a3, type, position);
}
// Safe Mode: secret coins
void (*TRAM_PlayLayer_processItems)(PlayLayer* self);
void PlayLayer_processItems(PlayLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
#ifdef FORCE_AUTO_SAFE_MODE
#ifndef STAR_RATED_LEVELS_GRANT_COINS
    if (getPlayLayerLevel(self)->m_eLevelType != GJLevelType::Local) return;
#else
    if (getPlayLayerLevel(self)->m_nStars < 1) return;
#endif
#endif
    if (hax.isSafeMode()) return;
    TRAM_PlayLayer_processItems(self);
}
#endif

#if GAME_VERSION >= GV_1_7
void (*TRAM_PlayLayer_playSpeedParticle)(PlayLayer* self, float speed);
void PlayLayer_playSpeedParticle(PlayLayer* self, float speed) {
    HaxManager& hax = HaxManager::sharedState();
    if (!hax.getModuleEnabled(ModuleID::PARTICLE_SPEED_PORTALS)) return;
    TRAM_PlayLayer_playSpeedParticle(self, speed);
}
#endif

void PlayLayer_om() {
    Omni::hook("_ZN9PlayLayer13destroyPlayerEv", 
        reinterpret_cast<void*>(PlayLayer_destroyPlayer),
        reinterpret_cast<void**>(&TRAM_PlayLayer_destroyPlayer));
    Omni::hook("_ZN9PlayLayer18togglePracticeModeEb",
        reinterpret_cast<void*>(PlayLayer_togglePracticeMode),
        reinterpret_cast<void**>(&TRAM_PlayLayer_togglePracticeMode));
    Omni::hook("_ZN9PlayLayer6updateEf",
        reinterpret_cast<void*>(PlayLayer_update),
        reinterpret_cast<void**>(&TRAM_PlayLayer_update));
#if GAME_VERSION > GV_1_0
    Omni::hook("_ZN9PlayLayer13toggleFlippedEbb",
        reinterpret_cast<void*>(PlayLayer_toggleFlipped),
        reinterpret_cast<void**>(&TRAM_PlayLayer_toggleFlipped));
#if GAME_VERSION < GV_1_6
    Omni::hook("_ZN9PlayLayer17delayedResetLevelEv",
        reinterpret_cast<void*>(PlayLayer_delayedResetLevel),
        reinterpret_cast<void**>(&TRAM_PlayLayer_delayedResetLevel));
#endif
#endif
    Omni::hook("_ZN9PlayLayer10resetLevelEv",
        reinterpret_cast<void*>(PlayLayer_resetLevel),
        reinterpret_cast<void**>(&TRAM_PlayLayer_resetLevel));
    Omni::hook("_ZN9PlayLayer6onQuitEv",
        reinterpret_cast<void*>(PlayLayer_onQuit),
        reinterpret_cast<void**>(&TRAM_PlayLayer_onQuit));
    Omni::hook("_ZN9PlayLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(PlayLayer_init),
        reinterpret_cast<void**>(&TRAM_PlayLayer_init));
    Omni::hook("_ZN9PlayLayer11shakeCameraEf",
        reinterpret_cast<void*>(PlayLayer_shakeCamera),
        reinterpret_cast<void**>(&TRAM_PlayLayer_shakeCamera));
    Omni::hook("_ZN9PlayLayer13levelCompleteEv",
        reinterpret_cast<void*>(PlayLayer_levelComplete),
        reinterpret_cast<void**>(&TRAM_PlayLayer_levelComplete));
#if GAME_VERSION >= GV_1_5
    Omni::hook("_ZN9PlayLayer17toggleProgressbarEv",
        reinterpret_cast<void*>(PlayLayer_toggleProgressbar),
        reinterpret_cast<void**>(&TRAM_PlayLayer_toggleProgressbar));
    Omni::hook("_ZN9PlayLayer11showNewBestEv",
        reinterpret_cast<void*>(PlayLayer_showNewBest),
        reinterpret_cast<void**>(&TRAM_PlayLayer_showNewBest));
#endif
#if GAME_VERSION >= GV_1_6
    Omni::hook("_ZN9PlayLayer13spawnParticleEPKciN7cocos2d15tCCPositionTypeENS2_7CCPointE",
        reinterpret_cast<void*>(PlayLayer_spawnParticle),
        reinterpret_cast<void**>(&TRAM_PlayLayer_spawnParticle));
    Omni::hook("_ZN9PlayLayer12processItemsEv",
        reinterpret_cast<void*>(PlayLayer_processItems),
        reinterpret_cast<void**>(&TRAM_PlayLayer_processItems));
#endif
#if GAME_VERSION >= GV_1_7
    Omni::hook("_ZN9PlayLayer17playSpeedParticleEf",
        reinterpret_cast<void*>(PlayLayer_playSpeedParticle),
        reinterpret_cast<void**>(&TRAM_PlayLayer_playSpeedParticle));
#endif
    // Omni::hook("_ZN9PlayLayer14createParticleEiPKciN7cocos2d15tCCPositionTypeE",
    //     reinterpret_cast<void*>(PlayLayer_createParticle),
    //     reinterpret_cast<void**>(&TRAM_PlayLayer_createParticle));
    // Omni::hook("_ZN9PlayLayer13toggleGlitterEb",
    //     reinterpret_cast<void*>(PlayLayer_toggleGlitter),
    //     reinterpret_cast<void**>(&TRAM_PlayLayer_toggleGlitter));
}