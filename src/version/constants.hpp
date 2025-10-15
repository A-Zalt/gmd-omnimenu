#define GAME_VERSION 3
#define MENU_VERSION "0.50"

#if GAME_VERSION == 1
    #define GAME_VER_PATH 1.0
#elif GAME_VERSION == 2
    #define GAME_VER_PATH 1.1
#elif GAME_VERSION == 3
    #define GAME_VER_PATH 1.2
#elif GAME_VERSION == 4
    #define GAME_VER_PATH 1.3
#elif GAME_VERSION == 5
    #define GAME_VER_PATH 1.4
#elif GAME_VERSION == 6
    #define GAME_VER_PATH 1.5
#elif GAME_VERSION == 7
    #define GAME_VER_PATH 1.6
#elif GAME_VERSION == 10
    #define GAME_VER_PATH 1.7
#elif GAME_VERSION == 18
    #define GAME_VER_PATH 1.8
#else
    #error "Unsupported GAME_VERSION"
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define PATH(file) GAME_VER_PATH/file

#define INCREASED_OBJECT_LIMIT 16384

#include STR(PATH(constants.hpp))

#if GAME_VERSION > 9
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