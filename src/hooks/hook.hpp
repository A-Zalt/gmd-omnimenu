#pragma once

#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook
#include "HaxManager.hpp"
#include <cocos2d.h>
#include "../version/VersionUtils.cc"

namespace Omni {
    void* getHandle() {
        return dlopen(MAIN_LIBRARY, RTLD_NOW);
    }

    void hook(const char* symbol, void* func, void** origFunc) {
        DobbyHook(dlsym(getHandle(), symbol), func, origFunc);
    }
}