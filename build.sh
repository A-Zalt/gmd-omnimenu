#!/usr/bin/env bash

if [ ! -v NDK_ROOT ]; then
    echo "NDK_ROOT is not set. The value of NDK_ROOT should be a path to a directory that contains files from the NDK."
    echo "Hint: NDK_ROOT=~/NDK/r16b ./build.sh"
    exit
fi

OBJCOPY_PATH="/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy"
STRIP_PATH="/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip"

if [ ! -f build/.setup-flag ]; then
    cmake \
         -B build \
         -DCMAKE_TOOLCHAIN_FILE=$NDK_ROOT\/build/cmake/android.toolchain.cmake \
         -DANDROID_TOOLCHAIN=gcc

    (($? == 0)) && touch build/.setup-flag
fi

if [ -f build/.setup-flag ]; then
    cmake --build build --config Release

    (($? != 0)) && exit

    eval $NDK_ROOT\$STRIP_PATH -s build/libmenu.so
fi
