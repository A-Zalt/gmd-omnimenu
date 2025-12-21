#pragma once
#ifndef _LAYERS_EDITOR_LEVELEDITORLAYER_HPP
#define _LAYERS_EDITOR_LEVELEDITORLAYER_HPP

#include <cocos2d.h>

#include <robtop/objects/levelsettingsobject.hpp>
#include <robtop/layers/editor/drawgridlayer.hpp>

class LevelEditorLayer : public cocos2d::CCLayer {
public:
		bool init(GJGameLevel*);
		void updateVisibility(float);

		CCLayer* getGameLayer() const;
		LevelSettingsObject* getLevelSettings() const;

		DrawGridLayer* getGridLayer() const;
};

#endif
