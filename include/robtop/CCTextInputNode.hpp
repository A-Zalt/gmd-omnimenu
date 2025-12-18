#pragma once

#include <cocos2d.h>

class CCTextInputNode : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate, public cocos2d::CCTextFieldDelegate 
{
public:
    static CCTextInputNode* create(float width, float height, char const* placeholder, char const* unk, int fontSize, char const* font);
    
    bool getProfanityFilter();
    void setProfanityFilter(bool profanityFilter);
    void setAllowedChars(std::string charFilter);
    // void setCharLimit(int limit);

    const char* getString();
    void setString(const char* string);

    void setMaxLabelScale(float scale);
    void setLabelPlaceholderScale(float scale);
    void setLabelPlaceholderColor(_ccColor3B);
};

class TextInputDelegate {
    virtual void textChanged(CCTextInputNode*) {}
    virtual void textInputOpened(CCTextInputNode*) {}
    virtual void textInputClosed(CCTextInputNode*) {}
};