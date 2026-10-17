#include "OSXFileSystem.h"

@implementation OSXFileSystem

static OSXFileSystem *singleton = nil;
static std::string gamedir = "";

void hd::FileSystem::Init(const std::string &baseDir) {
	if (singleton == nil) {
		singleton = [[OSXFileSystem alloc] init];
        gamedir = baseDir;
#ifndef LEVEL_EDITOR
		PakFileSystem::InitPakFileSystem(gamedir.c_str());
#endif
		//hdPakFileSystem::InitPakFileSystem(gamedir);1
	}
}


const std::string& hd::FileSystem::BaseDir() {
    assert(gamedir.length() > 0);
    return gamedir;
}


/*
 -----------------------------------------------------------------------------
 Function: hdGetFileSize() -Get the length of a file.
 
 Parameters: filestream -[in] Target file handle.
 
 Returns: The file length in bytes.
 
 Notes: 
 -----------------------------------------------------------------------------
 */
unsigned int hd::FileSystem::GetFileSize(hd::File *fhandle) {
	return fhandle->filesize;
}


/*
 -----------------------------------------------------------------------------
 Function: hdFileSeek() -Moves the file pointer to a specified location.
 
 Parameters: 
 fhandle -[in] Pointer to hd::File structure.
 offset -[in] Number of bytes from origin
 origin -[in] Initial position
 SEEK_SET -Beginning of file.
 SEEK_CUR -Current position of file pointer.
 SEEK_END -End of file.
 
 Returns: If successful zero, otherwise a nonzero value.
 
 Notes: 
 -----------------------------------------------------------------------------
 */
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


/*
 -----------------------------------------------------------------------------
 Function: hdCloseFile -Close file handle.
 
 Parameters: filestream -[in] Pointer to valid FILE structure.		 
 
 Returns: Nothing.
 
 Notes: Closes a file stream that was returned by hdFOpenFile.
 
 -----------------------------------------------------------------------------
 */
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


/*
 -----------------------------------------------------------------------------
 Function: hdOpenFile -Open file from the file system.
 
 By default, look in pakFileSystem first!
 
 If the file could not be found in the pakFileSystem, and contains a leading
 slash, then assume it is an absolute path.  If not, assume it is a relative
 path.
 -----------------------------------------------------------------------------
 */
hd::File *hd::FileSystem::OpenFile(const std::string &filename) {
	std::string finalPath;
	hd::File *hFile;
	const char *pathBase;
	struct stat s;
	int fd;

#ifndef LEVEL_EDITOR
	hd::log::i("Searching pak files for %s\n", filename.c_str());
	if (NULL != (hFile = PakFileSystem::OpenFile(filename))) {
		return hFile;
	}
#endif

	if (filename[0] == '/') {
		// Just use full filename
        finalPath = filename;
	} else {
		pathBase = hd::FileSystem::BaseDir().c_str();
        if (pathBase[strlen(pathBase) - 1] == '/') {
            finalPath = hd::FileSystem::BaseDir() + filename;
        } else {
            finalPath = hd::FileSystem::BaseDir() + "/" + filename;
        }
	}
	
	// high performance file mapping path, avoiding stdio
	fd = open( finalPath.c_str(), O_RDONLY );
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


/*
 -----------------------------------------------------------------------------
 Function: hdReadFile -Reads data from a stream.
 
 Parameters: buffer -[in/out] Storage location for data.
 size -[in] Item size in bytes.
 count -[in] Maximum number of items to be read.
 fhandle -[in] Pointer to valid hd::File structure.
 
 Returns: On success number of full items actually read, otherwise -1.
 
 Notes:
 
 -----------------------------------------------------------------------------
 */
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
	assert(singleton != nil);
	[singleton release];
	singleton = nil;
}


int hd::FileSystem::CreateFile(const std::string &fileName) {
	assert(singleton != nil);
    std::string fullPathTo = hd::FileSystem::FullPathTo(fileName);
    NSString *fileLocation = [NSString stringWithCString:fullPathTo.c_str() encoding:NSASCIIStringEncoding];
	return [singleton CreateFile:fileLocation];
}


bool hd::FileSystem::FileExists(const std::string &fileName) {
	assert(singleton != nil);
	return [singleton FileExists:[NSString stringWithCString:fileName.c_str() encoding:NSASCIIStringEncoding]];
}


std::string hd::FileSystem::FullPathTo(const std::string &filename) {
    char netpath[256];
    const std::string pathBase = hd::FileSystem::BaseDir();
    if (pathBase[pathBase.length() - 1] == '/') {
        snprintf(netpath, sizeof(netpath), "%s%s", pathBase.c_str(), filename.c_str());
    } else {
        snprintf(netpath, sizeof(netpath), "%s/%s", pathBase.c_str(), filename.c_str());
    }
    return std::string(netpath);
}


int hd::FileSystem::ReadFromFile(void **dest, size_t *len, const char *fileName) {
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
    [data getBytes:(*dest) length:(NSUInteger)[data length]];
	return 0;
}


int hd::FileSystem::WriteToFile(const std::string &fileName, const void* bytes, const unsigned long len) {
	assert(singleton != nil);
    std::string fullPathTo = hd::FileSystem::FullPathTo(fileName);
    NSString *fileLocation = [NSString stringWithUTF8String:fullPathTo.c_str()];
	return [singleton WriteToFile:fileLocation withData:[NSData dataWithBytes:bytes length:len]];
}


- (NSInteger)CreateFile:(NSString *)fileName {
	NSFileManager *fileManager = [NSFileManager defaultManager];
	if ([fileManager fileExistsAtPath:fileName]) {
#ifdef DEBUG
		hd::log::e("OSX FileSystem error: cannot overwrite file %s:\n",
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return -1;
	}
	
	if (NO == [fileManager createFileAtPath:fileName contents:[NSData data] attributes:[NSDictionary dictionary]]) {
#ifdef DEBUG
		hd::log::e("OSX FileSystem error: cannot create file (%s) at path:\n",
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif		
	}
	return 0;
}


- (BOOL)FileExists:(NSString *)fileName {
	NSFileManager *fileManager = [NSFileManager defaultManager];
	return [fileManager isWritableFileAtPath:fileName];
}


- (NSInteger)WriteToFile:(NSString *)fileName withData:(NSData *)fileData {
	NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error = nil;
    if (![fileManager createDirectoryAtPath:[fileName stringByDeletingLastPathComponent]
               withIntermediateDirectories:YES attributes:nil error:&error] ||
        ![fileData writeToFile:fileName options:NSDataWritingAtomic error:&error]) {
        hd::log::e("OSX FileSystem error writing %s: %s\n", [fileName UTF8String],
                   [[error localizedDescription] UTF8String]);
        return -1;
    }
    return 0;
}


- (NSData *)ReadFromFile:(NSString *)fileName {
	NSData *data = nil;
	BOOL isDir;
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	if (NO == [fileManager isReadableFileAtPath:fileName]) {
#ifdef DEBUG
		hd::log::e("OSX FileSystem error: cannot read from file %s because it does not exist\n",
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return nil;
	}
	
	[fileManager fileExistsAtPath:fileName isDirectory:&isDir];
	if (YES == isDir) {
		return nil;
	}
	
	NSFileHandle *hnd = [NSFileHandle fileHandleForReadingAtPath:fileName];
	hdAssert(hnd != nil);
	
	@try {
		data = [hnd availableData];
	} @catch(NSException *exception) {
		hd::log::e("OSX FileSystem error: cannot read DATA from file %s because of the following reason:\n",
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
		hd::log::e("\tName: %s\n\tReason: %s\n",
			   [[exception name] UTF8String],
			   [[exception reason] UTF8String]);
	}
	
	[hnd closeFile];
	
	if ([data length] == 0) {
		return nil;
	}
	
	return data;
}

@end
