#include "../hook.hpp"
#include "LevelSearchLayer.hpp"
#include "CCMenuItemToggler.hpp"

class MoreSearchLayer : public CCLayerColor {
public:
    CCLayer* m_mainLayer;
    CCMenu* m_buttonMenu;
    LevelSearchLayer* m_searchLayer;
    bool m_isValid;
    CCLabelBMFont* m_songLabel;
    int m_selectedSong;
    
    static MoreSearchLayer* create(::LevelSearchLayer* searchLayer) {
        auto ret = new MoreSearchLayer();
        if (ret && ret->init(searchLayer)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    // swallow touches
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        return true;
    }
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) {}
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) {}
    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) {}
    virtual void registerWithTouchDispatcher() {
        CCTouchDispatcher* dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        dispatcher->addTargetedDelegate(this, -128, true);
    }

    int getIntForKey(const char* key, int defaultValue = 0) {
        auto GLM = GameLevelManager::sharedState();
        auto dict = getSettingsDict(GLM);
        
        auto value = dict->valueForKey(key);
        if (value) {
            return value->intValue();
        }
        return defaultValue;
    }

    void setIntForKey(int value, const char* key) {
        auto GLM = GameLevelManager::sharedState();
        auto dict = getSettingsDict(GLM);
        
        auto strValue = CCString::createWithFormat("%d", value);
        
        std::string keyStr;
        keyStr.assign(key);
        
        dict->setObject(strValue, keyStr);
    }

    bool init(::LevelSearchLayer* searchLayer) {
        m_searchLayer = searchLayer;
        m_isValid = true;
        
        m_selectedSong = getIntForKey("selected_song_index", 0);
        
        if (m_selectedSong < 0) m_selectedSong = 0;
        if (m_selectedSong > 35) m_selectedSong = 35;
        if (!CCLayerColor::initWithColor({0, 0, 0, 150})) {
            return false;
        }
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        this->setKeypadEnabled(true);
        this->setTouchEnabled(true);
        m_mainLayer = CCLayer::create();
        if (!m_mainLayer) {
            return false;
        }
        this->addChild(m_mainLayer);

        auto bg = extension::CCScale9Sprite::create("GJ_square01.png", CCRectMake(0, 0, 80, 80));
        bg->setContentSize(CCSizeMake(440.f, 290.f));
        bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        m_mainLayer->addChild(bg, -1);

        auto title = CCLabelBMFont::create("Advanced Options", "goldFont.fnt");
        title->setPosition(ccp(winSize.width / 2, (winSize.height / 2) + 145.f - 20.f));
        title->setScale(0.7f);
        m_mainLayer->addChild(title, 20);
        m_buttonMenu = CCMenu::create();
        m_buttonMenu->setPosition(ccp(0, 0));
        m_mainLayer->addChild(m_buttonMenu, 10);

        auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(
            closeSpr,
            closeSpr,
            this,
            menu_selector(MoreSearchLayer::onClose)
        );
        closeBtn->setSizeMult(2.0f);
        closeBtn->setPosition(ccp((winSize.width / 2) - 220.f + 10.f, (winSize.height / 2) + 145.f - 10.f));
        m_buttonMenu->addChild(closeBtn);
        
        auto GLM = GameLevelManager::sharedState();
        
        bool uncompletedState = GLM->getBoolForKey("uncompleted_filter");
        createToggleButton(
            "Uncompleted",
            menu_selector(MoreSearchLayer::onUncompleted),
            uncompletedState,
            ccp((winSize.width / 2) - 140.f, (winSize.height / 2) + 70.f)
        );
        bool completedState = GLM->getBoolForKey("completed_filter");
        createToggleButton(
            "Completed",
            menu_selector(MoreSearchLayer::onCompleted),
            completedState,
            ccp((winSize.width / 2) + 60.f, (winSize.height / 2) + 70.f)
        );
        bool songFilterState = GLM->getBoolForKey("song_filter");
        createToggleButton(
            "Song",
            menu_selector(MoreSearchLayer::onSongFilter),
            songFilterState,
            ccp((winSize.width / 2) - 140.f, (winSize.height / 2) + 20.f)
        );
        createSongSelector(songFilterState);
        bool noStarState = GLM->getBoolForKey("nostar_filter");
        createToggleButton(
            "No Star",
            menu_selector(MoreSearchLayer::onNoStar),
            noStarState,
            ccp((winSize.width / 2) + 60.f, (winSize.height / 2) + 20.f)
        );
        return true;
    }
    void createSongSelector(bool isEnabled) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        float yPos = (winSize.height / 2) - 30.f;
        m_songLabel = CCLabelBMFont::create("", "bigFont.fnt");
        m_songLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        m_songLabel->setPosition(ccp(winSize.width / 2, yPos));
        m_mainLayer->addChild(m_songLabel);
        updateSongLabel();

        auto leftArrow = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
        auto leftBtn = CCMenuItemSpriteExtra::create(
            leftArrow,
            leftArrow,
            this,
            menu_selector(MoreSearchLayer::onSongPrevious)
        );
        leftBtn->setPosition(ccp((winSize.width / 2) - 120.f, yPos));
        leftBtn->setSizeMult(2.0f);
        m_buttonMenu->addChild(leftBtn);

        auto rightArrow = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
        auto rightBtn = CCMenuItemSpriteExtra::create(
            rightArrow,
            rightArrow,
            this,
            menu_selector(MoreSearchLayer::onSongNext)
        );

        rightBtn->setPosition(ccp((winSize.width / 2) + 120.f, yPos));
        rightBtn->setSizeMult(2.0f);
        m_buttonMenu->addChild(rightBtn);
        m_songLabel->setVisible(isEnabled);
        leftBtn->setVisible(isEnabled);
        rightBtn->setVisible(isEnabled);
    }
    void updateSongLabel() {
        if (!m_songLabel) {
            return;
        }
        const char* songTitle = LevelTools::getAudioTitle(m_selectedSong);
        
        auto labelText = CCString::createWithFormat("%02d: %s", m_selectedSong + 1, songTitle);
        
        m_songLabel->setString(labelText->getCString());
        float contentWidth = m_songLabel->getContentSize().width;
        
        float maxWidth = 200.0f;
        float maxScale = 0.8f;
        
        float scale;
        if (contentWidth > 0.0f && (maxWidth / contentWidth) < maxScale) {
            scale = maxWidth / contentWidth;
        } else {
            scale = maxScale;
        }
        
        m_songLabel->setScale(scale);
    }
    void selectSong(int songIndex) {
        if (songIndex < 0) songIndex = 0;
        if (songIndex > 35) songIndex = 35;
        
        m_selectedSong = songIndex;
        
        setIntForKey(m_selectedSong, "selected_song_index");
        
        updateSongLabel();
    }
    void createToggleButton(const char* text, SEL_MenuHandler callback, bool toggleState, CCPoint position) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        
        auto toggler = CCMenuItemToggler::create(
            toggleOff,
            toggleOn,
            this,
            callback
        );
        toggler->toggle(toggleState);
        toggler->setPosition(position);
        m_buttonMenu->addChild(toggler);
        
        auto label = CCLabelBMFont::create(text, "bigFont.fnt");
        label->setAnchorPoint(ccp(0.f, 0.5f));
        label->setPosition(ccp(position.x + 25.f, position.y));
        label->setScale(0.4f);
        m_mainLayer->addChild(label);
    }
    void onUncompleted(CCObject* sender) {
        auto GLM = GameLevelManager::sharedState();
        bool currentState = GLM->getBoolForKey("uncompleted_filter");
        GLM->setBoolForKey(!currentState, "uncompleted_filter");
    }
    void onCompleted(CCObject* sender) {
        auto GLM = GameLevelManager::sharedState();
        bool currentState = GLM->getBoolForKey("completed_filter");
        GLM->setBoolForKey(!currentState, "completed_filter");
    }
    void onSongFilter(CCObject* sender) {
        auto GLM = GameLevelManager::sharedState();
        bool currentState = GLM->getBoolForKey("song_filter");
        bool newState = !currentState;
        GLM->setBoolForKey(newState, "song_filter");
        if (m_songLabel) {
            m_songLabel->setVisible(newState);
        } else {
        }
        
        auto children = m_buttonMenu->getChildren();
        if (children) {
            CCObject* obj;
            CCARRAY_FOREACH(children, obj) {
                auto child = dynamic_cast<CCMenuItemSpriteExtra*>(obj);
                if (child) {
                    auto sprite = dynamic_cast<CCSprite*>(child->getNormalImage());
                    if (sprite && sprite->displayFrame()) {
                        auto pos = child->getPosition();
                        auto winSize = CCDirector::sharedDirector()->getWinSize();
                        float yPos = (winSize.height / 2) - 30.f;
                        if (abs(pos.y - yPos) < 5.0f) {
                            child->setVisible(newState);
                        }
                    }
                }
            }
        } else {
        }
    }
    
    void onNoStar(CCObject* sender) {
        auto GLM = GameLevelManager::sharedState();
        bool currentState = GLM->getBoolForKey("nostar_filter");
        GLM->setBoolForKey(!currentState, "nostar_filter");
    }
    void onSongPrevious(CCObject* sender) {
        if (m_selectedSong > 0) {
            selectSong(m_selectedSong - 1);
        }
    }
    void onSongNext(CCObject* sender) {
        if (m_selectedSong < 35) {
            selectSong(m_selectedSong + 1);
        }
    }
    void onClose(CCObject* sender) {
        m_isValid = false;
        m_searchLayer = nullptr;
        this->removeFromParentAndCleanup(true);
    }
    void show() {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (scene) {
            scene->addChild(this, 999);
        } else {
        }
    }
    virtual void keyBackClicked() {
        onClose(nullptr);
    }
};

class LevelSearchLayerExt {
public:
    void onAdvancedOptions(CCObject* sender) {
        auto searchLayer = static_cast<::LevelSearchLayer*>(sender);
        auto moreSearchLayer = MoreSearchLayer::create(searchLayer);
        if (moreSearchLayer) {
            moreSearchLayer->show();
        } else {
        }
    }
};

bool (*TRAM_LevelSearchLayer_init)(LevelSearchLayer* self);
bool LevelSearchLayer_init(LevelSearchLayer* self) {
    if (!TRAM_LevelSearchLayer_init(self)) {
        return false;
    }
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCMenu* settingsMenu = CCMenu::create();
    
    CCSprite* settingsSpr = CCSprite::create(MORE_FILTERS_TEXTURE);
    CCMenuItemSpriteExtra* settingsBtn = CCMenuItemSpriteExtra::create(
        settingsSpr, 
        settingsSpr, 
        self, 
        menu_selector(LevelSearchLayerExt::onAdvancedOptions)
    );
    
    settingsBtn->setPosition(ccp(winSize.width - 30, winSize.height - 30));
    settingsMenu->addChild(settingsBtn);
    settingsMenu->setPosition(ccp(0, 0));
    
    self->addChild(settingsMenu);
    
    return true;
}

void NP4_LevelSearchLayer_om() {
    Omni::hook("_ZN16LevelSearchLayer4initEv",
        reinterpret_cast<void*>(LevelSearchLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelSearchLayer_init));
}