#!/bin/sh

mkdir -p build
cd build

if [ ! -d "android" ]; then
    mkdir -p android && cd android
    git clone https://github.com/libsdl-org/SDL/
    cd SDL && git checkout tags/release-2.28.3
    mv android-project ../ && cd ../
    cd android-project/app/jni/
    mv ../../../SDL ./
    git clone https://github.com/libsdl-org/SDL_image.git
    cd SDL_image && git checkout tags/release-2.6.3 && cd ../
    git clone https://github.com/libsdl-org/SDL_ttf.git
    cd SDL_ttf && git checkout tags/release-2.20.2 && cd external
    git clone https://github.com/libsdl-org/freetype.git
    cd freetype && git checkout fd08bbe && cd ../
    git clone https://github.com/libsdl-org/harfbuzz.git
    cd harfbuzz && git checkout c248ad5 && cd ../../../

    echo \
        'LOCAL_PATH := $(call my-dir)

    include $(CLEAR_VARS)

    LOCAL_MODULE := main

    SDL_PATH := ../SDL

    LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/../SDL_image/ $(LOCAL_PATH)/../SDL_ttf/

    LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../../../../../src/*.c)

    LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf

    LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid

    include $(BUILD_SHARED_LIBRARY)'\
        > src/Android.mk

    cd ../../../../../
    ln -s $(pwd)/assets build/android/android-project/app/src/main/
fi

cd build/android/android-project/
./gradlew installDebug
cd ../../../
