#ifndef _VAST_FILEHANDLE_H_
#define _VAST_FILEHANDLE_H_

namespace hd {
    class File {
    public:
        // FILE *hFile;
        
        // Following is used when the file is loaded into memory
        // Was file loaded into memory?
		bool bLoaded;
        
        // Did we use mmap?
		bool mmapped;
		
        // Size of file data in bytes
		unsigned int	 filesize;
		
        // pointer to start of file data block
		unsigned char *ptrStart;
        
        // pointer to current position in file data block
		unsigned char *ptrCurrent;
        
        // pointer to end of file data block
		unsigned char *ptrEnd;
		
        // file data loaded into memory
		void *filedata;
    };
}

#endif