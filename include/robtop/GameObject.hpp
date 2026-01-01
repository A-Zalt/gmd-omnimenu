#pragma once

#include <cocos2d.h>

typedef enum {
    Solid,
    Unk1,
    Hazard,
    ReverseGravityPortal,
    NormalGravityPortal,
    IconPortal,
    ShipPortal,
    Decoration,
    Pulsing,
    YellowPad,
#if GAME_VERSION >= GV_1_5
    PinkPad,
#endif
#if GAME_VERSION >= GV_1_3
    BluePad,
#endif
    YellowOrb,
#if GAME_VERSION >= GV_1_5
    PinkOrb,
#endif
#if GAME_VERSION >= GV_1_3
    BlueOrb,
#endif
    MirrorPortal,
    UnmirrorPortal,
#if GAME_VERSION >= GV_1_1
    BallPortal,
#endif
#if GAME_VERSION >= GV_1_4
    NormalSizePortal,
    MiniSizePortal,
#endif
#if GAME_VERSION >= GV_1_5
    BirdPortal,
#endif
#if GAME_VERSION >= GV_1_6
    SpeedPortal, // apparently this type already exists in 1.6 for whatever reason
    BreakableBlock,
    SecretCoin
#endif
} GameObjectType;

#if GAME_VERSION >= GV_1_7
class CCSpritePlus : public cocos2d::CCSprite {
private:
    cocos2d::CCArray* followers_;
    CCSpritePlus* following_;
    bool hasFollowers_;

    bool initWithSpriteFrameName(const char*) override;

public:
    static CCSpritePlus* createWithSpriteFrame(cocos2d::CCSpriteFrame*);
    static CCSpritePlus* createWithSpriteFrameName(const char*);

    void addFollower(cocos2d::CCNode*);
    void removeFollower(cocos2d::CCNode*);

    void followSprite(CCSpritePlus*);
    void stopFollow();

    void setPosition(const cocos2d::CCPoint&) override;
    void setRotation(float) override;
    void setScale(float) override;
    void setScaleX(float) override;
    void setScaleY(float) override;

    virtual void setFlipX(bool);
    virtual void setFlipY(bool);
};
class GameObject : public CCSpritePlus {
#else
class GameObject : public cocos2d::CCSprite {
#endif
public:
    static GameObject* objectFromString(std::string);

    virtual void update(float dt) override;
    virtual void setRotation(float rotation) override;
    virtual void setScaleX(float scaleX) override;
    virtual void setScaleY(float scaleY) override;
    virtual void setPosition(const cocos2d::CCPoint& point) override;
    virtual void setVisible(bool visible) override;
    virtual void setScale(float scale) override;
    virtual void setOpacity(GLubyte opacity) override;

#if GAME_VERSION >= GV_1_7
    virtual void setFlipX(bool flipX) override;
    virtual void setFlipY(bool flipY) override;
#endif
    virtual void resetObject();
    virtual void triggerObject();
    virtual void activateObject();
    virtual void deactivateObject();
    virtual cocos2d::CCRect getObjectRect();
    virtual cocos2d::CCRect getObjectRect(float scaleX, float scaleY);
    virtual cocos2d::CCRect getObjectRect2(float scaleX, float scaleY);
};
