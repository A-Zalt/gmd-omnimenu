#ifndef _LEVELEDITORLAYER_HPP
#define _LEVELEDITORLAYER_HPP

#include <cocos2d.h>
#include <sprite_nodes/GameObject.hpp>

class DrawGridLayer : public cocos2d::CCLayer
{
public:
    virtual ~DrawGridLayer();
    virtual void draw();
    void addToEffects(GameObject*);
    void removeFromEffects(GameObject*);
};
class GJGameLevel;

class ColorPickerPopup;
class ColorPickerDelegate
{
public:
    virtual void colorPickerClosed(ColorPickerPopup *);
};

enum UndoCommand {
    Delete = 1
};

class UndoObject : public CCNode {
public:
    GameObject* m_pObject;
    UndoCommand m_pCommand;

    static UndoObject* create(GameObject*, UndoCommand);
    bool init(GameObject*, UndoCommand);
    virtual ~UndoObject();
    virtual GameObject* getObject();
    virtual UndoCommand getCommand();
};

class LevelSettingsObject : public CCObject
{
public:
    static LevelSettingsObject* objectFromString(std::string);
};

using namespace cocos2d;

class LevelEditorLayer : public CCLayer, public ColorPickerDelegate
{
public:
    CCSpriteBatchNode *m_pBatchNode;
    // CCArray<CCArray<GameObject*>>
    CCArray *m_obSectionArray;
    CCArray *m_obUndoList;
    CCArray *m_obRedoList;
    DrawGridLayer *m_pGrid;
    int m_nObjectCount;
    CCLayer *m_pLayer;
    GJGameLevel *m_pLevel;
    LevelSettingsObject *m_pLevelSettings;
    unsigned int m_nDistance;

    static LevelEditorLayer* create(GJGameLevel*);
    
    virtual ~LevelEditorLayer();
    virtual void levelSettingsUpdated();
    virtual int getObjectCount() { return m_nObjectCount; }
    virtual void setObjectCount(int obj_count) { m_nObjectCount = obj_count; }
    virtual CCLayer* getGameLayer() { return m_pLayer; }
    virtual GJGameLevel* getLevel() { return m_pLevel; }
    virtual void* getLevelSettings() { return m_pLevelSettings; }
    virtual unsigned int getLevelDistance() { return m_nDistance; }

    bool init(GJGameLevel* level);
    void createObjectsFromSetup(std::string);

    void addToSection(GameObject* obj);
    void addToUndoList(UndoObject* obj);
    void removeObjectFromSection(GameObject* obj);
    void removeObject(GameObject* obj);
};

#endif