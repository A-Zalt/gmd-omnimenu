#pragma once
#include "GJGameLevel.hpp"

class LevelTools {
public:
    static GJGameLevel* getLevel(int id);
    static const char* nameForArtist(int id);
    static const char* ngURLForArtist(int id);
    static const char* getAudioTitle(int id);
    static const char* getAudioFileName(int id);
};
