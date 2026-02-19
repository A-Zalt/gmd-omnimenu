#include "cocos2d.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "DS_Dictionary.hpp"
#include "GJGameLevel.hpp"
#include "HaxManager.hpp"

CCSprite* createInfoSprite() {
    return CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
}
ccColor3B hsvToRgb(float h, float s, float v) {
    int i = int(h / 60) % 6;
    float f = h / 60 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    float r, g, b;
    switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
    }
    return ccc3(r * 255, g * 255, b * 255);
}

JNIEnv* getEnv() {
    JNIEnv* env;

    if (cocos2d::JniHelper::getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        cocos2d::CCLog("Failed to get the environment using GetEnv()");
        return nullptr;
    }
    if (static_cast<int>(cocos2d::JniHelper::getJavaVM()->AttachCurrentThread(&env, 0)) < 0) {
        cocos2d::CCLog("Failed to get the environment using AttachCurrentThread()");
        return nullptr;
    }

    return env;
}

void writeGMD(GJGameLevel* level, const char* uriStr) {
    JNIEnv* env = getEnv();
    if (!env) return;
    
    jstring jUriStr = env->NewStringUTF(uriStr);

    jclass uriClass = env->FindClass("android/net/Uri");
    jmethodID parseMethod = env->GetStaticMethodID(uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
    jobject uri = env->CallStaticObjectMethod(uriClass, parseMethod, jUriStr);

    jclass activityClass = env->FindClass(JAVA_PATH_MAIN "/GeometryJump");
    jmethodID getActivity = env->GetStaticMethodID(activityClass, "getInstance", "()L" JAVA_PATH_MAIN "/GeometryJump;");
    jobject activity = env->CallStaticObjectMethod(activityClass, getActivity);
    jmethodID getCR = env->GetMethodID(activityClass, "getContentResolver", "()Landroid/content/ContentResolver;");
    jobject resolver = env->CallObjectMethod(activity, getCR);

    jclass resolverClass = env->FindClass("android/content/ContentResolver");
    jmethodID openOut = env->GetMethodID(resolverClass, "openOutputStream", "(Landroid/net/Uri;)Ljava/io/OutputStream;");
    jobject outputStream = env->CallObjectMethod(resolver, openOut, uri);

    env->DeleteLocalRef(activityClass);
    env->DeleteLocalRef(resolverClass);
    if (!outputStream) {
        env->DeleteLocalRef(uri);
        env->DeleteLocalRef(jUriStr);
        return;
    }

    auto dict = new DS_Dictionary();
    void* encodeWithCoder = DobbySymbolResolver(MAIN_LIBRARY, "_ZN11GJGameLevel15encodeWithCoderEP13DS_Dictionary");
    ((void(*)(GJGameLevel*, DS_Dictionary*))encodeWithCoder)(level, dict);

    std::string strObj = dict->saveRootSubDictToString();
    const char* str = strObj.c_str();

    std::vector<uint8_t> data(str, str + strlen(str));

    jclass outputStreamClass = env->FindClass("java/io/OutputStream");
    jmethodID writeMethod = env->GetMethodID(outputStreamClass, "write", "([B)V");
    jmethodID closeMethod = env->GetMethodID(outputStreamClass, "close", "()V");

    jbyteArray jData = env->NewByteArray(data.size());
    env->SetByteArrayRegion(jData, 0, data.size(), reinterpret_cast<const jbyte*>(data.data()));
    env->CallVoidMethod(outputStream, writeMethod, jData);
    env->CallVoidMethod(outputStream, closeMethod);
    
    env->DeleteLocalRef(jData);
    env->DeleteLocalRef(outputStream);
    env->DeleteLocalRef(outputStreamClass);
    env->DeleteLocalRef(uri);
    env->DeleteLocalRef(jUriStr);
}
GJGameLevel* readGMD(const char* uriStr) {
    JNIEnv* env = getEnv();
    if (!env) return nullptr;
    GJGameLevel* level;
    std::vector<uint8_t> result;

    jstring jUriStr = env->NewStringUTF(uriStr);
    jclass uriClass = env->FindClass("android/net/Uri");
    jmethodID parseMethod = env->GetStaticMethodID(uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
    jobject uri = env->CallStaticObjectMethod(uriClass, parseMethod, jUriStr);

    jclass activityClass = env->FindClass(JAVA_PATH_MAIN "/GeometryJump");
    jmethodID getActivity = env->GetStaticMethodID(activityClass, "getInstance", "()L" JAVA_PATH_MAIN "/GeometryJump;");
    jobject activity = env->CallStaticObjectMethod(activityClass, getActivity);
    jmethodID getCR = env->GetMethodID(activityClass, "getContentResolver", "()Landroid/content/ContentResolver;");
    jobject resolver = env->CallObjectMethod(activity, getCR);

    jclass resolverClass = env->FindClass("android/content/ContentResolver");
    jmethodID openIn = env->GetMethodID(resolverClass, "openInputStream", "(Landroid/net/Uri;)Ljava/io/InputStream;");
    jobject inputStream = env->CallObjectMethod(resolver, openIn, uri);

    if (!inputStream) {
        env->DeleteLocalRef(uri);
        env->DeleteLocalRef(jUriStr);
        return nullptr;
    }

    jclass inputStreamClass = env->FindClass("java/io/InputStream");
    jmethodID readMethod = env->GetMethodID(inputStreamClass, "read", "([B)I");
    jmethodID closeMethod = env->GetMethodID(inputStreamClass, "close", "()V");

    const int bufSize = 8192;
    jbyteArray buffer = env->NewByteArray(bufSize);

    while (true) {
        jint bytesRead = env->CallIntMethod(inputStream, readMethod, buffer);
        if (bytesRead <= 0) break;
        jbyte* bytes = env->GetByteArrayElements(buffer, nullptr);
        result.insert(result.end(), bytes, bytes + bytesRead);
        env->ReleaseByteArrayElements(buffer, bytes, JNI_ABORT);
    }

    env->CallVoidMethod(inputStream, closeMethod);
    env->DeleteLocalRef(buffer);
    env->DeleteLocalRef(inputStream);
    env->DeleteLocalRef(uri);
    env->DeleteLocalRef(jUriStr);

    const uint8_t* rawDataPtr = result.data();
    const char* str = reinterpret_cast<const char*>(rawDataPtr);

    auto dict = new DS_Dictionary();
    if (!dict->loadRootSubDictFromString(str)) return nullptr;

    level = GJGameLevel::createWithCoder(dict);

    return level;
}
jobject getGlobalContext(JNIEnv *env)
{   
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject activityThreadObj = env->CallStaticObjectMethod(activityThread, currentActivityThread);
    
    jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    jobject context = env->CallObjectMethod(activityThreadObj, getApplication);
    return context;
}
void copyStringToClipboardOld(const char* string) {
    JNIEnv* env = getEnv();
    jclass context_cls = env->FindClass("android/content/Context");
    jfieldID clipboardService_fid = env->GetStaticFieldID(context_cls, "CLIPBOARD_SERVICE", "Ljava/lang/String;");
    jmethodID getSystemService_mid = env->GetMethodID(context_cls, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject clipboardService_obj = env->CallObjectMethod(
        getGlobalContext(env), 
        getSystemService_mid, 
        (jstring)env->GetStaticObjectField(context_cls, clipboardService_fid)
    );
    
    jstring strToCopy = env->NewStringUTF(string);
    jclass clipManager_cls = env->FindClass("android/text/ClipboardManager");
    jmethodID setText_mid = env->GetMethodID(clipManager_cls, "setText", "(Ljava/lang/CharSequence;)V");
    env->CallVoidMethod(clipboardService_obj, setText_mid, strToCopy);
}
void copyStringToClipboard(const char* string) {
    JNIEnv* env = getEnv();
    jclass helperCls = env->FindClass(CLIPBOARD_HELPER_CLASS);
    if (!helperCls) {
        CCLog("warning: no helper class found. using copyStringToClipboardOld");
        return copyStringToClipboardOld(string);
    }
    jmethodID copyMid = env->GetStaticMethodID(helperCls, "copyToClipboard",
                                            "(Ljava/lang/String;)V");
    if (!copyMid) {
        CCLog("warning: no copy method found. using copyStringToClipboardOld");
        return copyStringToClipboardOld(string);
    }
    env->CallStaticVoidMethod(helperCls, copyMid, env->NewStringUTF(string));
}

void seekBackgroundMusicTo(int ms) {
    JNIEnv* env = getEnv();

    if (!env) {
        cocos2d::CCLog("Failed to get JNI environment");
        return;
    }
#if GAME_VERSION < GV_1_7
    jclass Cocos2dxActivity = env->FindClass("org/cocos2dx/lib/Cocos2dxActivity");
    if (Cocos2dxActivity == nullptr) {
        cocos2d::CCLog("Failed to find Cocos2dxActivity class");
        return;
    }
#else
    jclass Cocos2dxHelper = env->FindClass("org/cocos2dx/lib/Cocos2dxHelper");
    if (Cocos2dxHelper == nullptr) {
        cocos2d::CCLog("Failed to find Cocos2dxHelper class");
        return;
    }
#endif
    // some cocos2d java class names have been obfuscated, but not all
#if GAME_VERSION < GV_1_7
    jfieldID fieldID_backgroundMusicPlayer = env->GetStaticFieldID(Cocos2dxActivity, "backgroundMusicPlayer", "L" _Cocos2dxMusic ";");
#else
    jfieldID fieldID_backgroundMusicPlayer = env->GetStaticFieldID(Cocos2dxHelper, "sCocos2dMusic", "L" _Cocos2dxMusic ";");
#endif
    if (fieldID_backgroundMusicPlayer == nullptr) {
        cocos2d::CCLog("Failed to get field ID of backgroundMusicPlayer");
        return;
    }
#if GAME_VERSION < GV_1_7
    jobject backgroundMusicPlayer = env->GetStaticObjectField(Cocos2dxActivity, fieldID_backgroundMusicPlayer);
#else
    jobject backgroundMusicPlayer = env->GetStaticObjectField(Cocos2dxHelper, fieldID_backgroundMusicPlayer);
#endif
    if (backgroundMusicPlayer == nullptr) {
        cocos2d::CCLog("Failed to get backgroundMusicPlayer");
        return;
    }

    jclass Cocos2dxMusic = env->FindClass(_Cocos2dxMusic);
    if (Cocos2dxMusic == nullptr) {
        cocos2d::CCLog("Failed to get Cocos2dxMusic");
        return;
    }
    jfieldID fieldID_mBackgroundMediaPlayer = env->GetFieldID(Cocos2dxMusic, "mBackgroundMediaPlayer", "Landroid/media/MediaPlayer;");
    if (fieldID_mBackgroundMediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get field ID of mBackgroundMediaPlayer");
        return;
    }
    jobject mBackgroundMediaPlayer = env->GetObjectField(backgroundMusicPlayer, fieldID_mBackgroundMediaPlayer);
    if (mBackgroundMediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get mBackgroundMediaPlayer");
        return;
    }

    jclass MediaPlayer = env->GetObjectClass(mBackgroundMediaPlayer);
    if (MediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get MediaPlayer");
        return;
    }
    
    bool useNew = true;
    jclass versionClass;
    jfieldID sdkIntFieldID;
    jint sdkJint;
    int sdkInt;
    jmethodID seekTo;
    do {
        versionClass = env->FindClass("android/os/Build$VERSION");
        if (versionClass == nullptr) {
            cocos2d::CCLog("Warning: Failed to get class Build$VERSION. Using old \"broken\" method.");
            useNew = false;
            break;
        }
        sdkIntFieldID = env->GetStaticFieldID(versionClass, "SDK_INT", "I");
        if (sdkIntFieldID == nullptr) {
            cocos2d::CCLog("Warning: Failed to get field ID of SDK_INT. Using old \"broken\" method.");
            useNew = false;
            break;
        }

        sdkJint = env->GetStaticIntField(versionClass, sdkIntFieldID);
        sdkInt = static_cast<int>(sdkJint);
        if (sdkInt < 26) {
            cocos2d::CCLog("sdkInt: %i < 26. Using old \"broken\" method.", sdkInt);
            useNew = false;
            break;
        }
        seekTo = env->GetMethodID(MediaPlayer, "seekTo", "(JI)V");
        if (seekTo == nullptr) {
            cocos2d::CCLog("Warning: Failed to get method ID of seekTo(long, int). Using old \"broken\" method.");
            useNew = false;
            break;
        }
    } while (0);

    if (useNew) {
        env->CallVoidMethod(mBackgroundMediaPlayer, seekTo, static_cast<jlong>(static_cast<long>(ms)), static_cast<jint>(2));
    } else {
        jmethodID oldSeekTo = env->GetMethodID(MediaPlayer, "seekTo", "(I)V");
        if (oldSeekTo == nullptr) {
            cocos2d::CCLog("Failed to get method ID of seekTo(int)");
            return;
        }
        env->CallVoidMethod(mBackgroundMediaPlayer, oldSeekTo, static_cast<jint>(ms));
    } 
}

template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

int stoi(const std::string& s) {
    int result = 0;
    int multiplier = 1;
    if (s.empty()) return 0;
    for (size_t i = s.length(); i-- > 0;) {
        result += multiplier * (s[i] - '0');
        multiplier *= 10;
    }
    return result;
}

void getPackageName() {
    CCLog("a");
    auto& hax = HaxManager::sharedState();
    if (strcmp("", hax.packageName)) {
        return;
    }
#if GDPS == GDPS_NEOPOINTFOUR
    hax.packageName = "com.cynigdx.onepointfour";
#elif GDPS == GDPS_1_7
    hax.packageName = "com.ariccox.aricco17gdps";
#else
    hax.packageName = "com.robtopx.geometryjump";
#endif
    CCLog("%s", hax.packageName);
    JNIEnv* env = getEnv();
    if (!env) {
        CCLog("failed to get env; falling back on default package name");
        return;
    }

    jclass activityClass = env->FindClass(JAVA_PATH_MAIN "/GeometryJump");
    jmethodID getActivity = env->GetStaticMethodID(activityClass, "getInstance", "()L" JAVA_PATH_MAIN "/GeometryJump;");
    jobject activity = env->CallStaticObjectMethod(activityClass, getActivity);

    jmethodID mid = env->GetMethodID(activityClass, "getPackageName", "()Ljava/lang/String;");
    jstring pkg = (jstring) env->CallObjectMethod(activity, mid);

    hax.packageName = env->GetStringUTFChars(pkg, nullptr);
    CCLog("%s", hax.packageName);
}