#include "hooks.cpp"
#include "JniHook.cpp"

[[gnu::constructor]]
int main() {
#if GDPS == GDPS_NEOPOINTFOUR && GAME_VERSION != GV_1_4
    #error "Building for Neopointfour while GAME_VERSION is not 1.4"
#endif
#if GDPS == GDPS_1_7 && GAME_VERSION != GV_1_7
    #error "Building for 1.7 GDPS while GAME_VERSION is not 1.7"
#endif
    initialize_hooks();
}