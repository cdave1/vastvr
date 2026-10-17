#ifndef _VAST_COMMON_H_
#define _VAST_COMMON_H_

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <math.h>
#include <float.h>

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <Rpc.h>
typedef UUID hdUuid;
#include "core/common/compat/compat.h"
#else
#include <unistd.h>
#include <pthread.h>
#include <uuid/uuid.h>
typedef uuid_t * hdUuid;
#endif

#ifdef __APPLE__
#include <uuid/uuid.h>
#elif defined(ANDROID)
#include <jni.h>
#include <android/log.h>
#include <uuid.h>
#endif

#if (TARGET_OS_IPHONE == 1) && (TARGET_IPHONE_SIMULATOR == 0)
#ifdef __arm__
#include <mach-o/arch.h>
#endif
#endif

#if defined __cplusplus
#include "core/common/Log.h"
#endif

// Tyepdefs
typedef double hdTimestamp;
typedef double hdTimeInterval;

typedef short i16;
typedef unsigned short u16;

typedef unsigned char uint8_t;

// Assert
#ifdef DEBUG
#define hdAssert(A) assert(A)
#else
#define hdAssert(A) (void(0))
#endif

// Output
#ifdef DEBUG
    #ifdef ANDROID
        #define vinfo(...) ((void)__android_log_print(ANDROID_LOG_INFO, __FUNCTION__, __VA_ARGS__))
        #define vwarn(...) ((void)__android_log_print(ANDROID_LOG_WARN, __FUNCTION__, __VA_ARGS__))
    #else
        #define vinfo(...)  printf(__VA_ARGS__)
        #define vwarn(...)  fprintf(stderr, __VA_ARGS__);
    #endif
#else
    #define vinfo(...) ((void)0)
    #define vwarn(...) ((void)0)
#endif

#ifdef ANDROID
    #define verr(...) ((void)__android_log_print(ANDROID_LOG_ERROR, __FUNCTION__, __VA_ARGS__))
#else
    #define verr(...)   fprintf(stderr, __VA_ARGS__);
#endif

// Generic callback 
#define HDCALLBACK

#define hdFreeSafely(ptr) { if (ptr) { free(ptr); ptr = NULL; } }
#define hdDeleteSafely(ptr) { if (ptr) { delete ptr; ptr = NULL; } }

#define FreeSafely(ptr) hdFreeSafely(ptr)
#define DeleteSafely(ptr) hdDeleteSafely(ptr)

#endif
