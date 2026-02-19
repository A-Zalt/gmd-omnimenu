#ifndef NP4_GJCOMMENT_CPP
#define NP4_GJCOMMENT_CPP

#include "../hook.hpp"

static std::map<void*, int> commentDates;

CCNode* (*TRAM_GJComment_create)(CCDictionary* dict);
CCNode* GJComment_create(CCDictionary* dict) {
    auto comment = TRAM_GJComment_create(dict);
    if (!comment) {
        return comment;
    }
    auto dateValue = dict->valueForKey("9")->intValue();
    if (dateValue != 0) {
        commentDates[comment] = dateValue;
    } else {
    }
    return comment;
}

void (*TRAM_GJComment_destructor)(CCNode* self);
void GJComment_destructor(CCNode* self) {
    auto it = commentDates.find(self);
    if (it != commentDates.end()) {
        commentDates.erase(it);
    } else {
    }
    
    TRAM_GJComment_destructor(self);
}

int getCommentDate(void* comment) {
    auto it = commentDates.find(comment);
    if (it != commentDates.end()) {
        return it->second;
    }
    return 0;
}

void NP4_GJComment_om() {
    Omni::hook("_ZN9GJComment6createEPN7cocos2d12CCDictionaryE",
        reinterpret_cast<void*>(GJComment_create),
        reinterpret_cast<void**>(&TRAM_GJComment_create));
    Omni::hook("_ZN9GJCommentD1Ev",
        reinterpret_cast<void*>(GJComment_destructor),
        reinterpret_cast<void**>(&TRAM_GJComment_destructor));
}

#endif