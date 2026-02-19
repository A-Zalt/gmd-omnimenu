#include "../hook.hpp"

class InfoButtonHandler : public CCObject {
public:
    GJGameLevel* m_level;
    
    static InfoButtonHandler* create(GJGameLevel* lvl) {
        auto ret = new InfoButtonHandler();
        if (ret) {
            ret->m_level = lvl;
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    std::string timeToString(time_t input) {
        struct tm timeInfo;
        localtime_r(&input, &timeInfo);
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &timeInfo);
        return std::string(buffer);
    }
    
    void onInfoClick(CCObject* sender) {
        if (!m_level) {
            return;
        }
        
        HaxManager& hax = HaxManager::sharedState();
        
        int uploadTimestamp = hax.uploadTimestampMap[m_level];
        int updateTimestamp = hax.updateTimestampMap[m_level];
        int rateTimestamp = hax.rateTimestampMap[m_level];
        int originalID = hax.originalIDMap[m_level];
        
        std::string uploadDate = (uploadTimestamp > 0) ? timeToString(static_cast<time_t>(uploadTimestamp)) : "NA";
        std::string updateDate = (updateTimestamp > 0) ? timeToString(static_cast<time_t>(updateTimestamp)) : "NA";
        std::string rateDate = (rateTimestamp > 0) ? timeToString(static_cast<time_t>(rateTimestamp)) : "NA";
        
        std::stringstream ss;
        ss << "Uploaded: <cy>" << uploadDate << "</c>\nUpdated: <cy>" << updateDate << "</c>";
        if (rateTimestamp > 0) {
            ss << "\nRated: <cy>" << rateDate << "</c>";
        }           
        if (originalID > 0) {
            ss << "\nOriginal: <cy>" << originalID << "</c>";
        }
        
        std::string infoText = ss.str();
        
        FLAlertLayer* alert = FLAlertLayer::create(nullptr, "Level Info", infoText.c_str(), "OK", nullptr, 300.f);
        alert->show();
    }
};

bool (*TRAM_InfoLayer_init)(CCLayer* self, GJGameLevel* lvl);
bool InfoLayer_init(CCLayer* self, GJGameLevel* lvl) {
    if (!TRAM_InfoLayer_init(self, lvl)) {
        return false;
    }
    auto mainLayer = getMainLayer(self);
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    CCMenu* targetMenu = nullptr;
    CCObject* child;
    CCARRAY_FOREACH(mainLayer->getChildren(), child) {
        auto menu = dynamic_cast<CCMenu*>(child);
        if (menu) {
            targetMenu = menu;
            break;
        }
    }
    
    CCSprite* infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    auto handler = InfoButtonHandler::create(lvl);
    handler->retain();
    
    CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr, infoSpr, handler, 
        menu_selector(InfoButtonHandler::onInfoClick)
    );
    infoBtn->setSizeMult(1.2f);
    
    infoBtn->setUserObject(handler);
    infoBtn->setPosition(winSize.width - 200.f, (winSize.height / 2) - 167.f);
    
    targetMenu->addChild(infoBtn);
    
    return true;
}

void NP4_InfoLayer_om() {
    Omni::hook("_ZN9InfoLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(InfoLayer_init),
        reinterpret_cast<void**>(&TRAM_InfoLayer_init));
}