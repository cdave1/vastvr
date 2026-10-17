#include "PakFileSystem.h"


/*
 Notes while developing:
 
 create file map list.
 
 For each folder in strings file
 {
 create new pak file	
 
 For each path in strings, load raw file bytes
 {
 1. Get unpacked size
 2. Store path in map
 3. Create file object
 - pak file.
 - raw size
 - path
 - unpacked name
 Add to map.
 4. Zip the file
 5. Store zipped size in file object
 6. Malloc file object space for zipped
 bytes and memcpy to this space.
 7. Append this data to the pak file.
 
 7. Record heap counter size
 }
 
 combine all raw data into pak file.
 
 }
 Serialize the list of maps as binary and zip as pak0.pak
 */

using namespace boost;

void PakFileSystem::MakePak(const char* pakManifestFilePath, const char *destDir) {
	char tmp[1024];
	void *pakManifestFileBytes;
	size_t pakManifestFileLen;
    std::string line;
	
	std::smatch matchings;
	
	PakFileManifest *pakFileManifest;
	PakFile *pakFile;
	PakFileEntry *pakFileEntry;
	
	int pakFileSize;
	char *pakFileBytes;
	char *uncompressedBytes;
	char *compressedBytes;
	PakFileEntry *tmpPakFileEntry;
	
	// first test: zip the bytes pakFileListFile and write to dest.
	if (0 != hd::FileSystem::ReadFromFile(&pakManifestFileBytes, &pakManifestFileLen, pakManifestFilePath)) {
		hd::log::i("Failed to read file at path: %s\n", pakManifestFilePath);
		return;
	}
	
	pakFileManifest = new PakFileManifest();
	pakFileManifest->m_pakFileCount = 0;
	pakFile = NULL;
	pakFileEntry = NULL;
    
	std::regex dirLinePattern("^(/.+)/(.+):([Z])*$");
	
	std::istringstream iss((char *)pakManifestFileBytes);
	if (!iss.good()) {
		hd::log::i("Could not load manifest file: %s\n", pakManifestFilePath);
		return;
	}
	
	// Parse line by line
	while(!iss.eof()) {
		getline(iss, line);
		
		if (std::regex_match(line, matchings, dirLinePattern)) {
			hdAssert(line == matchings[0]);
			hdAssert(4 == matchings.size());
			
			pakFile = new PakFile();
			pakFileEntry = NULL;
			
			snprintf(tmp, 256, "pak%0d.pak", (int)(pakFileManifest->m_pakFiles.size() + 1));
			
			//cout << matchings[1] << " " << matchings[2] << " " << matchings[3] << " " << std::endl;
			
			pakFile->m_pakFileName = std::string(tmp);
			pakFile->m_fullDirectoryPath = std::string(matchings[1]) + "/" + std::string(matchings[2]);
			pakFile->m_directoryName = matchings[2];
			pakFile->m_isCompressed = (matchings[3] == std::string("Z"));
			
			pakFileManifest->m_pakFiles[pakFile->m_pakFileName] = pakFile;
		} else {
			if (line.length() > 0) {
				// Assume file.
				hdAssert(pakFile != NULL);
				//hdAssert(pakFileEntry == NULL);
				
				pakFileEntry = new PakFileEntry();
				snprintf(pakFileEntry->m_fileName, 256, "%s/%s", pakFile->m_directoryName.c_str(), line.c_str());
				
				pakFileEntry->m_pathToFile = pakFile->m_fullDirectoryPath + "/" + line.c_str();
				pakFileEntry->m_uncompressedSize = 0;
				pakFileEntry->m_compressedSize = 0;
				pakFileEntry->m_start = -1;
				
				pakFile->m_files[std::string(pakFileEntry->m_fileName)] = pakFileEntry;
				
				// Add file to lookup table.
				pakFileManifest->m_fileLookupTable[std::string(pakFileEntry->m_fileName)] = pakFile->m_pakFileName;
			}
		}
	}
	
	std::map<std::string, PakFile*>::iterator pakFiles;
	std::map<std::string, PakFileEntry*>::iterator pakFileContents;
	
	pakFileSize = 0;
	pakFileBytes = NULL;
	uncompressedBytes = NULL;
	compressedBytes = NULL;
	
	/*
	 * Create each pak file.
	 */
	for (pakFiles = pakFileManifest->m_pakFiles.begin(); 
		 pakFiles != pakFileManifest->m_pakFiles.end(); 
		 ++pakFiles) {
		hd::log::i("%s (%s)\n", (*pakFiles).second->m_directoryName.c_str(), (*pakFiles).second->m_pakFileName.c_str());
		
		hdAssert(pakFileBytes == NULL);
		hdAssert(pakFileSize == 0);
		
		for (pakFileContents = (*pakFiles).second->m_files.begin(); 
			 pakFileContents != (*pakFiles).second->m_files.end(); 
			 ++pakFileContents) {
			uncompressedBytes = NULL;
			compressedBytes = NULL;
			tmpPakFileEntry = (*pakFileContents).second;
			
			hd::log::i("\t%s\n", tmpPakFileEntry->m_fileName);
			snprintf(tmp, 1024, "%s", tmpPakFileEntry->m_pathToFile.c_str());
			
			if (0 != hd::FileSystem::ReadFromFile((void**)&uncompressedBytes, (size_t*)&(tmpPakFileEntry->m_uncompressedSize), tmp)) {
				hd::log::i("Failed to read file at path: %s\n", tmp);
			} else {
				if ((*pakFiles).second->m_isCompressed) {
					// zip the bytes
                    int zipResult = hd::Compression::Zip_(uncompressedBytes,
                                                          tmpPakFileEntry->m_uncompressedSize,
                                                          &compressedBytes,
                                                          &(tmpPakFileEntry->m_compressedSize));
                    
					if (zipResult != 0) {
						hd::log::e("hdCompression_Zip FAILED %s\n", tmp);
						break;
					}
				} else {
					if (NULL == (compressedBytes = (char *)malloc(sizeof(char) * tmpPakFileEntry->m_uncompressedSize))) {
						hd::log::e("MALLOC FAILED %s\n", tmp);
						break;
					}
					memcpy(compressedBytes, uncompressedBytes, tmpPakFileEntry->m_uncompressedSize);
					tmpPakFileEntry->m_compressedSize = tmpPakFileEntry->m_uncompressedSize;
				}
				
				tmpPakFileEntry->m_start = pakFileSize;
				pakFileSize += tmpPakFileEntry->m_compressedSize;
				
				if (pakFileBytes == NULL) {
					if (NULL == (pakFileBytes = (char *)malloc(sizeof(char) * pakFileSize))) {
						hd::log::e("MALLOC FAILED %s\n", tmp);
						break;
					}
				} else {
					// reallocate space.
					if (NULL == (pakFileBytes = (char *)realloc(pakFileBytes, sizeof(char) * pakFileSize))) {
                        free(pakFileBytes);
						hd::log::e("REALLOC FAILED %s\n", tmp);
						break;
					}
				}

                hdAssert(compressedBytes);
				
				// We now have enough space to shove the zipped bytes into the pak file
				memcpy(pakFileBytes + (sizeof(char) * (pakFileSize - tmpPakFileEntry->m_compressedSize)), 
					   compressedBytes, 
					   tmpPakFileEntry->m_compressedSize);
				
				// Return resources.
				if (uncompressedBytes != NULL) 
					free(uncompressedBytes);
				
				if (compressedBytes != NULL) 
					free(compressedBytes);	
				
				uncompressedBytes = NULL;
				compressedBytes = NULL;
			}			
		} // eof second loop.
		
		// Our pak file should be ready
		if (pakFileSize > 0 && pakFileBytes != NULL) {
			// pak file name:
			
			snprintf(tmp, 256, "%s/%s", destDir, (*pakFiles).second->m_pakFileName.c_str());
			
			if (false == hd::FileSystem::FileExists(tmp)) {
				hd::FileSystem::CreateFile(tmp);
			}
			
			if (0 != hd::FileSystem::WriteToFile(tmp, pakFileBytes, pakFileSize)) {
				hd::log::e("Failed to write file to path: %s\n", tmp);
			}
			
			hd::log::i("Wrote: %s (%d bytes)\n", tmp, pakFileSize);
			free(pakFileBytes);
		}
		
		pakFileBytes = NULL;
		pakFileSize = 0;
		tmpPakFileEntry = NULL;
		uncompressedBytes = NULL;
		compressedBytes = NULL;
	}
	
	/*
	 * Pak filesystem manifest - always at pak0.pak
	 */
	int writeRes;
	char *ossBuffer;
	long ossLen;
	char *zipped;
	long zippedLen;
	
	snprintf(tmp, 256, "%s/pak0.pak", destDir);
	
	if (!hd::FileSystem::FileExists(tmp)) {
		int createRes = hd::FileSystem::CreateFile(tmp);
		hdAssert (0 == createRes);
	}
	
	std::ostringstream oss(std::ios::in | std::ios::out);
	boost::archive::binary_oarchive boa(oss);
	boa << (* pakFileManifest);
	
	ossLen = oss.str().size() + 1;
	if ((ossBuffer = new char[ossLen]) == NULL) {
		hd::log::i("Out of memory.\n");
		return;
	}
	
	oss.rdbuf()->sgetn(ossBuffer, ossLen);
	
    int res = hd::Compression::Zip_(ossBuffer, ossLen,
                                    &zipped, &zippedLen);
    
	hdAssert(res == 0);
	
	writeRes = hd::FileSystem::WriteToFile(tmp, zipped, zippedLen);
    if (writeRes != 0) {
        hd::log::w("There was a problem writing the pak file out.\n");
    }
	free(zipped);
	
	delete [] ossBuffer;
    
}



static PakFileManifest *rdnlyPakFS = NULL;
static char pakBaseDir[256];

/*
 * Base pak file manifest assumed to be "pak0.pak" in the
 * given baseDir directory.
 *
 * It is stored as the zipped result of a boost binary
 * serialization of a PakFileManifest object.
 *
 * Why a zipped boost binary serialization? Cause we're assholes.
 */
void PakFileSystem::InitPakFileSystem(const char* baseDir) {
	char tmp[256];
	void *zippedManifest, *unzippedManifest;
	long zippedLen, unzippedLen;
	hd::File *hnd;
	
	snprintf(pakBaseDir, 256, "%s", baseDir);
	snprintf(tmp, 256, "%s/pak0.pak", baseDir);
	
	/*
     if (!hd::FileSystem::FileExists(tmp))
     {
     hd::log::i("[PakFileSystem] Pak file manifest %s does not exist!\n", tmp);
     rdnlyPakFS = NULL;
     return;
     }
     
     if (0 != hd::FileSystem::ReadFromFile(&zippedManifest, (int *)&zippedLen, tmp))  
     {
     hd::log::i("[PakFileSystem] Couldn't read from pak file manifest %s\n", tmp);
     rdnlyPakFS = NULL;
     return;
     }*/
	
	if (NULL == (hnd = hd::FileSystem::OpenFile(tmp))) {
		hd::log::i("[PakFileSystem] Pak file manifest %s does not yet exist.  Will attempt to find files in regular file system.\n", tmp);
		rdnlyPakFS = NULL;
		return;		
	}
	
	zippedLen = hd::FileSystem::GetFileSize(hnd);
	if (NULL == (zippedManifest = (char *)malloc(sizeof(char) * zippedLen))) {
		hd::log::i("MALLOC FAIL!!!");
		hdAssert(false);
	}
	
	if (hd::FileSystem::ReadFile(zippedManifest, zippedLen, 1, hnd) > 0) {
		hd::log::i("[PakFileSystem] Couldn't read from pak file manifest %s\n", tmp);
		rdnlyPakFS = NULL;
		return;
	}
	
	
	if (0 != hd::Compression::Unzip_((char *)zippedManifest, zippedLen,
                                    (char **)&unzippedManifest, &unzippedLen)) {
		hd::log::i("[PakFileSystem] Couldn't decompress pak file manifest %s\n", tmp);
		rdnlyPakFS = NULL;
		return;
	}
	
	
	rdnlyPakFS = new PakFileManifest();
	
	std::istringstream iss(std::ios::in | std::ios::out);
	iss.rdbuf()->sputn((const char*)unzippedManifest, unzippedLen);
	boost::archive::binary_iarchive ia(iss);
	ia >> (* rdnlyPakFS);
	
	free(zippedManifest);
	free(unzippedManifest);
	
	hd::FileSystem::CloseFile(hnd);
	
	// TODO: error checks.
}


void PakFileSystem::PrintFileManifest() {
	if (rdnlyPakFS == NULL) {
		hd::log::i("No file manifest.\n");
		return;
	}
	
	std::map<std::string, std::string>::iterator lookupIter;
	
	for (lookupIter = rdnlyPakFS->m_fileLookupTable.begin(); 
		 lookupIter != rdnlyPakFS->m_fileLookupTable.end(); 
		 ++lookupIter) {
		hd::log::i("%s (%s)\n", (*lookupIter).first.c_str(), (*lookupIter).second.c_str());
	}
}



int PakFileSystem::OpenFile(const char *fileName,  char **fileBytes, int *fileLen) {
	PakFile *pakFile;
	PakFileEntry *fileEntry;
	char tmp[256];
	char lookupKey[256];
	char *compressedBuffer;
	char *uncompressedBuffer;
	long uncompressedSize;
	hd::File *pakFileHnd;
	std::string pakFileName;
	
	*fileBytes = NULL;
	*fileLen = 0;
	compressedBuffer = NULL;
	uncompressedBuffer = NULL;
	pakFileHnd = NULL;
	
	if (rdnlyPakFS == NULL) {
		return -1;
	}
	
	/*
	 * If filename starts with a slash then it is not a valid identifier - need
	 * to convert it to a path with the format <<PakAlias>>/<<filename>>.<<extension>>
	 */
	if (fileName[0] == '/') {
		int len = strlen(fileName) - 1;
		uint32_t slashesFound = 0;
		while (slashesFound <= 1) {
			if (fileName[len--] == '/')
				slashesFound++;
		}
		// Note: strlcpy May not work on Android
		strlcpy(lookupKey, &(fileName[len+2]), sizeof(lookupKey));
	} else {
		strlcpy(lookupKey, fileName, sizeof(lookupKey));
	}
	
	/*
	 * File needs to be in the file lookup table, as well as the
	 * specific pak file manifest.
	 */
	if (rdnlyPakFS->m_fileLookupTable.count(std::string(lookupKey)) != 1) {
		hd::log::i("[PakFileSystem] Couldn't find file %s\n", lookupKey);
		goto OpenFileFail;		
	}
	pakFileName = rdnlyPakFS->m_fileLookupTable[std::string(lookupKey)];
	
	/*
	 * So it's a serious inconsistency if the lookup table contains
	 * the pak file but the pak file manifest doesn't.
	 *
	 * Kill the program if this happens.
	 */
	if (rdnlyPakFS->m_pakFiles.count(pakFileName) != 1) {
		hd::log::i("[PakFileSystem] INCONSISTENT STATE. KILLING. %s\n", fileName);
		hdAssert(false);
	}
	pakFile = rdnlyPakFS->m_pakFiles[pakFileName];
	
	/*
	 * File will definitely exist if we're this far.
	 */
	hdAssert(!pakFile->m_files.empty());
	hdAssert(pakFile->m_files.count(std::string(lookupKey)) == 1);
	
	fileEntry = pakFile->m_files[std::string(lookupKey)];
	
	hdAssert(strcmp(fileEntry->m_fileName, lookupKey) == 0);
	
	hd::log::i("Found %s in %s for key %s\n", fileEntry->m_fileName, pakFile->m_pakFileName.c_str(), lookupKey);
	
	/*
	 * Now we get the bytes out of the pak file.
	 *
	 * Use hd::FileSystem::OpenFile for fastness
	 */
	snprintf(tmp, 256, "%s%s", pakBaseDir, pakFile->m_pakFileName.c_str());
	
	pakFileHnd = hd::FileSystem::OpenFile(tmp);
	if (!pakFileHnd) {
		hd::log::i("[PakFileSystem] Couldn't find a physical pak file %s\n", tmp);
		goto OpenFileFail;
	}
	
	if (!(compressedBuffer = (char *)calloc(1, sizeof(char) * fileEntry->m_compressedSize))) {
		hd::log::i("[PakFileSystem] CALLOC FAIL %s\n", tmp);
		goto OpenFileFail;
	}
	
	if (0 != hd::FileSystem::FileSeek(pakFileHnd, fileEntry->m_start, 0)) {
		hd::log::i("[PakFileSystem] Couldn't seek to position in physical pak file %s\n", tmp);
		goto OpenFileFail;
	}
	
	if (hd::FileSystem::ReadFile(compressedBuffer, fileEntry->m_compressedSize, 1, pakFileHnd) == 0) {
		hd::log::i("[PakFileSystem] Couldn't read any data in physical pak file %s\n", tmp);
		goto OpenFileFail;
	}
	
	/*
	 * If the pakfile is compressed, we simply unzip the bytes.
	 *
	 * If it is uncompressed, we need to allocate enough space to
	 * copy the bytes into.
	 *
	 */
	if (pakFile->m_isCompressed) {
        hd::Compression::Unzip_(compressedBuffer, fileEntry->m_compressedSize,
                               &uncompressedBuffer, &uncompressedSize);
		free(compressedBuffer);
	} else {
		if (!(uncompressedBuffer = (char *)calloc(1, sizeof(char) * fileEntry->m_uncompressedSize))) {
			hd::log::i("[PakFileSystem] No space for uncompressed buffer %s\n", tmp);
			goto OpenFileFail;
		}
		
		memcpy(uncompressedBuffer, compressedBuffer, fileEntry->m_uncompressedSize);
		free(compressedBuffer);
		uncompressedSize = fileEntry->m_uncompressedSize;
	}
	
	/*
	 * We have the file bytes, so now prepare the outbuffer to be used by the
	 * caller.
	 */
	if (!(*fileBytes = (char *)calloc(1, sizeof(char) * uncompressedSize))) {
		hd::log::i("[PakFileSystem] CALLOC FAIL %s\n", tmp);
		goto OpenFileFail;
	}
	
	memcpy(*fileBytes, uncompressedBuffer, uncompressedSize);
	*fileLen = uncompressedSize;
	
	free(uncompressedBuffer);
	hd::FileSystem::CloseFile(pakFileHnd);
    
	return 0;
    
	
	
OpenFileFail:
	
	*fileBytes = NULL;
	*fileLen = 0;
	
	if (uncompressedBuffer)
		hdFreeSafely(uncompressedBuffer);
	
	if (compressedBuffer)
		hdFreeSafely(compressedBuffer);
	
	if (pakFileHnd)
		hd::FileSystem::CloseFile(pakFileHnd);
	
	return -1;
	
}


hd::File *PakFileSystem::OpenFile(const char *fileName) {
	hd::File *hnd;
	
	hnd = (hd::File*)calloc(1, sizeof(hd::File));
	//memset(hnd, 0, sizeof( hd::File));
	
	if (-1 == PakFileSystem::OpenFile(fileName, (char **)&hnd->filedata, (int *)&hnd->filesize)) {
		free(hnd);
		return NULL;
	}
	
	hnd->ptrStart =  hnd->ptrCurrent = (unsigned char *)hnd->filedata;
	hnd->ptrEnd =  (unsigned char *)hnd->filedata + hnd->filesize;
	hnd->bLoaded = true;
	hnd->mmapped = false;
	
	return hnd;
}
