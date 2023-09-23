#!/bin/sh

SDL_COMMIT=tags/release-2.28.3
SDL_IMAGE_COMMIT=tags/release-2.6.3
SDL_TTF_COMMIT=tags/release-2.20.2
ANDROID_PROJECT=com.igrmm.isgt
APP_NAME=ISGT

mkdir -p build

if [ ! -d "build/android" ]; then
    cd build
    mkdir -p android && cd android
    git clone https://github.com/libsdl-org/SDL/
    cd SDL && git checkout $SDL_COMMIT && cd build-scripts
    ./androidbuild.sh $ANDROID_PROJECT /dev/null
    cd ../ && mv build/$ANDROID_PROJECT ../ && cd ../
    cd $ANDROID_PROJECT/app/jni/
    git clone https://github.com/libsdl-org/SDL_image.git
    cd SDL_image && git checkout $SDL_IMAGE_COMMIT && cd ../
    git clone https://github.com/libsdl-org/SDL_ttf.git
    cd SDL_ttf && git checkout $SDL_TTF_COMMIT && git submodule update --init
    cd ../

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

    cd ../
    sed -i "s/Game/$APP_NAME/" src/main/res/values/strings.xml

    sed -i "/<activity android:name/a \            android:screenOrientation=\"landscape\"" src/main/AndroidManifest.xml

    cd ../../../../
    ln -s $(pwd)/assets build/android/$ANDROID_PROJECT/app/src/main/
fi
cd build/android/$ANDROID_PROJECT/
./gradlew installDebug
cd ../../../
