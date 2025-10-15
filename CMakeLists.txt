cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ANDROID_ABI armeabi)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

project(menu)

add_library(menu SHARED src/main.cc src/layers/HaxLayer.cpp)

set(DOBBY_DEBUG OFF CACHE INTERNAL "" FORCE)
set(DOBBY_GENERATE_SHARED OFF CACHE INTERNAL "" FORCE)
add_definitions(-DLOGGING_DISABLE -DNDEBUG)
add_subdirectory(vendor/Dobby)

target_compile_options(menu PUBLIC -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.)

target_link_libraries(menu dobby)
target_link_libraries(menu ${CMAKE_CURRENT_SOURCE_DIR}/libgame.so log android)

target_include_directories(menu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cocos2d/cocos2dx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cocos2d/cocos2dx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cocos2d/cocos2dx/platform/android
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cocos2d/cocos2dx/kazmath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/layers
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/layers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hooks
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version)