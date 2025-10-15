#include "hook.hpp"

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!! SHOUTOUT TO ADELFA/AKQANILE FOR IMPLEMENTATION !!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/ 
bool (*TRAM_CCString_initWithFormatAndValist)(cocos2d::CCString* self, const char* format, va_list ap);
bool CCString_initWithFormatAndValist(cocos2d::CCString* self, const char* format, va_list ap) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.upload100KbFix) {
         size_t buf_size = static_cast<size_t>(vsnprintf(nullptr, 0, format, ap)) + 1;
        char* buf = static_cast<char*>(malloc(buf_size));

        vsprintf(buf, format, ap);

        typedef std::string* (*gd_string_assign_t)(void*, const char*, uint);
        gd_string_assign_t gd_string_assign = reinterpret_cast<gd_string_assign_t>(get_address(basicstring_assign));
        gd_string_assign((void*)&self->m_sString, buf, buf_size - 1);
        
        free(buf);

        return true;
    } else {
        return TRAM_CCString_initWithFormatAndValist(self, format, ap);
    }
}

void CCString_om() {
    Omni::hook("_ZN7cocos2d8CCString23initWithFormatAndValistEPKcSt9__va_list",
        reinterpret_cast<void*>(CCString_initWithFormatAndValist),
        reinterpret_cast<void**>(&TRAM_CCString_initWithFormatAndValist));
}