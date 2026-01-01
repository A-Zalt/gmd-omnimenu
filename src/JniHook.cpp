#include "HaxManager.hpp"
#include "Utils.hpp"
#include <jni.h>
#ifdef USE_MINIAUDIO
#include "AudioManager.hpp"
#endif

extern "C" JNIEXPORT void JNICALL
JNI_FN(JAVA_PATH_MAIN_JNI_HOOK, GeometryJump, nativeOnFileSaveChosen)(
        JNIEnv* env, jobject /*thiz*/, jstring uri) {
    const char* uriStr = env->GetStringUTFChars(uri, nullptr);
    auto& hax = HaxManager::sharedState();
    writeGMD(hax.gdShareLevel, uriStr);
    env->ReleaseStringUTFChars(uri, uriStr);
}
extern "C" JNIEXPORT void JNICALL
JNI_FN(JAVA_PATH_MAIN_JNI_HOOK, GeometryJump, nativeOnFileOpenChosen)(
        JNIEnv* env, jobject /*thiz*/, jstring uri) {
    const char* uriStr = env->GetStringUTFChars(uri, nullptr);
    auto& hax = HaxManager::sharedState();
#if GAME_VERSION < GV_1_4
    hax.myLevelsLayer->loadLevel(readGMD(uriStr));
#else
    hax.levelBrowserLayer->loadLevel(readGMD(uriStr));
#endif
    env->ReleaseStringUTFChars(uri, uriStr);
}

#ifdef USE_MINIAUDIO
extern "C" JNIEXPORT void JNICALL
JNI_FN(JAVA_PATH_MAIN_JNI_HOOK, GeometryJump, nativeSetAssetManager)(
        JNIEnv* env, jclass clazz, jobject assetManager) {
    AudioManager::sharedManager().init(env, assetManager);
}
#endif