#include "versions.hpp"




#define MENU_VERSION "v1.0.4"
#define MENU_SETTINGS "settings" READABLE_GAME_VERSION ".json"
#define MENU_SETTINGS_PATH "/storage/emulated/0/OMNImenu/"

#define FORCE_AUTO_SAFE_MODE // Comment this out to disable force auto safe mode
#define PING_SPOOFING // Comment this out to disable Pig Spoofing (brutal)
// #define STAR_RATED_LEVELS_GRANT_COINS // Uncomment this to enable star rated levels granting secret coins even in Force Auto Safe Mode (1.6+)
#define GDPS GDPS_NONE // Possible values: GDPS_NONE, GDPS_NEOPOINTFOUR and GDPS_1_7 (1.7 GDPS) 

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

// #if GAME_VERSION == 1
//     #define READABLE_GAME_VERSION "1.0"
//     #define READABLE_GAME_VERSION_FULL "1.02"
//     #define OBJECT_LIMIT 3999
// #elif GAME_VERSION == 2
//     #define READABLE_GAME_VERSION "1.1"
//     #define READABLE_GAME_VERSION_FULL "1.11"
//     #define OBJECT_LIMIT 3999
// #elif GAME_VERSION == 4
//     #define READABLE_GAME_VERSION "1.3"
//     #define READABLE_GAME_VERSION_FULL "1.3"
//     #define OBJECT_LIMIT 5999
// #elif GAME_VERSION == 5
//     #define READABLE_GAME_VERSION "1.4"
//     #define READABLE_GAME_VERSION_FULL "1.41"
//     #define OBJECT_LIMIT 5999
// #elif GAME_VERSION == 6
//     #define READABLE_GAME_VERSION "1.5"
//     #define READABLE_GAME_VERSION_FULL "1.51"
//     #define OBJECT_LIMIT 5999
// #elif GAME_VERSION == 7
//     #define READABLE_GAME_VERSION "1.6"
//     #define READABLE_GAME_VERSION_FULL "1.6"
//     #define OBJECT_LIMIT 7999
// #elif GAME_VERSION == 10
//     #define READABLE_GAME_VERSION "1.7"
//     #define READABLE_GAME_VERSION_FULL "1.71"
//     #define OBJECT_LIMIT 9999
// #elif GAME_VERSION == 18
//     #define READABLE_GAME_VERSION "1.8"
//     #define READABLE_GAME_VERSION_FULL "1.811"
//     #define OBJECT_LIMIT 11999
// #endif

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
    #define sender_param_1_7 , sender
    #define dummy_sender_param_1_7 , nullptr
    #define _Cocos2dxMusic "org/cocos2dx/lib/Cocos2dxMusic"
#else
    #define SEL_MenuHandler_1_7_compat
    #define SEL_MenuHandler_1_7_compat2
    #define sender_param_1_7
    #define dummy_sender_param_1_7
    #define _Cocos2dxMusic "org/cocos2dx/lib/p"
#endif