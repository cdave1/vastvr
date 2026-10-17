#ifndef _VAST_PNG_LOADER_H
#define _VAST_PNG_LOADER_H

#include "core/filesystem/FileSystem.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	extern void LoadPNG( const char *filename, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes );

    ///
    /// Save pixels in rgba format.
    ///
    extern bool WritePNG(const char *filename, uint8_t *pixels, uint32_t width, uint32_t height);
	
#ifdef __cplusplus
}
#endif

#endif