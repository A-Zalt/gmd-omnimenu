#pragma once
#ifndef __HOOKING_ESSENTIALS
#define __HOOKING_ESSENTIALS

#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook
#include <fmt/format.h> // fmtlib
#include "HaxManager.hpp"
#include <cocos2d.h>
#include "../version/VersionUtils.hpp"

#define CC_CONTENT_SCALE_FACTOR_ROBTOP() CCDirector::sharedDirector()->getContentScaleFactorHD()
#define CC_RECT_PIXELS_TO_POINTS_ROBTOP(__rect_in_pixels__)                                                                        \
    CCRectMake( (__rect_in_pixels__).origin.x / CC_CONTENT_SCALE_FACTOR_ROBTOP(), (__rect_in_pixels__).origin.y / CC_CONTENT_SCALE_FACTOR_ROBTOP(),    \
            (__rect_in_pixels__).size.width / CC_CONTENT_SCALE_FACTOR_ROBTOP(), (__rect_in_pixels__).size.height / CC_CONTENT_SCALE_FACTOR_ROBTOP() )
#define CC_POINT_PIXELS_TO_POINTS_ROBTOP(__pixels__)                                                                        \
    CCPointMake( (__pixels__).x / CC_CONTENT_SCALE_FACTOR_ROBTOP(), (__pixels__).y / CC_CONTENT_SCALE_FACTOR_ROBTOP())
#define CC_SIZE_PIXELS_TO_POINTS_ROBTOP(__size_in_pixels__)                                                                        \
    CCSizeMake( (__size_in_pixels__).width / CC_CONTENT_SCALE_FACTOR_ROBTOP(), (__size_in_pixels__).height / CC_CONTENT_SCALE_FACTOR_ROBTOP())
#define CC_SIZE_POINTS_TO_PIXELS_ROBTOP(__size_in_points__)                                                                        \
    CCSizeMake( (__size_in_points__).width * CC_CONTENT_SCALE_FACTOR_ROBTOP(), (__size_in_points__).height * CC_CONTENT_SCALE_FACTOR_ROBTOP())


namespace Omni {
    void* getHandle() {
        return dlopen(MAIN_LIBRARY, RTLD_NOW);
    }

    void hook(const char* symbol, void* func, void** origFunc) {
        DobbyHook(dlsym(getHandle(), symbol), func, origFunc);
    }
}
#endif