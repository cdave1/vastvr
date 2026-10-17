#ifndef _VAST_FILESYSTEM_H_
#define _VAST_FILESYSTEM_H_

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string>

#ifdef WIN32
// TODO
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

#include "File.h"

#define _VAST_FS_ABSOLUTE_PATH 0x0008

namespace hd {
    class FileSystem {
    public:
        
        static void Init(const std::string &baseDir);
        
        static int CreateFile(const std::string &filename);
        
        static bool FileExists(const std::string &filename);
        
        static int WriteToFile(const std::string &filename, const void *bytes, const unsigned long length);
        
        static hd::File * OpenFile(const std::string &filename);
        
        static void CloseFile(hd::File *fhandle);
        
        static unsigned int ReadFile (void *buffer, unsigned int size, unsigned int count, hd::File *fhandle);
        
        static unsigned int FileTell (hd::File *fhandle);
        
        static unsigned int FileSeek (hd::File *fhandle, unsigned int offset, unsigned int origin);
        
        static unsigned int GetFileSize (hd::File *fhandle);
        
        static void InitFileSystem(const char *p);
        
        static const std::string & BaseDir();

        static std::string FullPathTo(const std::string &filename);
        
        //
        // Returns 0 on success, -1 on fail.
        //
        static int ReadFromFile(void **dest, size_t *len, const char *fileName);
        
        static void Teardown();
        
    };
}


#endif
