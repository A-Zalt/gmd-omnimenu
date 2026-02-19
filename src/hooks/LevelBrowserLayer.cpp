#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "LevelBrowserLayer.hpp"
#include "ButtonSprite.hpp"

#if GAME_VERSION >= GV_1_4

void LevelBrowserLayer::onImport() {
    JNIEnv* env = getEnv();
    if (!env || env == nullptr) {
        GDSHARE_FL("Error: could not get JNI Environment");
        return;
    }
    jclass activityClass = env->FindClass(JAVA_PATH_MAIN "/GeometryJump");
    if (activityClass == nullptr) {
        GDSHARE_FL("Error: could not get the activity class");
        return;
    }
    jmethodID showPicker = env->GetMethodID(activityClass, "showOpenFilePicker", "()V");
    if (showPicker == nullptr) {
        GDSHARE_FL("Error: could not find showOpenFilePicker method. Are you sure you changed the smali?");
        return;
    }
    jmethodID getActivity = env->GetStaticMethodID(activityClass, "getInstance", "()L" JAVA_PATH_MAIN "/GeometryJump;");
    if (getActivity == nullptr) {
        GDSHARE_FL("Error: could not find getInstance method. Are you sure you changed the smali?");
        return;
    }
    jobject activity = env->CallStaticObjectMethod(activityClass, getActivity);
    env->CallVoidMethod(activity, showPicker);
}

void LevelBrowserLayer::FLAlert() {
    HaxManager& hax = HaxManager::sharedState();
    switch (hax.gdShareMessageID) {
        case 1:
            GDSHARE_FL("Error: could not load level");
            break;
        case 2:
            if (hax.gdShareData != 0) {
                GDSHARE_FL(fmt::format(
                    "Error: level was made for a newer version of Geometry Dash ({} > {})",
                    intToReadableGV(hax.gdShareData),
                    READABLE_GAME_VERSION
                ).c_str());
            } else {
                GDSHARE_FL("Error: level was made for a newer version of Geometry Dash");
            }
            break;
        default:
            GDSHARE_FL("Unknown error");
            break;
    }
    hax.gdShareMessageID = 0;
}
void LevelBrowserLayer::reload() {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, LevelBrowserLayer::scene(getSearchObject(this))));
}

void LevelBrowserLayer::loadLevel(GJGameLevel* level) {
    HaxManager& hax = HaxManager::sharedState();
    if (level == nullptr) {
        hax.gdShareMessageID = 1;
        runAction(CCSequence::create(
            CCDelayTime::create(0.2f),
            CCCallFunc::create(this, callfunc_selector(LevelBrowserLayer::FLAlert)),
            nullptr
        ));
        // GDSHARE_FL("Error: could not load level");
        return;
    }
    if (level->m_nGameVersion > GAME_VERSION) {
        hax.gdShareMessageID = 2;
        hax.gdShareData = level->m_nGameVersion;
        runAction(CCSequence::create(
            CCDelayTime::create(0.2f),
            CCCallFunc::create(this, callfunc_selector(LevelBrowserLayer::FLAlert)),
            nullptr
        ));
        return;
    }
    level->m_nLevelID = 0;
    level->m_eLevelType = GJLevelType::Editor;
#ifdef FORCE_AUTO_SAFE_MODE
    level->m_bIsVerified = false;
    level->m_bIsDemon = false;
    level->m_nStars = 0;
#if GAME_VERSION >= GV_1_6
    level->m_bAutoLevel = false;
#endif
#endif
    getLocalLevels()->insertObject(level, 0);
    // this delay is necessary. TODO come up with a better solution?
    runAction(CCSequence::create(
        CCDelayTime::create(0.2f),
        CCCallFunc::create(this, callfunc_selector(LevelBrowserLayer::reload)),
        nullptr
    ));
    
}
#endif

class LevelBrowserLayerExt;

std::map<void*, LevelBrowserLayerExt*> extensionMap;

class JumpToPageLayer : public CCLayerColor, public TextInputDelegate {
public:
    LevelBrowserLayer* m_levelBrowserLayer;
    CCTextInputNode* m_input;
    CCLayer* m_mainLayer;
    CCMenu* m_buttonMenu;
    bool m_isValid;
    
    static JumpToPageLayer* create(LevelBrowserLayer* lbl) {
        auto ret = new JumpToPageLayer();
        if (ret && ret->init(lbl)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    bool init(LevelBrowserLayer* lbl) {
        m_levelBrowserLayer = lbl;
        m_isValid = true;
        if (!CCLayerColor::initWithColor({0, 0, 0, 105})) {
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

        auto bg = extension::CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
        bg->setContentSize({280.f, 160.f});
        bg->setPosition({winSize.width / 2, winSize.height / 2});
        m_mainLayer->addChild(bg);

        auto title = CCLabelBMFont::create("Go to Page", "goldFont.fnt");
        title->setPosition({winSize.width / 2, winSize.height / 2 + 55.f});
        title->setScale(0.8f);
        m_mainLayer->addChild(title);

        auto inputBg = extension::CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
        inputBg->setContentSize({200.f, 40.f});
        inputBg->setColor({123,60,31});
        inputBg->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
        m_mainLayer->addChild(inputBg);

        m_input = CCTextInputNode::create(180.f, 30.f, "Page", "Thonburi", 24, "bigFont.fnt");
        m_input->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
        m_input->setAllowedChars("0123456789");
        m_input->setDelegate(this);
        m_input->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
        m_input->setString(fmt::format("{}", getSearchPage(getSearchObject(lbl)) + 1).c_str());
        m_mainLayer->addChild(m_input);
        
        m_buttonMenu = CCMenu::create();
        m_buttonMenu->setPosition({0, 0});
        m_mainLayer->addChild(m_buttonMenu);

        auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(
            closeSpr,
            closeSpr,
            this,
            menu_selector(JumpToPageLayer::onClose)
        );
        closeBtn->setPosition({winSize.width / 2 - 130.f, winSize.height / 2 + 70.f});
        m_buttonMenu->addChild(closeBtn);

        auto goSpr = ButtonSprite::create("Go", 60, 0, 0.6f, true, "goldFont.fnt", "GJ_button_01.png");
        auto goBtn = CCMenuItemSpriteExtra::create(
            goSpr,
            goSpr,
            this,
            menu_selector(JumpToPageLayer::onGo)
        );
        goBtn->setPosition({winSize.width / 2, winSize.height / 2 - 45.f});
        m_buttonMenu->addChild(goBtn);

        auto firstPageBtn = CCMenuItemSpriteExtra::create(
            createDoubleArrowSprite(false),
            createDoubleArrowSprite(false),
            this,
            menu_selector(JumpToPageLayer::onFirstPage)
        );
        firstPageBtn->setPosition({winSize.width / 2 - 80.f, winSize.height / 2 - 45.f});
        m_buttonMenu->addChild(firstPageBtn);

        auto lastPageBtn = CCMenuItemSpriteExtra::create(
            createDoubleArrowSprite(true),
            createDoubleArrowSprite(true),
            this,
            menu_selector(JumpToPageLayer::onLastPage)
        );
        lastPageBtn->setPosition({winSize.width / 2 + 80.f, winSize.height / 2 - 45.f});
        m_buttonMenu->addChild(lastPageBtn);

        setupTouchPriority();
        return true;
    }
    
    void setupTouchPriority() {
        auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        
        dispatcher->removeDelegate(this);
        dispatcher->removeDelegate(m_buttonMenu);
        dispatcher->removeDelegate(m_input);

        m_input->setTouchEnabled(true);
        dispatcher->addTargetedDelegate(m_input, -130, true);
        
        m_buttonMenu->setTouchEnabled(true);
        dispatcher->addTargetedDelegate(m_buttonMenu, -129, true);
        
        dispatcher->addTargetedDelegate(this, -128, true);
    }
    
    CCSprite* createDoubleArrowSprite(bool flipX = true) {
        auto arrow1 = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        auto arrow2 = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        
        arrow1->setScale(0.7f);
        arrow2->setScale(0.7f);
        arrow1->setFlipX(flipX);
        arrow2->setFlipX(flipX);
        
        auto container = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        container->setOpacity(0);
        
        if (flipX) {
            arrow1->setPosition({container->getContentSize().width / 2 - 4, container->getContentSize().height / 2});
            arrow2->setPosition({container->getContentSize().width / 2 + 4, container->getContentSize().height / 2});
            container->addChild(arrow1);
            container->addChild(arrow2);
        } else {
            arrow1->setPosition({container->getContentSize().width / 2 + 4, container->getContentSize().height / 2});
            arrow2->setPosition({container->getContentSize().width / 2 - 4, container->getContentSize().height / 2});
            container->addChild(arrow1);
            container->addChild(arrow2);
        }
        
        return container;
    }
    
    void onFirstPage(CCObject* sender) {
        if (!m_isValid || !m_levelBrowserLayer) {
            onClose(nullptr);
            return;
        }
        GJSearchObject* searchObject = getSearchObject(m_levelBrowserLayer);
        
        getSearchPage(searchObject) = 0;
        m_levelBrowserLayer->loadPage(searchObject);
        onClose(nullptr);
    }
    
    void onLastPage(CCObject* sender) {
        if (!m_isValid || !m_levelBrowserLayer) {
            onClose(nullptr);
            return;
        }
        GJSearchObject* searchObject = getSearchObject(m_levelBrowserLayer);
        int total = getTotalLevels(m_levelBrowserLayer);
        
        if (total <= 0) {
            onClose(nullptr);
            return;
        }
        int lastPage = (total - 1) / 10;
        getSearchPage(searchObject) = lastPage;
        m_levelBrowserLayer->loadPage(searchObject);
        onClose(nullptr);
    }
    
    void onClose(CCObject* sender) {
        m_isValid = false;
        
        if (m_input) {
            m_input->onClickTrackNode(false);
        }
        
        auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        if (dispatcher) {
            dispatcher->removeDelegate(this);
            if (m_buttonMenu) {
                dispatcher->removeDelegate(m_buttonMenu);
            }
            if (m_input) {
                dispatcher->removeDelegate(m_input);
            }
        }
        
        m_levelBrowserLayer = nullptr;
        
        this->removeFromParentAndCleanup(true);
    }
    
    void onGo(CCObject* sender) {
        if (!m_isValid || !m_input || !m_levelBrowserLayer) {
            onClose(nullptr);
            return;
        }
        
        const char* inputStr = nullptr;
        
        CCArray* children = m_input->getChildren();
        if (children) {
            for (unsigned int i = 0; i < children->count(); i++) {
                CCObject* obj = children->objectAtIndex(i);
                if (!obj) continue;
                
                CCLabelBMFont* childLabel = dynamic_cast<CCLabelBMFont*>(obj);
                if (childLabel) {
                    const char* labelStr = childLabel->getString();
                    if (labelStr && strlen(labelStr) > 0) {
                        inputStr = labelStr;
                        break;
                    }
                }
            }
        }
        
        if (!inputStr || strlen(inputStr) == 0) {
            onClose(nullptr);
            return;
        }
        
        int pageNum = atoi(inputStr);
        if (pageNum < 1) pageNum = 1;
        pageNum--;
        
        int total = getTotalLevels(m_levelBrowserLayer);
        if (total > 0) {
            int lastPage = (total - 1) / 10;
            if (pageNum > lastPage) {
                pageNum = lastPage;
            }
        }
        
        GJSearchObject* searchObject = getSearchObject(m_levelBrowserLayer);
        if (searchObject) {
            getSearchPage(searchObject) = pageNum;
            m_levelBrowserLayer->loadPage(searchObject);
        }
        onClose(nullptr);
    }
    
    virtual void registerWithTouchDispatcher() override {}
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        return true;
    }
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override {}
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override {}
    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) override {}
    virtual void keyBackClicked() override {
        onClose(nullptr);
    }
    
    void show() {
#if GAME_VERSION < GV_1_7
        auto scene = CCDirector::sharedDirector()->getRunningScene();
#else
        auto scene = MEMBER_BY_OFFSET(CCScene*, CCDirector::sharedDirector(), 0x98);
#endif
        if (scene) {
            scene->addChild(this, 999);
        }
    }
};

class LevelBrowserLayerExt : public CCObject {
public:
    CCLabelBMFont* pageLabel;
    CCMenuItemSpriteExtra* pageButton;
    CCMenuItemSpriteExtra* randomPageButton;
    ButtonSprite* pageButtonSprite;
    CCMenu* pageMenu;
    LevelBrowserLayer* self;
    bool isInitialized;
    
    LevelBrowserLayerExt() : pageLabel(nullptr), pageButton(nullptr), 
                             randomPageButton(nullptr), pageButtonSprite(nullptr), 
                             pageMenu(nullptr), self(nullptr), isInitialized(false) {
    }
    
    virtual ~LevelBrowserLayerExt() {
        cleanup();
    }
    
    void cleanup() {
        isInitialized = false;
        pageLabel = nullptr;
        pageButton = nullptr;
        randomPageButton = nullptr;
        pageButtonSprite = nullptr;
        pageMenu = nullptr;
        self = nullptr;
    }
    
    static LevelBrowserLayerExt* get(LevelBrowserLayer* lbl) {
        if (!lbl) {
            return nullptr;
        }
        
        auto it = extensionMap.find(lbl);
        if (it != extensionMap.end()) {
            if (it->second && it->second->self == lbl) {
                return it->second;
            } else {
                if (it->second) {
                    it->second->cleanup();
                    it->second->release();
                }
                extensionMap.erase(it);
            }
        }
        
        auto ext = new LevelBrowserLayerExt();
        ext->autorelease();
        ext->retain();
        ext->self = lbl;
        extensionMap[lbl] = ext;
        return ext;
    }
    
    static void remove(LevelBrowserLayer* lbl) {
        if (!lbl) {
            return;
        }
        
        auto it = extensionMap.find(lbl);
        if (it != extensionMap.end()) {
            auto ext = it->second;
            if (ext) {
                ext->cleanup();
                ext->release();
            }
            extensionMap.erase(it);
        }
    }
    
    void onRandomPage(CCObject* sender) {
        if (!self || !isInitialized) {
            return;
        }
        
        auto it = extensionMap.find(self);
        if (it == extensionMap.end() || it->second != this) {
            return;
        }
        
        GJSearchObject* searchObject = getSearchObject(self);
        if (!searchObject) {
            return;
        }
        
        int total = getTotalLevels(self);
        if (total <= 0) {
            return;
        }
        
        int totalPages = (total + 9) / 10;
        if (totalPages <= 1) {
            return;
        }
        
        static bool seeded = false;
        if (!seeded) {
            srand(time(nullptr));
            seeded = true;
        }
        
        int randomPage = rand() % totalPages;
        getSearchPage(searchObject) = randomPage;
        self->loadPage(searchObject);
    }
    
    void onGoToPage(CCObject* sender) {
        if (!self || !isInitialized) {
            return;
        }
        
        auto it = extensionMap.find(self);
        if (it == extensionMap.end() || it->second != this) {
            return;
        }
        
        auto layer = JumpToPageLayer::create(self);
        if (layer) {
            layer->show();
        }
    }
    
    void updatePageLabel() {
        if (!isInitialized || !self || !pageButton) return;
        
        auto it = extensionMap.find(self);
        if (it == extensionMap.end() || it->second != this) {
            return;
        }
        
        GJSearchObject* searchObject = getSearchObject(self);
        if (!searchObject) return;
        
        int currentPage = getSearchPage(searchObject) + 1;
        
        std::stringstream ss;
        ss << currentPage;
        std::string pageText = ss.str();
        
        auto newSprite = ButtonSprite::create(pageText.c_str(), 20, 0, 0.6f, true, "bigFont.fnt", "GJ_button_02.png");
        if (newSprite) {
            pageButton->setNormalImage(newSprite);
            pageButton->setSelectedImage(newSprite);
            pageButtonSprite = newSprite;
        }
    }
};

void LevelBrowserLayer::onRefresh() {
    auto searcher = getSearchObject(this);
    if (!searcher) return;
#if GAME_VERSION > GV_1_0
    GameLevelManager::sharedState()->resetTimerForKey(searcher->getKey());
#else
    getKeyTimers()->removeObjectForKey(searcher->getKey());
#endif
    this->loadPage(searcher);
}

bool (*TRAM_LevelBrowserLayer_init)(LevelBrowserLayer* self, GJSearchObject* searcher);
bool LevelBrowserLayer_init(LevelBrowserLayer* self, GJSearchObject* searcher) {
    LevelBrowserLayerExt::remove(self);
    if (!TRAM_LevelBrowserLayer_init(self, searcher)) return false;
    HaxManager& hax = HaxManager::sharedState();

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto ext = LevelBrowserLayerExt::get(self);
    if (!ext) {
        return false;
    }
    ext->isInitialized = false;

#if GAME_VERSION >= GV_1_4
    if (hax.getModuleEnabled(ModuleID::GDSHARE) && getSearchType(searcher) == 98) {
        hax.levelBrowserLayer = self;
        CCMenu* importMenu = CCMenu::create();
        CCSprite* importSpr = cocos2d::CCSprite::create("gdshare_import.png");
        CCMenuItemSpriteExtra* importBtn = CCMenuItemSpriteExtra::create(importSpr, importSpr, self, menu_selector(LevelBrowserLayer::onImport));

        self->addChild(importMenu, 1000);
        importMenu->addChild(importBtn);
        importMenu->setPosition(ccp(winSize.width - 30.f, 90.f));
    }
#endif
    if (hax.getModuleEnabled(ModuleID::PAGE_REFRESH) && getSearchType(searcher) != 98 && getSearchType(searcher) != 99) {
        CCMenu* refreshMenu = CCMenu::create();
        CCSprite* refreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
        CCMenuItemSpriteExtra* refreshBtn = CCMenuItemSpriteExtra::create(refreshSpr, refreshSpr, self, menu_selector(LevelBrowserLayer::onRefresh));

        self->addChild(refreshMenu, 1000);
        refreshMenu->addChild(refreshBtn);
        refreshMenu->setPosition(ccp(winSize.width - 30.f, 30.f));
    }
    if (hax.getModuleEnabled(ModuleID::PAGE_CONTROLS)) {
        ext->pageMenu = CCMenu::create();
    
        if (ext->pageMenu) {
            ext->pageButtonSprite = ButtonSprite::create("1", 20, 0, 0.6f, true, "bigFont.fnt", "GJ_button_02.png");
            
            if (ext->pageButtonSprite) {
                ext->pageButton = CCMenuItemSpriteExtra::create(
                    ext->pageButtonSprite, 
                    ext->pageButtonSprite, 
                    ext, 
                    menu_selector(LevelBrowserLayerExt::onGoToPage)
                );
                
                if (ext->pageButton) {
                    ext->pageMenu->addChild(ext->pageButton);
                    ext->pageMenu->setPosition({0, 0});
                    ext->pageButton->setPosition(ccp(winSize.width - 25.f, winSize.height - 50.f));
                    self->addChild(ext->pageMenu, 1001);
                }
            }

            auto randomSpr = CCSprite::create(RANDOM_BUTTON_TEXTURE);
            if (randomSpr) {
                ext->randomPageButton = CCMenuItemSpriteExtra::create(
                    randomSpr,
                    randomSpr,
                    ext,
                    menu_selector(LevelBrowserLayerExt::onRandomPage)
                );
                
                if (ext->randomPageButton) {
                    ext->randomPageButton->setPosition(ccp(winSize.width - 25.f, winSize.height - 90.f));
                    ext->pageMenu->addChild(ext->randomPageButton);
                }
            }
        }
        
        ext->isInitialized = true;
        ext->updatePageLabel();
    }
    return true;
}
void (*TRAM_LevelBrowserLayer_loadPage)(LevelBrowserLayer* self, GJSearchObject* obj);
void LevelBrowserLayer_loadPage(LevelBrowserLayer* self, GJSearchObject* obj) {
    TRAM_LevelBrowserLayer_loadPage(self, obj);

    auto ext = LevelBrowserLayerExt::get(self);
    if (ext && ext->isInitialized) {
        ext->updatePageLabel();
    }
}
void (*TRAM_LevelBrowserLayer_destructor)(LevelBrowserLayer*);
void LevelBrowserLayer_destructor(LevelBrowserLayer* self) {
    LevelBrowserLayerExt::remove(self);
    TRAM_LevelBrowserLayer_destructor(self);
}

#if GDPS == GDPS_NEOPOINTFOUR
static std::string currentCacheKey;

const char* (*TRAM_GJSearchObject_getKey)(GJSearchObject* self);
const char* GJSearchObject_getKey(GJSearchObject* self) {
    const char* originalKey = TRAM_GJSearchObject_getKey(self);
    
    auto GLM = GameLevelManager::sharedState();
    if (!GLM) return originalKey;
    
    bool uncompletedFilter = GLM->getBoolForKey("uncompleted_filter");
    bool completedFilter = GLM->getBoolForKey("completed_filter");
    bool songFilter = GLM->getBoolForKey("song_filter");
    bool noStarFilter = GLM->getBoolForKey("nostar_filter");
    
    if (!uncompletedFilter && !completedFilter && !songFilter && !noStarFilter) {
        return originalKey;
    }
    
    currentCacheKey = originalKey;
    
    if (uncompletedFilter) currentCacheKey += "_uc";
    if (completedFilter) currentCacheKey += "_c";
    if (noStarFilter) currentCacheKey += "_ns";
    
    if (songFilter) {
        auto dict = getSettingsDict(GLM);
        std::string keyStr("selected_song_index");
        auto value = dict->valueForKey(keyStr);
        int songID = 0;
        if (value) {
            songID = value->intValue() + 1;
        }
        
        std::stringstream ss;
        ss << "_s" << songID;
        currentCacheKey += ss.str();
    }

    return currentCacheKey.c_str();
}
#endif

void LevelBrowserLayer_om() {
    Omni::hook("_ZN17LevelBrowserLayer4initEP14GJSearchObject",
        reinterpret_cast<void*>(LevelBrowserLayer_init),
        reinterpret_cast<void**>(&TRAM_LevelBrowserLayer_init));
    Omni::hook("_ZN17LevelBrowserLayer8loadPageEP14GJSearchObject",
        reinterpret_cast<void*>(LevelBrowserLayer_loadPage),
        reinterpret_cast<void**>(&TRAM_LevelBrowserLayer_loadPage));
    Omni::hook("_ZThn296_N17LevelBrowserLayerD1Ev",
        reinterpret_cast<void*>(LevelBrowserLayer_destructor),
        reinterpret_cast<void**>(&TRAM_LevelBrowserLayer_destructor));
#if GDPS == GDPS_NEOPOINTFOUR
    Omni::hook("_ZN14GJSearchObject6getKeyEv",
        reinterpret_cast<void*>(GJSearchObject_getKey),
        reinterpret_cast<void**>(&TRAM_GJSearchObject_getKey));
#endif
}