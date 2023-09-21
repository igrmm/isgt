#!/bin/sh

[ -z "$ANDROID_HOME" ] && echo "ANDROID_HOME not set" && exit
[ -z "$ANDROID_NDK_HOME" ] && echo "ANDROID_NDK_HOME not set" && exit

# last stable
if [ ! -d "SDL" ]; then
    git clone https://github.com/libsdl-org/SDL/
    cd SDL && git checkout tags/release-2.28.3 && cd ../
fi

# bug fix: https://github.com/libsdl-org/SDL/issues/7267#issuecomment-1424880747
if [ ! -d "SDL_image" ]; then
    git clone https://github.com/libsdl-org/SDL_image.git
    cd SDL_image && git checkout 44354bc && cd ../
fi

cd ../../
./gradlew assemble
