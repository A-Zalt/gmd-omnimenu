#define GAME_VERSION 3
#define READABLE_GAME_VERSION "1.2"
#define READABLE_GAME_VERSION_FULL "1.22"
#define MENU_VERSION "0.42"

#if GAME_VERSION > 9
    #define MAIN_LIBRARY "libcocos2dcpp.so"
#else
    #define MAIN_LIBRARY "libgame.so"
#endif