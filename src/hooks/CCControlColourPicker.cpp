#include "hook.hpp"
#include "RGBColorInputs.hpp"
#include "ColorSelectPopup.hpp"
#include <string>

/*

        WARNING: THIS CODE IS SO UTTERLY ATROCIOUS THAT YOU MAY FIND YOURSELF CONTEMPLATING SUICIDE
        VIEWER DISCRETION IS ADVISED.
                            ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
                            ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡶⠿⠿⢶⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
                            ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡿⠃⠀⠀⠀⠀⠙⢷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
                            ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡿⢡⠀⠀⠀⠀⠀⢀⡈⢿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀
                            ⠀⠀⠀⠀⠀⠀⠀⠀⣰⡟⢠⣿⠀⠀⠀⠀⠀⢸⣷⡈⢻⣆⠀⠀⠀⠀⠀⠀⠀⠀
                            ⠀⠀⠀⠀⠀⠀⢀⣼⠏⢠⣿⣿⡆⠀⠀⠀⠀⣸⣿⣷⡄⠹⣆⠀⠀⠀⠀⠀⠀⠀
                            ⠀⠀⠀⠀⠀⢀⣾⠃⣰⣿⣿⣿⡇⠀⠀⠀⠀⣿⣿⣿⣿⡄⠹⣷⡀⠀⠀⠀⠀⠀
                            ⠀⠀⠀⠀⢠⡿⠁⣰⣿⣿⣿⣿⣿⠀⠀⠀⢠⣿⣿⣿⣿⣿⣆⠘⣷⡀⠀⠀⠀⠀
                            ⠀⠀⠀⢠⡿⠁⣼⣿⣿⣿⣿⣿⣿⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣆⠘⢿⡄⠀⠀⠀
                            ⠀⠀⣠⡟⢀⣼⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣧⠈⢿⡄⠀⠀
                            ⠀⢰⡿⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⡟⠛⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠈⣿⡄⠀
                            ⠀⢼⡇⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠆⢸⡇⠀
                            ⠀⠘⣷⣄⠙⠛⠻⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠟⠛⠋⣠⡿⠃⠀
                            ⠀⠀⠈⠉⠛⠓⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠛⠛⠉⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        BY FAR THE WORST CODE I'VE EVER WRITTEN FOR OMNIMENU SO FAR. DEAR GOD THIS IS AWFUL.

*/



RGBColorInputs* RGBColorInputs::create(extension::CCControlColourPicker* parent) {
    auto ret = new RGBColorInputs;
    if (ret->init(parent)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void RGBColorInputs::createTheThings() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    this->redInput = CCTextInputNode::create(100.0, 40.0, "Red", "Thonburi", 12, "bigFont.fnt");
    redInput->setPosition(ccp(100, winSize.height - 30));
    redInput->setMaxLabelScale(0.7);
    redInput->setLabelPlaceholderScale(0.6);
    setCharLimit(redInput, 3);
    redInput->setTag(RED_INPUT_TAG);
    redInput->setAllowedChars("0123456789");
    redInput->setAnchorPoint({0, 0.5});

    this->greenInput = CCTextInputNode::create(100.0, 40.0, "Green", "Thonburi", 12, "bigFont.fnt");
    greenInput->setPosition(ccp(100, winSize.height - 60));
    greenInput->setMaxLabelScale(0.7);
    greenInput->setLabelPlaceholderScale(0.6);
    setCharLimit(greenInput, 3);
    greenInput->setTag(GREEN_INPUT_TAG);
    greenInput->setAllowedChars("0123456789");
    greenInput->setAnchorPoint({0, 0.5});

    this->blueInput = CCTextInputNode::create(100.0, 40.0, "Blue", "Thonburi", 12, "bigFont.fnt");
    blueInput->setPosition(ccp(100, winSize.height - 90));
    blueInput->setMaxLabelScale(0.7);
    blueInput->setLabelPlaceholderScale(0.6);
    setCharLimit(blueInput, 3);
    blueInput->setTag(BLUE_INPUT_TAG);
    blueInput->setAllowedChars("0123456789");
    blueInput->setAnchorPoint({0, 0.5});

    addChild(redInput);
    addChild(greenInput);
    addChild(blueInput);

    auto bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(80, 30));
    bg->_setZOrder(-1);
    bg->setPosition({100, winSize.height - 30});
    addChild(bg);
    bg->setScale(0.9);

    bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(80, 30));
    bg->_setZOrder(-1);
    bg->setPosition({100, winSize.height - 60});
    addChild(bg);
    bg->setScale(0.9);

    bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(80, 30));
    bg->_setZOrder(-1);
    bg->setPosition({100, winSize.height - 90});
    addChild(bg); 
    bg->setScale(0.9);
    
    auto label = CCLabelBMFont::create("R: ", "bigFont.fnt");
    label->setPosition(ccp(55, winSize.height - 30));
    label->setScale(0.6f);
    addChild(label); 
    
    label = CCLabelBMFont::create("G: ", "bigFont.fnt");
    label->setPosition(ccp(55, winSize.height - 60));
    label->setScale(0.6f);
    addChild(label);
    
    label = CCLabelBMFont::create("B: ", "bigFont.fnt");
    label->setPosition(ccp(55, winSize.height - 90));
    label->setScale(0.6f);
    addChild(label);
}

bool RGBColorInputs::init(extension::CCControlColourPicker* parent) {
    if (!CCLayer::init()) return false;
    this->parent = parent;
    createTheThings();
}

void RGBColorInputs::setupDelegates() {
    if (this->redInput) {
        setTextInputDelegate(this->redInput, this);
    }
    if (this->greenInput) {
        setTextInputDelegate(this->greenInput, this);
    }
    if (this->blueInput) {
        setTextInputDelegate(this->blueInput, this);
    }
}

std::string* intFilter(const char* input) {
    std::string* out = new std::string;

    for (const char* p = input; *p != '\0'; ++p) {
        if (*p >= '0' && *p <= '9') {
            out->push_back(*p);
        }
    }

    return out;
}

void RGBColorInputs::textChanged(CCTextInputNode* node) {
    if (ignore) return;
    if (!node || node == nullptr) return;

    // this is my replacement for getString because that shit dont work
    auto booba = *(CCTextFieldTTF**)((char*)node + CCTextInputNode__m_textField);
    if (!booba) return;
    if (!booba->m_pInputText) return;
    const char* bro = booba->m_pInputText->c_str();
    if (!bro || bro == nullptr) {
        CCLog("bro is a nullptr :skull:");
        return;
    }
    std::string* str = intFilter(bro);
    int num = stoi(*str);
    _ccColor3B color = this->parent->getColorValue();

    if (num < 0) num = 0;
    if (num > 255) num = 255;
    if (node == this->redInput) color.r = num;
    else if (node == this->greenInput) color.g = num;
    else if (node == this->blueInput) color.b = num;

    this->parent->setColorValue(color);
}

bool (*TRAM_CCControlColourPicker_init)(extension::CCControlColourPicker* self);
bool CCControlColourPicker_init(extension::CCControlColourPicker* self) {
    if (!TRAM_CCControlColourPicker_init(self)) return false;
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::RGB_COLOR_INPUTS)) {
        auto widget = RGBColorInputs::create(self);
        widget->setupDelegates();
        widget->setTag(RGBCOLORINPUTS_TAG);
        self->addChild(widget);
        // _ccColor3B color = self->getColorValue();
        // widget->ignore = true;
        // widget->redInput->setString(ToString(static_cast<unsigned int>(color.r)).c_str());
        // widget->greenInput->setString(ToString(static_cast<unsigned int>(color.g)).c_str());
        // widget->blueInput->setString(ToString(static_cast<unsigned int>(color.b)).c_str());
        // widget->ignore = false;
    }
    return true;
}

void (*TRAM_CCControlColourPicker_updateHueAndControlPicker)(extension::CCControlColourPicker* self);
void CCControlColourPicker_updateHueAndControlPicker(extension::CCControlColourPicker* self) {
    TRAM_CCControlColourPicker_updateHueAndControlPicker(self);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::RGB_COLOR_INPUTS)) {
        _ccColor3B color = self->getColorValue();
        if (!self->getParent()) return;
        auto child = self->getParent()->getChildByTag(RGBCOLORINPUTS_TAG);
        if (!child) return;
        auto widget = static_cast<RGBColorInputs*>(child);
        if (!widget->getParent()) self->getParent()->addChild(widget);
        widget->ignore = true;
        widget->redInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.r))).c_str());
        widget->greenInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.g))).c_str());
        widget->blueInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.b))).c_str());
        widget->ignore = false;
    }
}

void (*TRAM_CCControlColourPicker_colourSliderValueChanged)(extension::CCControlColourPicker* self, CCObject* sender, unsigned int controlEvent);
void CCControlColourPicker_colourSliderValueChanged(extension::CCControlColourPicker* self, CCObject* sender, unsigned int controlEvent) {
    TRAM_CCControlColourPicker_colourSliderValueChanged(self, sender, controlEvent);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::RGB_COLOR_INPUTS)) {
        _ccColor3B color = self->getColorValue();
        if (!self->getParent()) return;
        auto child = self->getParent()->getChildByTag(RGBCOLORINPUTS_TAG);
        if (!child) return;
        auto widget = static_cast<RGBColorInputs*>(child);
        if (!widget->getParent()) self->getParent()->addChild(widget);
        widget->ignore = true;
        widget->redInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.r))).c_str());
        widget->greenInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.g))).c_str());
        widget->blueInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.b))).c_str());
        widget->ignore = false;
    }
}
void (*TRAM_CCControlColourPicker_hueSliderValueChanged)(extension::CCControlColourPicker* self, CCObject* sender, unsigned int controlEvent);
void CCControlColourPicker_hueSliderValueChanged(extension::CCControlColourPicker* self, CCObject* sender, unsigned int controlEvent) {
    TRAM_CCControlColourPicker_hueSliderValueChanged(self, sender, controlEvent);
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::RGB_COLOR_INPUTS)) {
        _ccColor3B color = self->getColorValue();
        if (!self->getParent()) return;
        auto child = self->getParent()->getChildByTag(RGBCOLORINPUTS_TAG);
        if (!child) return;
        auto widget = static_cast<RGBColorInputs*>(child);
        if (!widget->getParent()) self->getParent()->addChild(widget);
        widget->ignore = true;
        widget->redInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.r))).c_str());
        widget->greenInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.g))).c_str());
        widget->blueInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.b))).c_str());
        widget->ignore = false;
    }
}

bool (*TRAM_ColorSelectPopup_init)(ColorSelectPopup* self, GameObject* obj);
bool ColorSelectPopup_init(ColorSelectPopup* self, GameObject* obj) {
    if (!TRAM_ColorSelectPopup_init(self, obj)) return false;
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::RGB_COLOR_INPUTS)) {
        auto colorPicker = getColorPicker(self);
        auto widget = static_cast<RGBColorInputs*>(colorPicker->getChildByTag(RGBCOLORINPUTS_TAG));
        widget->removeFromParentAndCleanup(false);
        self->addChild(widget);
        _ccColor3B color = self->getColorValue();

        widget->ignore = true;
        widget->redInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.r))).c_str());
        widget->greenInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.g))).c_str());
        widget->blueInput->setString(fmt::format("{}", (static_cast<unsigned int>(color.b))).c_str());
        widget->ignore = false;
    }
    return true;
};

#if GAME_VERSION < GV_1_7
bool (*TRAM_ColorPickerPopup_init)(ColorPickerPopup* self);
bool ColorPickerPopup_init(ColorPickerPopup* self) {
    if (!TRAM_ColorPickerPopup_init(self)) return false;
#else
bool (*TRAM_ColorPickerPopup_init)(ColorPickerPopup* self, int r, int g, int b);
bool ColorPickerPopup_init(ColorPickerPopup* self, int r, int g, int b) {
    if (!TRAM_ColorPickerPopup_init(self, r, g, b)) return false;
#endif
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::RGB_COLOR_INPUTS)) {
        auto colorPicker = getColorPicker(self);
        auto widget = colorPicker->getChildByTag(RGBCOLORINPUTS_TAG);
        widget->removeFromParentAndCleanup(false);
        self->addChild(widget);
    }
    return true;
};


void CCControlColourPicker_om() {
    Omni::hook("_ZN7cocos2d9extension21CCControlColourPicker4initEv",
        reinterpret_cast<void*>(CCControlColourPicker_init),
        reinterpret_cast<void**>(&TRAM_CCControlColourPicker_init));
    Omni::hook("_ZN7cocos2d9extension21CCControlColourPicker25updateHueAndControlPickerEv",
        reinterpret_cast<void*>(CCControlColourPicker_updateHueAndControlPicker),
        reinterpret_cast<void**>(&TRAM_CCControlColourPicker_updateHueAndControlPicker));
    Omni::hook("_ZN16ColorSelectPopup4initEP10GameObject",
        reinterpret_cast<void*>(ColorSelectPopup_init),
        reinterpret_cast<void**>(&TRAM_ColorSelectPopup_init));
    Omni::hook(
#if GAME_VERSION < GV_1_7
        "_ZN16ColorPickerPopup4initEv",
#else
        "_ZN16ColorPickerPopup4initEiii",
#endif
        reinterpret_cast<void*>(ColorPickerPopup_init),
        reinterpret_cast<void**>(&TRAM_ColorPickerPopup_init));
    Omni::hook("_ZN7cocos2d9extension21CCControlColourPicker24colourSliderValueChangedEPNS_8CCObjectEj",
        reinterpret_cast<void*>(CCControlColourPicker_colourSliderValueChanged),
        reinterpret_cast<void**>(&TRAM_CCControlColourPicker_colourSliderValueChanged));
    Omni::hook("_ZN7cocos2d9extension21CCControlColourPicker21hueSliderValueChangedEPNS_8CCObjectEj",
        reinterpret_cast<void*>(CCControlColourPicker_hueSliderValueChanged),
        reinterpret_cast<void**>(&TRAM_CCControlColourPicker_hueSliderValueChanged));
}