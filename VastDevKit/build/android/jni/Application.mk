APP_BUILD_SCRIPT := Android.mk
$(info $(APP_BUILD_SCRIPT))
APP_MODULES      := vastdevkit gnustl_shared

BUILD_STYLE ?= DEV
ifeq ($(BUILD_STYLE),DEV)
  NDK_DEBUG := 1
  APP_OPTIM := debug
endif

ifeq ($(BUILD_STYLE),QA)
  APP_OPTIM := release
endif

ifeq ($(BUILD_STYLE),RELEASE)
  APP_OPTIM := release
endif

NDK_TOOLCHAIN_VERSION := 4.8
APP_ABI := armeabi armeabi-v7a
APP_PLATFORM := android-21
APP_STL := gnustl_shared
APP_CPPFLAGS += -std=c++11
LOCAL_C_INCLUDES += ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/include
TARGET_CPU_ABI := armeabi-v7a 
TARGET_CPU_ABI2 := armeabi 