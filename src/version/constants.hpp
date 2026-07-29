#include "versions.hpp"




#define MENU_VERSION "v1.3.0"
#define MENU_SETTINGS_OLD "settings" READABLE_GAME_VERSION ".json"
#define MENU_SETTINGS_PATH "/storage/emulated/0/OMNImenu/"

// #define STEALTH_MODE // gd colon mode

#define FORCE_AUTO_SAFE_MODE // Comment this out to disable force auto safe mode
#define PING_SPOOFING // Comment this out to disable Pig Spoofing (brutal)
// #define STAR_RATED_LEVELS_GRANT_COINS // Uncomment this to enable star rated levels granting secret coins even in Force Auto Safe Mode (1.6+)
#define GDPS GDPS_NONE // Possible values: GDPS_NONE, GDPS_NEOPOINTFOUR and GDPS_1_7 (1.7 GDPS by Aricco) 
// #define EXTRA_COLORS // Uncomment this to enable extra colors (1.3 and 1.4 only)
#define NUMBER_OF_BACKUPS 5 // Change this to modify the amount of available backups per game version

#if GDPS == GDPS_NONE
    // You have to replace both of these if necessary
    #define JAVA_PATH_MAIN "com/robtopx/geometryjump"
    #define JAVA_PATH_MAIN_JNI_HOOK com_robtopx_geometryjump
#elif GDPS == GDPS_1_7
    #define JAVA_PATH_MAIN "com/ariccox/aricco17gdps"
    #define JAVA_PATH_MAIN_JNI_HOOK com_ariccox_aricco17gdps
#elif GDPS == GDPS_NEOPOINTFOUR
    #define JAVA_PATH_MAIN "com/cynigdx/onepointfour"
    #define JAVA_PATH_MAIN_JNI_HOOK com_cynigdx_onepointfour
#else
    #error "Unknown GDPS"
#endif


#define FEATURED_TEXTURE "thelazycat/GJ_featuredCoin_001.png"
#define MAGIC_TEXTURE "thelazycat/GJ_epicCoin_001.png"
#define COPY_MARK_TEXTURE "randomb/collaborationIcon_001.png"
#define MORE_FILTERS_TEXTURE "randomb/GJ_plusBtn_001.png"
#define RANDOM_BUTTON_TEXTURE "GJ_randomBtn_001.png"
#define CLIPBOARD_HELPER_CLASS JAVA_PATH_MAIN "/ClipboardHelper"
#define JNI_FN_EXPAND(pkg, cls, fn) Java_##pkg##_##cls##_##fn
#define JNI_FN(pkg, cls, fn) JNI_FN_EXPAND(pkg, cls, fn)

#if GAME_VERSION == GV_1_0
    #define GAME_VER_PATH 1.0
#elif GAME_VERSION == GV_1_1
    #define GAME_VER_PATH 1.1
#elif GAME_VERSION == GV_1_2
    #define GAME_VER_PATH 1.2
#elif GAME_VERSION == GV_1_3
    #define GAME_VER_PATH 1.3
#elif GAME_VERSION == GV_1_4
    #define GAME_VER_PATH 1.4
#elif GAME_VERSION == GV_1_5
    #define GAME_VER_PATH 1.5
#elif GAME_VERSION == GV_1_6
    #define GAME_VER_PATH 1.6
#elif GAME_VERSION == GV_1_7
    #define GAME_VER_PATH 1.7
#elif GAME_VERSION == GV_1_8
    #define GAME_VER_PATH 1.8
#else
    #error "Unsupported GAME_VERSION"
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define PATH(file) GAME_VER_PATH/file

#define INCREASED_OBJECT_LIMIT 16384

#include STR(PATH(constants.hpp))

#if GAME_VERSION >= GV_1_7
    #define MAIN_LIBRARY "libcocos2dcpp.so"
#else
    #define MAIN_LIBRARY "libgame.so"
#endif

#define GDSHARE_FL(errorMsg) \
    FLAlertLayer::create(\
        nullptr,\
        "GDShare",\
        errorMsg,\
        "OK",\
        nullptr,\
        300.f\
    )->show();


#if GAME_VERSION >= GV_1_7
    #define SEL_MenuHandler_1_7_compat CCObject* sender
    #define SEL_MenuHandler_1_7_compat2 \
     , CCObject* sender
    #define dummy_first_sender_param_1_7 nullptr
    #define sender_param_1_7 , sender
    #define dummy_sender_param_1_7 , nullptr
    #define hook_1_7_compat CCObject* sender
    #define _Cocos2dxMusic "org/cocos2dx/lib/Cocos2dxMusic"
#else
    #define SEL_MenuHandler_1_7_compat
    #define SEL_MenuHandler_1_7_compat2
    #define dummy_first_sender_param_1_7
    #define sender_param_1_7
    #define dummy_sender_param_1_7
    #define hook_1_7_compat CCNode* sender
    #define _Cocos2dxMusic "org/cocos2dx/lib/p"
#endif