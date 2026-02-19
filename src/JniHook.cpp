#include "HaxManager.hpp"
#include "Utils.hpp"
#include <jni.h>

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