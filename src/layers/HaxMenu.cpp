// massive thanks to adelfa for helping me figure out this touch dispatcher bullshit

#include "../layers/HaxMenu.hpp"
// #include "../layers/CustomScrollLayer.hpp"
#include "VersionUtils.hpp"
#include "FLAlertLayer.hpp"
#include "HaxManager.hpp"
#include "LocalLevelManager.hpp"
#include "CCMenuItemToggler.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "Utils.hpp"
#include "GameSoundManager.hpp"
#include "ButtonSprite.hpp"
#include "RespawnTimeInput.hpp"
#include <fmt/format.h>
#include <sys/stat.h>
#include <errno.h>
#include "AppDelegate.hpp"
#include <fstream>
#include "LoadingLayer.hpp"
#include "HaxButton.hpp"

using namespace cocos2d;

RespawnTimeInput* RespawnTimeInput::create(HaxMenu* parent) {
    auto ret = new RespawnTimeInput;
    if (ret->init(parent)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
bool RespawnTimeInput::init(HaxMenu* parent) {
    if (!CCLayer::init()) return false;
    this->parent = parent;

    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    // auto clock = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
    // clock->setPosition(ccp(winSize.width - 105, winSize.height - 53));
    // clock->setScale(0.75);
    // addChild(clock);

    this->input = CCTextInputNode::create(90.0, 40.0, "Respawn Time", "Thonburi", 12, "bigFont.fnt");
    input->setPosition(ccp(winSize.width / 2 + 40, winSize.height - 20));
    input->setMaxLabelScale(0.7);
    input->setLabelPlaceholderScale(0.4);
    setCharLimit(input, 5);
    input->setAllowedChars("0123456789.");
    input->setAnchorPoint({0, 0.5});
    input->setLabelPlaceholderColor(ccc3(127, 127, 127));
    input->setString(fmt::format("{:.2f}", hax.respawnTime).c_str());
    addChild(input);

    auto bg = extension::CCScale9Sprite::create("square02_small.png", CCRectMake(0,0,40,40));
    bg->setContentSize(CCSizeMake(90, 20));
    bg->_setZOrder(-1);
    bg->setPosition({winSize.width / 2 + 39, winSize.height - 20});
    addChild(bg);
    bg->setScale(0.9);

    return true;
}

void RespawnTimeInput::textChanged(CCTextInputNode* node) {
    HaxManager& hax = HaxManager::sharedState();
    if (!node) return;

    auto booba = *(CCTextFieldTTF**)((char*)node + CCTextInputNode__m_textField);
    if (!booba) return;
    if (!booba->m_pInputText) return;
    auto bro = booba->m_pInputText->c_str();
    if (!bro || bro == nullptr) {
        return;
    }

    float value = std::atof(bro);
    if (value >= 0 && value <= 500) {
        hax.respawnTime = value;
    }
}

void RespawnTimeInput::setupDelegates() {
    if (input) {
        setTextInputDelegate(input, this);
    }
}

HaxMenu* HaxMenu::create(CCLayer* referrer) {
    auto ret = new HaxMenu();
    if (ret->init(referrer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

// CCScene* HaxMenu::scene() {
//     auto layer = HaxMenu::create();
//     auto scene = CCScene::create();
//     scene->addChild(layer);
//     return scene;
// }

float HaxMenu::getDuration() {
    auto& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::FAST_MENU)) return 0.f;
    return 0.25f;
}

ccColor3B color = ccc3(127, 255, 255);

// bool CCRectContainsPoint(CCRect rect, const CCPoint& point)
// {
//     if (point.x >= CCRect::CCRectGetMinX(rect) && point.x < CCRect::CCRectGetMaxX(rect)
//         && point.y >= CCRect::CCRectGetMinY(rect) && point.y < CCRect::CCRectGetMaxY(rect)) {
//         return true;
//     }
//     return false;
// }


bool HaxMenu::init(CCLayer* referrer) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180)))
        return false;

    canSpoofPih = true;

    auto& hax = HaxManager::sharedState();

    this->referrer = referrer;
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    CCTouchDispatcher* touchDispatch = director->getTouchDispatcher();
    touchDispatch->setForcePrio(true);
    touchDispatch->setTargetPrio(0x80000002);

    this->catButtons = CCArray::create();
    this->catButtons->retain();

    CCNode* leftParent = CCNode::create();
    addChild(leftParent);
    leftParent->setPosition(-100.f, winSize.height / 2);
    this->leftParent = leftParent;
    
    CCSprite* leftPanel = CCSprite::create("menupanel.png");
    leftParent->addChild(leftPanel);
    leftPanel->setPosition({0.f, 0.f});
    leftPanel->setScaleY(2.0f);
    this->leftPanel = leftPanel;

    auto logo = CCSprite::create("omnimenu_logo.png");
    leftParent->addChild(logo, 1001);
    logo->setPosition(ccp(0, winSize.height / 2 - 30));
    logo->setScale(0.8f);
    
    leftParent->runAction(CCEaseOut::create(
        CCMoveTo::create(getDuration(), ccp(100.0f, winSize.height / 2)), 3
    ));

    auto rightParent = CCNode::create(); // CustomScrollLayer::create();
    addChild(rightParent);
    rightParent->setPosition(winSize.width + 100.f, winSize.height / 2);
    this->rightParent = rightParent;
    
    CCSprite* rightPanel = CCSprite::create("menupanel.png");
    rightParent->addChild(rightPanel);
    rightPanel->setPosition({0.f, 0.f});
    rightPanel->setScaleY(2.0f);
    this->rightPanel = rightPanel;
    
    rightParent->runAction(CCEaseOut::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width - 100.f, winSize.height / 2)), 3
    ));

    this->catMenu = CCMenu::create();
    this->leftParent->addChild(catMenu, 1001);
    catMenu->setPosition(ccp(-75, 0));

    this->modMenu = CCMenu::create();
    addChild(modMenu, 1002);
    this->modMenu->setPosition(this->rightParent->getPosition());

    modMenu->runAction(CCEaseOut::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width - 80.f, winSize.height / 2)), 3
    ));
    // modMenu->setPosition(ccp(winSize.width - 75, 0));

    addButton(" Player ", 14, 80, this, menu_selector(HaxMenu::onPlayer));
    addButton(" Visual ", 14, 60, this, menu_selector(HaxMenu::onVisual));
    addButton(" Editor ", 14, 40, this, menu_selector(HaxMenu::onEditor));
    addButton(" Bypass ", 14, 20, this, menu_selector(HaxMenu::onBypass));
    addButton(" Informational ", 14, 0, this, menu_selector(HaxMenu::onInformational));
    addButton(" Universal ", 14, -20, this, menu_selector(HaxMenu::onUniversal));
    addButton(" Label ", 14, -40, this, menu_selector(HaxMenu::onLabel));
    addButton(" Particles ", 14, -60, this, menu_selector(HaxMenu::onParticles));
    addButton(" Save Data ", 14, -80, this, menu_selector(HaxMenu::onSaveData));

    setTouchEnabled(true);
    setKeypadEnabled(true);
    // referrer->setTouchEnabled(false);
    // referrer->setScale(0.2f);

    if (hax.lastCategory == ModuleCategory::SaveData) {
        onSaveData(dummy_first_sender_param_1_7);
    } else {
        onCategory(hax.lastCategory);
    }

    return true;
}

void HaxMenu::setColorAtIndex(int ind) {
    int index = ind + 1;
    if (ind == 7) index = 1;
    else if (ind == 0 || ind == 8) index = ind;

    static_cast<CCLabelTTF*>(static_cast<CCMenuItemLabel*>(this->catButtons->objectAtIndex(index))->getLabel())->setColor(color);

#if GAME_VERSION < GV_1_7
    for (int i = 0; i < this->catButtons->count(); i++) {
        if (i == index) continue;
        static_cast<CCLabelTTF*>(static_cast<CCMenuItemLabel*>(this->catButtons->objectAtIndex(i))->getLabel())->setColor(ccWHITE);
    }
#else
    for (int i = 0; i < this->catButtons->count(); i++) {
        if (i == index) continue;

        auto mLabel = static_cast<CCMenuItemLabel*>(this->catButtons->objectAtIndex(i));
        if (!mLabel) continue;
        auto ttfLabel = static_cast<CCLabelTTF*>(MEMBER_BY_OFFSET(CCNode*, mLabel, 0x10C));
        ttfLabel->setColor(ccWHITE);
    }
#endif
}

void HaxMenu::cleanupMembers() {
    auto& hax = HaxManager::sharedState();
    while (this->rightParent->getChildrenCount() > 1) {
        CCNode* node = static_cast<CCNode*>(this->rightParent->getChildren()->objectAtIndex(1));
        this->rightParent->removeChild(node, true);
    }
    while (this->modMenu->getChildrenCount() > 0) {
        if (this->modMenu->getChildrenCount() > 1) {
            CCNode* node2 = static_cast<CCNode*>(this->modMenu->getChildren()->objectAtIndex(1));
            this->modMenu->removeChild(node2, true);
        }
        CCNode* node = static_cast<CCNode*>(this->modMenu->getChildren()->objectAtIndex(0));
        std::string* userData = static_cast<std::string*>(node->getUserData());
        delete userData;
        userData = nullptr;
        this->modMenu->removeChild(node, true);
    }
    if (hax.respawnInput && hax.respawnInput->input) {
        setTextInputDelegate(hax.respawnInput->input, nullptr);
        hax.respawnInput->removeFromParentAndCleanup(true);
    }
    hax.respawnInput = nullptr;
}

void HaxMenu::addButton(const char* text, float fontSize, float yOffset, CCObject* target, SEL_MenuHandler selector) {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    auto label = CCLabelTTF::create(text, "Helvetica-Oblique.ttf", fontSize);
    auto item = CCMenuItemLabel::create(label, target, selector);
    this->catButtons->addObject(item);
    this->catMenu->addChild(item, 1002);
    item->setAnchorPoint({0, 0.5});
    item->setPosition(ccp(-25, 10 + yOffset));
}
void HaxMenu::onPlayer(SEL_MenuHandler_1_7_compat) {
    onCategory(ModuleCategory::Player);
}
void HaxMenu::onVisual(SEL_MenuHandler_1_7_compat) {
    onCategory(ModuleCategory::Visual);
}
void HaxMenu::onEditor(SEL_MenuHandler_1_7_compat) {
    onCategory(ModuleCategory::Editor);
}
void HaxMenu::onBypass(SEL_MenuHandler_1_7_compat) {
    onCategory(ModuleCategory::Bypass);
}
void HaxMenu::onInformational(SEL_MenuHandler_1_7_compat) {
    onCategory(ModuleCategory::Informational);
}
void HaxMenu::onUniversal(SEL_MenuHandler_1_7_compat) {
    onCategory(ModuleCategory::Universal);
}
void HaxMenu::onLabel(SEL_MenuHandler_1_7_compat) {
    onCategory(ModuleCategory::Label);
}
void HaxMenu::onParticles(SEL_MenuHandler_1_7_compat) {
    onCategory(ModuleCategory::Particles);
}
void HaxMenu::onSaveData(SEL_MenuHandler_1_7_compat) {
    auto& hax = HaxManager::sharedState();
    hax.lastCategory = ModuleCategory::SaveData;
    setColorAtIndex(8);
    cleanupMembers();
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    auto label = CCLabelTTF::create("Create Backup ", "Helvetica-Oblique.ttf", 12);
    auto item = CCMenuItemLabel::create(label, this, menu_selector(HaxMenu::onExportData));
    this->modMenu->addChild(item, 1003);
    item->setAnchorPoint({0.f, 0.5f});
    item->setPosition(ccp(-98, winSize.height / 2 - 20));

    int y = -45;
    for (int i = 1; i <= NUMBER_OF_BACKUPS; i++) {
        auto label1 = CCLabelTTF::create(fmt::format("Load Backup #{} ", i).c_str(), "Helvetica-Oblique.ttf", 12);
        auto item1 = CCMenuItemLabel::create(label1, this, menu_selector(HaxMenu::onImportData));
        this->modMenu->addChild(item1, 1003);
        item1->setAnchorPoint({0.f, 0.5f});
        item1->setPosition(ccp(-98, winSize.height / 2 + y));
        item1->setTag(i);
        if (hax.backupDates[i-1] != 0) {
            const long int date = hax.backupDates[i-1];
            auto label2 = CCLabelTTF::create(
                fmt::format("Created {} ", 
                    std::asctime(std::localtime(&date))
                ).c_str(),
                "Helvetica-Oblique.ttf", 
                10);
            this->rightParent->addChild(label2);
            label2->setColor(ccc3(180, 180, 180));
            label2->setAnchorPoint({0.f, 0.5f});
            label2->setPosition(ccp(-95, winSize.height / 2 + y - 20));
        }
        y -= 30;
    }
    // this->rightParent->m_height = 50;
    // auto label2 = CCLabelTTF::create(fmt::format("Backup Key: {}").c_str(), "Helvetica-Oblique.ttf", 11);
    // label2->setPosition(ccp(38, winSize.height / 2 - 20));
}
void HaxMenu::onExportData(SEL_MenuHandler_1_7_compat) {
    auto& hax = HaxManager::sharedState();
    auto result = hax.createBackup();
    if (result.errorCode == -1) {
        FLAlertLayer::create(
            nullptr,
            "Export Data",
            "<cr>Error:</c> could not open file for writing",
            "OK",
            nullptr,
            300.f
        )->show();
    } else {
        #if GAME_VERSION > GV_1_0
            FLAlertLayer::create(
                nullptr,
                "Export Data",
                fmt::format("Data has been exported to: CCGameManager_backup_{}.dat and CCLocalLevels_backup_{}.dat", result.randNum, result.randNum).c_str(),
                "OK",
                nullptr,
                300.f
            )->show();
        #else
            FLAlertLayer::create(
                nullptr,
                "Export Data",
                fmt::format("Data has been exported to: CCGameManager_backup_{}.dat", result.randNum).c_str(),
                "OK",
                nullptr,
                300.f
            )->show();
        #endif
        onSaveData(dummy_first_sender_param_1_7);
    }
}
void HaxMenu::onImportData(CCObject* sender) {
    auto& hax = HaxManager::sharedState();

    auto node = (CCNode*) sender;
    int keyIndex = node->getTag() - 1;

    if (hax.backupKeys[keyIndex] == 0) {
        FLAlertLayer::create(
            nullptr,
            "Load Backup",
            "<cr>Error:</c> this backup does not exist for this version",
            "OK",
            nullptr,
            300.f
        )->show();
        return;
    }

    FILE* fp = fopen(fmt::format("{}{}/CCGameManager_backup_{}.dat", MENU_SETTINGS_PATH, hax.packageName, hax.backupKeys[keyIndex]).c_str(), "rb");
    bool gmExists = !!fp;
#if GAME_VERSION > GV_1_0
    FILE* fp2 = fopen(fmt::format("{}{}/CCLocalLevels_backup_{}.dat", MENU_SETTINGS_PATH, hax.packageName, hax.backupKeys[keyIndex]).c_str(), "rb");
    bool llExists = !!fp2;
#endif
    if (!fp
#if GAME_VERSION > GV_1_0
        && !fp2
#endif
    ) {
        FLAlertLayer::create(
            nullptr,
            "Load Backup",
            "<cr>Error:</c> could not open file for reading",
            "OK",
            nullptr,
            300.f
        )->show();
        return;
    }

    fclose(fp);
#if GAME_VERSION > GV_1_0
    fclose(fp2);
#endif
    
    if (gmExists) {
        const char* gmFilename = fmt::format("{}{}/CCGameManager_backup_{}.dat", MENU_SETTINGS_PATH, hax.packageName, hax.backupKeys[keyIndex]).c_str();

        std::ifstream t(gmFilename);
        t.seekg(0, std::ios::end);
        size_t size = t.tellg();
        std::string buffer(size, ' ');
        t.seekg(0);
        t.read(&buffer[0], size); 

        auto gm = new DS_Dictionary();
        gm->loadRootSubDictFromString(buffer.c_str());

        auto gmCur = new DS_Dictionary();
        void* encodeDataTo = DobbySymbolResolver(MAIN_LIBRARY, "_ZN11GameManager12encodeDataToEP13DS_Dictionary");
        ((void(*)(GameManager*, DS_Dictionary*))encodeDataTo)(GameManager::sharedState(), gmCur);

        gmCur->saveRootSubDictToFile("CCGameManager2.dat");
        gm->saveRootSubDictToFile("CCGameManager.dat");
    }
#if GAME_VERSION > GV_1_0
    if (llExists) {
        const char* llFilename = fmt::format("{}{}/CCLocalLevels_backup_{}.dat", MENU_SETTINGS_PATH, hax.packageName, hax.backupKeys[keyIndex]).c_str();

        std::ifstream t2(llFilename);
        t2.seekg(0, std::ios::end);
        size_t size2 = t2.tellg();
        std::string buffer2(size2, ' ');
        t2.seekg(0);
        t2.read(&buffer2[0], size2); 

        auto ll = new DS_Dictionary();
        ll->loadRootSubDictFromString(buffer2.c_str());

        auto localLevels = new DS_Dictionary();
        void* encodeDataToLL = DobbySymbolResolver(MAIN_LIBRARY, "_ZN17LocalLevelManager12encodeDataToEP13DS_Dictionary");
        ((void(*)(LocalLevelManager*, DS_Dictionary*))encodeDataToLL)(LocalLevelManager::sharedState(), localLevels);

        localLevels->saveRootSubDictToFile("CCLocalLevels2.dat");
        ll->saveRootSubDictToFile("CCLocalLevels.dat");
    }
#endif

    hax.saveSettingsToFile();

    CCDirector::sharedDirector()->replaceScene(LoadingLayer::scene());
}

void HaxMenu::onCategory(ModuleCategory category) {
    auto& hax = HaxManager::sharedState();
    hax.lastCategory = category;

    setColorAtIndex(static_cast<int>(category));
    cleanupMembers();

    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    int y = -20;
    float height = 20;

    int key = 0;
    for (Module& record : hax.modules)
    {
        if (!record.exists) {
            key++;
            continue;
        }
        if (record.category != category) {
            key++;
            continue;
        }

#if GAME_VERSION > GV_1_0
        auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
#else
        auto toggleOn = CCSprite::create("GJ_checkOn.png");
        auto toggleOff = CCSprite::create("GJ_checkOff.png");
#endif

        auto checkbox = CCMenuItemToggler::create(toggleOn, toggleOff, this, menu_selector(HaxMenu::toggler));
        checkbox->toggle(!record.enabled);
        checkbox->setScale(0.5f);
        checkbox->setTag(key);
        this->modMenu->addChild(checkbox, 1003);
        checkbox->setPosition({-100, winSize.height / 2 + y});

        std::string labelValue = record.name;
        labelValue += " "; // italics font gets cut off grrrr
        auto label = CCLabelTTF::create(labelValue.c_str(), "Helvetica-Oblique.ttf", 11);
        this->rightParent->addChild(label, 1003);
        label->setAnchorPoint({0.f, 0.5f});
        label->setPosition(ccp(-68, winSize.height / 2 + y));

        CCSprite* infoSpr = createInfoSprite();
        infoSpr->setScale(0.6f);
        CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(infoSpr, infoSpr, this, menu_selector(HaxMenu::modInfo));
        infoBtn->setSizeMult(1.5f);
        infoBtn->setPosition(ccp(-80 + label->getContentSize().width, winSize.height / 2 + y));
        infoBtn->setTag(key);
        this->modMenu->addChild(infoBtn, 1003);
        y -= 16;
        height += 16;
        key++;
    }
    if (category == ModuleCategory::Player) {
        auto respawnInput = RespawnTimeInput::create(this);
        respawnInput->setupDelegates();
        this->addChild(respawnInput, 1003);
        hax.respawnInput = respawnInput;
        if (!hax.getModuleEnabled(ModuleID::CUSTOM_RESPAWN_TIME)) hax.respawnInput->setVisible(false);
    }
    if (category == ModuleCategory::Universal) {
        auto udidSpr = ButtonSprite::create("Copy UDID", 50, 0, 1, false, "bigFont.fnt", "GJ_button_04.png");
        // delSelSpr->setScale(0.8f);

        auto udidBtn = CCMenuItemSpriteExtra::create(udidSpr, udidSpr, this, menu_selector(HaxMenu::onUDID));
        udidBtn->setPosition(ccp(-70, -winSize.height / 2 + 30));
        this->modMenu->addChild(udidBtn);
        this->udidBtn = udidBtn;
        
#if GDPS == GDPS_NEOPOINTFOUR
        /*auto restoreSpr = ButtonSprite::create("Restore", 50, 0, 1, false, "bigFont.fnt", "GJ_button_04.png");

        auto restoreBtn = CCMenuItemSpriteExtra::create(restoreSpr, restoreSpr, this, menu_selector(HaxMenu::onRestore));
        restoreBtn->setPosition(ccp(10, -winSize.height / 2 + 30));
        this->modMenu->addChild(restoreBtn);
        this->restoreBtn = restoreBtn;*/
#endif
    }
    // CCLog("abcasdfasdadsfdas");
    // this->rightParent->m_height = height;
    // CCLog("abcasdfasdadsfdas");
}
 // if you see this dm "dfdfdcsxxs" to scarfolk.resident on discord

void HaxMenu::onUDID() {
    std::string udid = getPlayerUDID();
    copyStringToClipboard(udid.c_str());

    FLAlertLayer::create(
        nullptr,
        "UDID",
        CCString::createWithFormat(
            "<cy>Your UDID</c>: %s. It should also appear on your <cg>clipboard</c>.\n<co>Warning</c>: Do <cr>NOT</c> share this with anyone! Your <cy>UDID</c> can be used to <cr>access your account</c>.", udid.c_str())->getCString(),
        "OK",
        nullptr,
        300.f
    )->show();
}

void HaxMenu::toggler(CCObject* sender) {
    CCMenuItem* menuItem = (CCMenuItem *)(sender);
    int tag = menuItem->getTag();
    auto& hax = HaxManager::sharedState();
    hax.modules[tag].toggle();
    if (tag == ModuleID::FLOATING_ICON) {
        if (!hax.omniMenu) {
            hax.omniMenu = HaxButton::create();
            hax.omniMenu->retain();
            hax.omniMenu->setup();
            hax.omniMenu->m_sMenu->setVisible(false);
            hax.omniMenu->m_sMenu->setEnabled(false);
        }
    }
    if (tag == ModuleID::PIG_SPOOFING) {
        if (!canSpoofPih) return;
        // float value = static_cast<float>(rand()) / RAND_MAX * 333;
        // CCLog("%f", value);
        // if (value >= 33 && value < 34) {
        //     canSpoofPih = false;
        //     static_cast<CCMenuItemToggler*>(sender)->toggle(false);
        //     this->runAction(CCSequence::create(
        //         CCDelayTime::create(2.f),
        //         CCCallFunc::create(this, callfunc_selector(HaxMenu::onThree)),
        //         nullptr
        //     ));
        // } else {
        GameSoundManager::sharedManager()->playEffect("pih.mp3", 1.0, 0.0, 1.0);
        hax.modules[tag].toggle();
        static_cast<CCMenuItemToggler*>(sender)->toggle(true);
        this->runAction(CCSequence::create(
            CCDelayTime::create(0.2f),
            CCCallFunc::create(this, callfunc_selector(HaxMenu::onPih)),
            nullptr
        ));
        // }
    }
}
void HaxMenu::modInfo(CCObject* sender) {
    CCMenuItem* menuItem = (CCMenuItem *)(sender);
    int tag = menuItem->getTag();
    auto& hax = HaxManager::sharedState();
    Module mod = hax.modules[tag];
    FLAlertLayer::create(
        nullptr,
        mod.name,
        mod.description.c_str(),
        "OK",
        nullptr,
        300.f
    )->show();
}
void HaxMenu::onPih(CCObject* sender) {
    // CCLog("CCObject: %x", sizeof(CCObject));
    // CCLog("CCPoint: %x", sizeof(CCPoint));
    // CCLog("CCNode: %x", sizeof(CCNode));
    // CCLog("CCLayer: %x", sizeof(CCLayer));

    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    CCSprite* pih = CCSprite::create("pih.png");
    addChild(pih, 1010);
    pih->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    pih->setScale(1.5);
    pih->runAction(CCSequence::create(
        CCFadeOut::create(1.5f),
        CCCallFunc::create(pih, callfunc_selector(CCNode::removeFromParentAndCleanup)),
        nullptr
    ));
}
// void HaxMenu::onThree(CCObject* sender) {
//     GameSoundManager::sharedManager()->playEffect("three.mp3", 1.0, 0.0, 1.0);
//     CCDirector* director = CCDirector::sharedDirector();
//     CCSize winSize = director->getWinSize();
//     CCSprite* three = CCSprite::create("three.png");
//     addChild(three, 1011);
//     three->setPosition(ccp(winSize.width / 2, winSize.height / 2));
//     three->runAction(CCSequence::create(
//         CCFadeIn::create(0.2f),
//         CCDelayTime::create(1),
//         CCFadeOut::create(0.25f),
//         CCCallFunc::create(three, callfunc_selector(CCNode::removeFromParentAndCleanup)),
//         nullptr
//     ));
// }

void HaxMenu::keyBackClicked() {
    onClose(nullptr);
}

void HaxMenu::onClose(CCObject* sender) {
    auto& hax = HaxManager::sharedState();
    hax.saveSettingsToFile();
    
    if (hax.getModuleEnabled(ModuleID::FLOATING_ICON)) {
        hax.omniMenu->resetVisibility();
    }

    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    //referrer->setTouchEnabled(true);
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    leftParent->runAction(CCEaseIn::create(
        CCMoveTo::create(getDuration(), ccp(-80.f, winSize.height / 2)), 3
    ));
    rightParent->runAction(CCEaseIn::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width + 80.f, winSize.height / 2)), 3
    ));
    modMenu->runAction(CCEaseIn::create(
        CCMoveTo::create(getDuration(), ccp(winSize.width + 80.f, winSize.height / 2)), 3
    ));
    if (sender) this->retain();
    this->runAction(CCSequence::create(
        CCDelayTime::create(getDuration()),
        CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParentAndCleanup)), // CCRemoveSelf does not exist in old cocos
        nullptr
    ));
    hax.respawnInput = nullptr;
}

bool HaxMenu::ccTouchBegan(cocos2d::CCTouch* t, cocos2d::CCEvent*)
{
    auto pos = getTouchLocation(t);
#if GAME_VERSION >= GV_1_7
    CCRect bb1 = leftPanel->boundingBox();
    CCRect bb2 = rightPanel->boundingBox();

    CCRect bb3 = CCRect(bb1.getMinX() + leftParent->getPositionX(), bb1.getMinY() + leftParent->getPositionY(),
        bb1.getMaxX() + leftParent->getPositionX(), bb1.getMaxY() + leftParent->getPositionY());

    CCRect bb4 = CCRect(bb2.getMinX() + rightParent->getPositionX(), bb2.getMinY() + rightParent->getPositionY(),
        bb2.getMaxX() + rightParent->getPositionX(), bb2.getMaxY() + rightParent->getPositionY());

    if (!(bb3.containsPoint(pos)) && !(bb4.containsPoint(pos))) {
        onClose(nullptr);
    }
#else
    CCRect bb1 = leftPanel->boundingBox();
    CCRect bb2 = rightPanel->boundingBox();
    CCRect bb3 = CCRect(CCRect::CCRectGetMinX(bb1) + leftParent->getPositionX(), CCRect::CCRectGetMinY(bb1) + leftParent->getPositionY(),
        CCRect::CCRectGetMaxX(bb1) + leftParent->getPositionX(), CCRect::CCRectGetMaxY(bb1) + leftParent->getPositionY());

    CCRect bb4 = CCRect(CCRect::CCRectGetMinX(bb2) + rightParent->getPositionX(), CCRect::CCRectGetMinY(bb2) + rightParent->getPositionY(),
        CCRect::CCRectGetMaxX(bb2) + rightParent->getPositionX(), CCRect::CCRectGetMaxY(bb2) + rightParent->getPositionY());

    if (!CCRect::CCRectContainsPoint(bb3, pos) && !CCRect::CCRectContainsPoint(bb4, pos)) {
        onClose(nullptr);
    }
#endif
    return true;
}

// from starry sky. never in a million years would i have figured ts out without it
void HaxMenu::registerWithTouchDispatcher()
{
    CCTouchDispatcher* t = CCDirector::sharedDirector()->getTouchDispatcher();   
    t->addTargetedDelegate(this, 0x80000003, true);
}