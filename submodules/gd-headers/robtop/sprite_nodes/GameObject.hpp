#ifndef _GAMEOBJECT_HPP
#define _GAMEOBJECT_HPP

#include <cocos2d.h>
#include <string>

using namespace cocos2d;

class GameObject : public CCSprite
{
public:
	float m_fScale;
	bool m_bUnk9000;
	bool m_bUnk3000;
	bool m_bPoweredOn;
	bool m_bUnk;
	float m_fWidth;
	float m_fHeight;
	bool d;
	bool m_bUnk2;
	char pad[47];
	bool m_bIsRotated;
	float m_fScaleModX;
	float m_fScaleModY;
	int m_nM_ID;
	int m_unkType;
	int m_nSectionIdx;
	bool m_bShouldSpawn;
	char pad3[3];
	CCPoint *m_pStartPos;
	char pad2[28];
	std::string m_stTextureName;
	char m_eBlendAdditive;
	bool m_bUsePlayerColor;
	bool m_bUsePlayerColor2;
	bool m_bIsDisabled;
	bool m_bUseAudioScale;
	float m_fStartRotation;
	float m_fStartScaleX;
	float m_fStartScaleY;
	bool m_bShouldHide;
	float m_fSpawnXPos;
	bool m_bIsInvisible;
	float m_fEnterAngle;
	int m_nEnterEffect;
	ccColor3B m_obTintColor;
	float m_fTintDuration;
	int m_nObjectKey;
	bool m_bDontTransform;
	bool m_bHasBeenActivated;
	bool m_bStateVar;
	int m_nObjectZ;
	CCNode *m_pObjectParent;

	static GameObject* objectFromString(std::string);
	
	virtual ~GameObject();

	virtual void update(float dt);
	virtual void setRotation(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
	virtual void setPosition(const CCPoint&);
	virtual void setVisible(bool);
	virtual void setOpacity(unsigned char);
	virtual void setScale(float);

	virtual void resetObject();
	virtual void triggerObject();
	virtual void activateObject();
	virtual void deactivateObject();
	
	virtual const CCRect getObjectRect();
	virtual const CCRect getObjectRect(float, float);

	virtual const CCPoint getRealPosition();
	virtual void setStartPos(CCPoint);
	virtual const std::string getSaveString();

	virtual void setFlipX(bool);
	virtual void setFlipY(bool);

	virtual void calculateSpawnXPos();
	virtual void triggerActivated();
	virtual void powerOnObject();
	virtual void powerOffObject();

	virtual bool getIsRotated();
	
	// obvious synthesizers
	virtual float getScaleModX();
	virtual void setScaleModX(float);

	virtual float getScaleModY();
	virtual void setScaleModY(float);

	virtual int getM_ID();
	virtual void setM_ID(int);

	virtual int /*unk*/ getType();

	virtual int getSectionIdx();
	virtual void setSectionIdx(int);

	virtual bool /*unk*/ getShouldSpawn();
	virtual	const CCPoint getStartPos();
	virtual void* /*unk*/ getFrame();
	virtual void* /*unk*/ getBlendAdditive();
	
	virtual bool getUsePlayerColor();
	virtual bool getUsePlayerColor2();
	virtual bool getIsDisabled();

	virtual bool getUseAudioScale();
	virtual void setUseAudioScale(bool);

	virtual float getStartRotation();
	virtual void setStartRotation(float);

	virtual float getStartScaleX();
	virtual void setStartScaleX(float);

	virtual float getStartScaleY();
	virtual void setStartScaleY(float);

	virtual bool getShouldHide();
	virtual void setShouldHide(bool);

	virtual float /*unk*/ getSpawnXPos();
	virtual bool getIsInvisible();
	
	virtual float getEnterAngle();
	virtual void setEnterAngle(float);
	
	virtual int getEnterEffect();
	virtual void setEnterEffect(int);

	virtual ccColor3B /*unk*/ getTintColor();
	virtual void setTintColor(ccColor3B);

	virtual float getTintDuration();
	virtual void setTintDuration(float);

	virtual int getObjectKey();
	virtual void setObjectKey(int);

	virtual bool getDontTransform();
	virtual void setDontTransform(bool);

	virtual bool /*unk*/ getHasBennActivated();

	virtual bool getStateVar();
	virtual void setStateVar(bool);

	virtual int getObjectZ();
	virtual void setObjectZ(int);

	virtual CCNode* getObjectParent();
	virtual void setObjectParent(CCNode*);
};

#endif
