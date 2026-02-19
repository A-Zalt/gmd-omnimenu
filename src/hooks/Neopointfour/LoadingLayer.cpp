#include "../hook.hpp"

const char* (*TRAM_LoadingLayer_getLoadingString)(CCLayer* self);
const char* LoadingLayer_getLoadingString(CCLayer* self) {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
            
    if (timeinfo->tm_mon == 8 && timeinfo->tm_mday == 17) { // months are 0 indexed fsr
        return "9/17 Never Forget";
    }
            
    const char* result;
    int number = arc4random() % 22;
    switch (number) {
        case 1:
            result = "Listen to the music to help time your jumps";
            break;
        case 2:
            result = "Back for more are ya?";
            break;
        case 3:
            result = "Use practice mode to learn the layout of a level";
            break;
        case 4:
            result = "Build your own levels using the level editor";
            break;
        case 5:
            result = "Go online to play other players' levels!";
            break;
        case 6:
            result = "If at first you don't succeed, try, try again...";
            break;
        case 7:
            result = "Can you beat them all?";
            break;
        case 8:
            result = "Customize your character's icon and color!";
            break;
        case 9:
            result = "You can download all songs from the level select page!";
            break;
        case 10:
            result = "Spikes are not your friends, don't forget to jump";
            break;
        case 11:
            result = "Go big or go home";
            break;
        case 12:
            result = "smung iphone";
            break;
        case 13:
            result = "My mom's kinda boneless";
            break;
        case 14:
            result = "I'm literally older than yo Brainrot Ahhh.";
            break;
        case 15:
            result = "evnos";
            break;
        case 16:
            result = "Nobody earned the prize";
            break;
        case 17:
            result = "wow hexagon force is jard !!!!!";
            break;
        case 18:
            result = "what should I add\ndeco";
            break;
        case 19:
            result = "pidge";
            break;
        case 20:
            result = "strabery";
            break;
        case 21:
            result = "ENDURANCE FUCKING SUCKS";
            break;
        default:
            result = "Unlock new icons and colors by completing achievements!";
            break;
    }
    return result;
}

void NP4_LoadingLayer_om() {
    Omni::hook("_ZN12LoadingLayer16getLoadingStringEv",
        reinterpret_cast<void*>(LoadingLayer_getLoadingString),
        reinterpret_cast<void**>(&TRAM_LoadingLayer_getLoadingString));
}