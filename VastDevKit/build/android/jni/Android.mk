LOCAL_PATH := $(call my-dir)
MY_LOCAL_PATH := $(LOCAL_PATH)

LIBUUID_PATH := ../../../build/android/jni/e2fsprogs/lib

libext2_uuid_src_files := \
    $(LIBUUID_PATH)/uuid/clear.c \
    $(LIBUUID_PATH)/uuid/compare.c \
    $(LIBUUID_PATH)/uuid/copy.c \
    $(LIBUUID_PATH)/uuid/gen_uuid.c \
    $(LIBUUID_PATH)/uuid/isnull.c \
    $(LIBUUID_PATH)/uuid/pack.c \
    $(LIBUUID_PATH)/uuid/parse.c \
    $(LIBUUID_PATH)/uuid/unpack.c \
    $(LIBUUID_PATH)/uuid/unparse.c \
    $(LIBUUID_PATH)/uuid/uuid_time.c


libext2_uuid_c_includes := $(LIBUUID_PATH)

libext2_uuid_cflags := -O2 -g -W -Wall \
    -DHAVE_INTTYPES_H \
    -DHAVE_UNISTD_H \
    -DHAVE_ERRNO_H \
    -DHAVE_NETINET_IN_H \
    -DHAVE_SYS_IOCTL_H \
    -DHAVE_SYS_MMAN_H \
    -DHAVE_SYS_MOUNT_H \
    -DHAVE_SYS_PRCTL_H \
    -DHAVE_SYS_RESOURCE_H \
    -DHAVE_SYS_SELECT_H \
    -DHAVE_SYS_STAT_H \
    -DHAVE_SYS_TYPES_H \
    -DHAVE_STDLIB_H \
    -DHAVE_STRDUP \
    -DHAVE_MMAP \
    -DHAVE_UTIME_H \
    -DHAVE_GETPAGESIZE \
    -DHAVE_LSEEK64 \
    -DHAVE_LSEEK64_PROTOTYPE \
    -DHAVE_EXT2_IOCTLS \
    -DHAVE_LINUX_FD_H \
    -DHAVE_TYPE_SSIZE_T \
    -DHAVE_SYS_TIME_H \
    -DHAVE_SYS_PARAM_H \
    -DHAVE_SYSCONF

libext2_uuid_system_shared_libraries := libc

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libext2_uuid_src_files)
LOCAL_C_INCLUDES := $(libext2_uuid_c_includes)
LOCAL_CFLAGS := $(libext2_uuid_cflags)
LOCAL_STATIC_LIBRARIES := libc
LOCAL_MODULE := libext2_uuid_static
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)





SDL2_PATH := ../../../build/android/jni/SDL
DIFONT_PATH := ../../../lib/difont

include $(SDL2_PATH)/Android.mk

LOCAL_PATH := $(MY_LOCAL_PATH)
include $(CLEAR_VARS)

include $(DIFONT_PATH)/platforms/android/jni/module.mk

LOCAL_PATH := $(MY_LOCAL_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE := vastdevkit

# Vast library paths
VAST_DEVKIT_HOME := ../../..
VAST_DEVKIT_INCLUDE := $(VAST_DEVKIT_HOME)/include
VAST_DEVKIT_SRC := $(VAST_DEVKIT_HOME)/src
VAST_DEVKIT_LIBS := $(VAST_DEVKIT_HOME)/lib

# 3rd party libraries
JSON_CPP_PATH := $(VAST_DEVKIT_LIBS)/jsoncpp
LIBPNG_PATH := $(VAST_DEVKIT_LIBS)/libpng
BOX2D_PATH := $(VAST_DEVKIT_LIBS)/Box2D/2.1.2/Box2D

# Vast Libraries
VAST_CORE :=        $(VAST_DEVKIT_SRC)/core
VAST_ANIMATION :=   $(VAST_DEVKIT_SRC)/animation
VAST_DATA :=        $(VAST_DEVKIT_SRC)/data
VAST_GIBS :=        $(VAST_DEVKIT_SRC)/gibs
VAST_GRAPHICS :=    $(VAST_DEVKIT_SRC)/graphics
VAST_MATH :=        $(VAST_DEVKIT_SRC)/math
VAST_PHYSICS :=     $(VAST_DEVKIT_SRC)/physics
VAST_TESTS :=       $(VAST_DEVKIT_SRC)/test
VAST_UI :=          $(VAST_DEVKIT_SRC)/ui

# Header directives
VAST_DEVKIT_HEADERS := \
        $(JSON_CPP_PATH) \
        $(LIBPNG_PATH) \
        $(SDL2_PATH) $(SDL2_PATH)/include \
        $(LIBUUID_PATH) \
        $(LIBUUID_PATH)/uuid \
        $(BOX2D_PATH) \
        $(DIFONT_PATH)/src \
        $(DIFONT_PATH)/lib/freetype2/include \
        $(VAST_DEVKIT_INCLUDE) \
        $(VAST_DEVKIT_SRC) \
        $(VAST_CORE) \
        $(VAST_ANIMATION) \
        $(VAST_GRAPHICS) \
        $(VAST_MATH) \
        $(VAST_GIBS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include_all \
            $(VAST_DEVKIT_HEADERS)

LOCAL_CFLAGS := \
        -DGL_GLEXT_PROTOTYPES=1 \
        -DANDROID_NDK \
        -include $(VAST_DEVKIT_SRC)/core/common/Common.h

LOCAL_CPPFLAGS := \
        -DGL_GLEXT_PROTOTYPES=1 \
        -DANDROID_NDK \
        -ftemplate-depth=256 \
        -include $(VAST_DEVKIT_SRC)/core/common/Common.h

ifeq ($(NDK_DEBUG),1)
  LOCAL_CFLAGS += -DDEBUG
endif

JSON_CPP_FILES := $(JSON_CPP_PATH)/jsoncpp.cpp

LIBPNG_SRC_FILES := \
        $(LIBPNG_PATH)/png.c \
        $(LIBPNG_PATH)/pngerror.c \
        $(LIBPNG_PATH)/pngget.c \
        $(LIBPNG_PATH)/pngmem.c \
        $(LIBPNG_PATH)/pngpread.c \
        $(LIBPNG_PATH)/pngread.c \
        $(LIBPNG_PATH)/pngrio.c \
        $(LIBPNG_PATH)/pngrtran.c \
        $(LIBPNG_PATH)/pngrutil.c \
        $(LIBPNG_PATH)/pngset.c \
        $(LIBPNG_PATH)/pngtrans.c \
        $(LIBPNG_PATH)/pngwio.c \
        $(LIBPNG_PATH)/pngwrite.c \
        $(LIBPNG_PATH)/pngwtran.c \
        $(LIBPNG_PATH)/pngwutil.c

BOX2D_DYNAMICS := $(BOX2D_PATH)/Box2D/Dynamics 
BOX2D_SRCS := $(wildcard $(BOX2D_PATH)/Box2D/*.cpp) \
                $(wildcard $(BOX2D_PATH)/Box2D/Collision/*.cpp) \
                $(wildcard $(BOX2D_PATH)/Box2D/Collision/Shapes/*.cpp) \
                $(wildcard $(BOX2D_PATH)/Box2D/Common/*.cpp) \
                $(wildcard $(BOX2D_PATH)/Box2D/Dynamics/*.cpp) \
                $(wildcard $(BOX2D_PATH)/Box2D/Dynamics/Contacts/*.cpp) \
                $(wildcard $(BOX2D_PATH)/Box2D/Dynamics/Joints/*.cpp) \
                $(BOX2D_PATH)/Box2D/Dynamics/Controllers/b2BuoyancyController.cpp \
                $(BOX2D_PATH)/Box2D/Dynamics/Controllers/b2ConstantForceController.cpp \
                $(BOX2D_PATH)/Box2D/Dynamics/Controllers/b2Controller.cpp \
                $(BOX2D_PATH)/Box2D/Dynamics/Controllers/b2GravityController.cpp

VAST_CORE_SRCS := $(wildcard $(VAST_CORE)/*.cpp) \
                $(wildcard $(VAST_CORE)/application/*.cpp) \
                $(wildcard $(VAST_CORE)/collections/*.cpp) \
                $(wildcard $(VAST_CORE)/common/*.cpp) \
                $(wildcard $(VAST_CORE)/component/*.cpp) \
                $(wildcard $(VAST_CORE)/compression/*.cpp) \
                $(wildcard $(VAST_CORE)/device/*.cpp) \
                $(wildcard $(VAST_CORE)/device/android/*.cpp) \
                $(wildcard $(VAST_CORE)/encoding/*.cpp) \
                $(wildcard $(VAST_CORE)/entity/*.cpp) \
                $(wildcard $(VAST_CORE)/filesystem/PakFileSystem.cpp) \
                $(wildcard $(VAST_CORE)/filesystem/android/*.cpp) \
                $(wildcard $(VAST_CORE)/messages/*.cpp) \
                $(wildcard $(VAST_CORE)/notifications/*.cpp) \
                $(wildcard $(VAST_CORE)/system/*.cpp) \
                $(wildcard $(VAST_CORE)/system/android/*.cpp)

VAST_ANIMATION_SRCS := $(wildcard $(VAST_ANIMATION)/*.cpp)

VAST_GIBS_SRCS := $(VAST_GIBS)/Alarm.cpp \
                $(VAST_GIBS)/BaseEvent.cpp \
                $(VAST_GIBS)/Constants.cpp \
                $(VAST_GIBS)/DeviceDetector.Android.cpp \
                $(VAST_GIBS)/EventQueue.cpp \
                $(VAST_GIBS)/EventRenderer.cpp \
                $(wildcard $(VAST_GIBS)/*/*.cpp)

VAST_GRAPHICS_SRCS := $(wildcard $(VAST_GRAPHICS)/components/*.cpp) \
                $(wildcard $(VAST_GRAPHICS)/gl/*.cpp) \
                $(wildcard $(VAST_GRAPHICS)/mesh/*.cpp) \
                $(wildcard $(VAST_GRAPHICS)/shader/*.cpp) \
                $(VAST_GRAPHICS)/texture/PNGLoader.cpp \
                $(VAST_GRAPHICS)/texture/TextureManager.gl3.cpp \
                $(VAST_GRAPHICS)/texture/TGALoader.cpp \
                $(VAST_GRAPHICS)/util/hdNoise.cpp

VAST_MATH_SRCS := $(VAST_MATH)/Matrix.cpp

VAST_PHYSICS_SRCS := $(wildcard $(VAST_PHYSICS)/*/components/*.cpp)

VAST_TEST_SRCS := $(wildcard $(VAST_TESTS)/*.cpp)

VAST_UI_SRCS := $(wildcard $(VAST_UI)/*.cpp) \
                $(wildcard $(VAST_UI)/fonts/*.cpp)

LOCAL_SRC_FILES := \
        $(LIBPNG_SRC_FILES) \
        $(JSON_CPP_FILES) \
        $(BOX2D_SRCS) \
        $(VAST_CORE_SRCS) \
        $(VAST_ANIMATION_SRCS) \
        $(VAST_GIBS_SRCS) \
        $(VAST_GRAPHICS_SRCS) \
        $(VAST_PHYSICS_SRCS) \
        $(VAST_MATH_SRCS) \
        $(VAST_TEST_SRCS) \
        $(VAST_UI_SRCS)

LOCAL_STATIC_LIBRARIES += difont libSDL2 libext2_uuid_static

LOCAL_LDLIBS := -lGLESv3 -llog -ldl -lc -lm -lz

#LOCAL_LDFLAGS := -Wl,-Map=${LOCAL_PATH}/../obj/local/${TARGET_ARCH_ABI}/libvastdevkit-${TARGET_ARCH_ABI}.map 

include $(BUILD_SHARED_LIBRARY)
