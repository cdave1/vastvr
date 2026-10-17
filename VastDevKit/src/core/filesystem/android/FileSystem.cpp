#include "core/filesystem/FileSystem.h"
#include "core/filesystem/PakFileSystem.h"

static char gamedir[512];

void hd::FileSystem::Init(const char* baseDir) {
        snprintf(gamedir, 512, "%s", baseDir);
#ifndef LEVEL_EDITOR
        PakFileSystem::InitPakFileSystem(gamedir);
#endif
}


const char * hd::FileSystem::BaseDir() {
    assert(strlen(gamedir) > 0);
    return gamedir;
}


unsigned int hd::FileSystem::GetFileSize(hd::File *fhandle) {
    return fhandle->filesize;
}


unsigned int hd::FileSystem::FileSeek(hd::File *fhandle, unsigned int offset, unsigned int origin) {
    switch(origin) {
        case SEEK_SET:
            if (offset > fhandle->filesize) {
                return 1;
            }
            
            fhandle->ptrCurrent = fhandle->ptrStart + offset;
            break;
            
        case SEEK_END:
            if (offset > 0) {
                return 1;
            }
            
            // offset is negative 
            fhandle->ptrCurrent = fhandle->ptrEnd + offset;
            break;
            
        case SEEK_CUR:
            if (offset > 0) {
                if((int)offset > (fhandle->ptrEnd - fhandle->ptrCurrent)) {
                    return 1;
                }
            }
            
            fhandle->ptrCurrent += offset;
            break;
            
        default:
            return 1;
    }
    
    return 0;
}


void hd::FileSystem::CloseFile(hd::File *fhandle) {
    if (fhandle->filedata) {
#ifdef USE_MMAP
        if (fhandle->mmapped) {
            if (munmap(fhandle->filedata, fhandle->filesize ) != 0) {
                assert(false);
            }
        } else {
            free( fhandle->filedata );
        }
#else
        free(fhandle->filedata);
#endif
        fhandle->filedata = NULL;
    }
    
    free(fhandle);
}


hd::File *hd::FileSystem::OpenFile(const char *filename) {
    char netpath[256];
    hd::File *hFile;
    const char *pathBase;
    struct stat s;
    int fd;

#ifndef LEVEL_EDITOR
    hd::log::i("Searching pak files for %s\n", filename);
    if (NULL != (hFile = PakFileSystem::OpenFile(filename))) {
        return hFile;
    }
#endif

    if (filename[0] == '/') {
        // Just use full filename
        snprintf(netpath, sizeof(netpath), "%s", filename);
    } else {
        pathBase = hd::FileSystem::BaseDir();
        if (pathBase[strlen(pathBase) - 1] == '/') {
            snprintf(netpath, sizeof(netpath), "%s%s", pathBase, filename );
        } else {
            snprintf(netpath, sizeof(netpath), "%s/%s", pathBase, filename );
        }
    }
    
    // high performance file mapping path, avoiding stdio
    fd = open( netpath, O_RDONLY );
    if (fd == -1) {
        return NULL;
    }
    fstat(fd, &s);
    
    hFile = (hd::File*)malloc( sizeof( hd::File ) );
    memset( hFile, 0, sizeof( hd::File ) );
    
    hFile->filesize = s.st_size;
#ifdef USE_MMAP    
    hFile->filedata = mmap(NULL, hFile->filesize, PROT_READ, MAP_FILE|MAP_PRIVATE, fd, 0);
    if ((int)hFile->filedata == -1) {
        hd::log::e("mmap failed: %s\n", strerror( errno));
        assert( 0 );
    }
    hFile->mmapped = true;
#else
    hFile->filedata = malloc( hFile->filesize );
    read( fd, hFile->filedata, hFile->filesize );
    hFile->mmapped = false;
#endif
    hFile->ptrStart =  hFile->ptrCurrent = (unsigned char *)hFile->filedata;
    hFile->ptrEnd =  (unsigned char *)hFile->filedata + hFile->filesize;
    hFile->bLoaded = true;
    // mmap doesn't require the file to stay open
    close( fd );
    return hFile;
}


unsigned int hd::FileSystem::ReadFile(void *buffer, unsigned int size, unsigned int count, hd::File *fhandle) {
    unsigned char *buf = (unsigned char *)buffer;
    signed int i;
    
    if ((int)(size * count) > (fhandle->ptrEnd - fhandle->ptrCurrent)) {
        signed int read;
        
        read = (fhandle->ptrEnd - fhandle->ptrCurrent);
        
        for (i = 0; i < (fhandle->ptrEnd - fhandle->ptrCurrent); ++i) {
            buf[ i ] = fhandle->ptrCurrent[ i ];
        }
        
        fhandle->ptrCurrent = fhandle->ptrEnd;
        
        return(read);
    } else {
        for (i = 0; i < (int)(size * count); ++i, fhandle->ptrCurrent++) {
            buf[ i ] = *fhandle->ptrCurrent;
        }
        
        return (size > 0) ? ((size * count) / size) : 0;
    }
    
    /* should never get here */
    return -1;
}


void hd::FileSystem::Teardown() {
    //assert(singleton != nil);
    //[singleton release];
    //singleton = nil;
}


int hd::FileSystem::CreateFile(const char *fileName) {
    return -1;
    /*
    assert(singleton != nil);
    std::string fullPathTo = hd::FileSystem::FullPathTo(fileName);
    NSString *fileLocation = [NSString stringWithCString:fullPathTo.c_str() encoding:NSASCIIStringEncoding];
    return [singleton CreateFile:fileLocation];*/
}


bool hd::FileSystem::FileExists(const char* fileName) {
    //assert(singleton != nil);
    //return [singleton FileExists:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]];
    return false;
}


std::string hd::FileSystem::FullPathTo(const std::string &filename) {
    char netpath[256];
    const char *pathBase = hd::FileSystem::BaseDir();
    if (pathBase[strlen(pathBase) - 1] == '/') {
        snprintf(netpath, sizeof(netpath), "%s%s", pathBase, filename.c_str());
    } else {
        snprintf(netpath, sizeof(netpath), "%s/%s", pathBase, filename.c_str());
    }
    return std::string(netpath);
}


int hd::FileSystem::ReadFromFile(void **dest, size_t *len, const char *fileName) {
    /*
    assert(singleton != nil);
    NSData *data = [singleton ReadFromFile:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]];
    if (data == nil) {
        hd::log::e("Data was nil\n");
        return -1;
    }

    if (((*dest) = malloc(sizeof(unsigned char) * [data length])) == NULL) {
        hd::log::e("Out of mem\n");
        return -1;
    }
    
    *len = [data length];
    [data getBytes:(*dest) length:(NSUInteger)[data length]];*/
    return -1;
}


int hd::FileSystem::WriteToFile(const char* fileName, const void* bytes, const unsigned long len) {
    //assert(singleton != nil);
    std::string fullPathTo = hd::FileSystem::FullPathTo(fileName);
    //NSString *fileLocation = [NSString stringWithCString:fullPathTo.c_str() encoding:NSASCIIStringEncoding];
    //return [singleton WriteToFile:fileLocation withData:[NSData dataWithBytes:bytes length:len]];
}