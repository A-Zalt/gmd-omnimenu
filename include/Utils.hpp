#pragma once
#include <cocos2d.h>
#include <jni.h>

CCSprite* createInfoSprite();
ccColor3B hsvToRgb(float h, float s, float v);
JNIEnv* getEnv();
void writeGMD(GJGameLevel* level, const char* uriStr);
GJGameLevel* readGMD(const char* uriStr);
void copyStringToClipboardOld(const char* string);
void copyStringToClipboard(const char* string);
void seekBackgroundMusicTo(int ms);

template <typename T>
std::string ToString(T val);

int stoi(const std::string& str);
void getPackageName();