#pragma once
#ifndef _LAYERS_PLAYLAYER_HPP
#define _LAYERS_PLAYLAYER_HPP

#include <cocos2d.h>

#include <robtop/objects/playerobject.hpp>
#include <robtop/objects/startposobject.hpp>

class PlayLayer : public cocos2d::CCLayer {
public:
    void destroyPlayer(PlayerObject*);
    void togglePracticeMode(bool);
    void onQuit();
    bool getPracticeMode() const;
    bool getPlaybackMode() const;
    void setupReplay(std::string replay);
    GJGameLevel* getLevel() const;

    cocos2d::CCLayer* getGameLayer() const;

    bool init(GJGameLevel*);

    void levelComplete();
    void updateProgressbar();
 
    void resume();
    void resumeAndRestart();

    void showRetryLayer();
    void showEndLayer();

    void pauseGame(bool);

    void resetLevel();

    StartPosObject* setStartPosObject(StartPosObject*);
    void setStartPos(cocos2d::CCPoint);



    PlayerObject* getPlayer() const;
    PlayerObject* getPlayer2() const;

};

#endif
