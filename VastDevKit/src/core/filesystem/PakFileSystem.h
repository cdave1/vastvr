#ifndef _VAST_PAK_MAKER_H_
#define _VAST_PAK_MAKER_H_

#include <fstream>
#include <regex>
#include <map>
#include <string>

#include "core/filesystem/FileSystem.h"
#include "core/compression/Compression.h"


class PakFileEntry {
public:
	long m_uncompressedSize;
	
	long m_compressedSize;
	
	long m_start;
		
	char m_fileName[256];
	
    std::string m_pathToFile;
};


class PakFile {
public:
    std::string m_pakFileName;
	
    std::string m_fullDirectoryPath;
	
    std::string m_directoryName;
	
	bool m_isCompressed;
	
	std::map<std::string, PakFileEntry *> m_files;
};


class PakFileManifest {
public:
	std::map<std::string, std::string> m_fileLookupTable;
	
	std::map<std::string, PakFile*> m_pakFiles;
	
	int m_pakFileCount;
};


class PakFileSystem {
public:
	static void MakePak(const char *pakManifestFilePath, const char *destDir);
	
	static void InitPakFileSystem(const char *baseDir);
	
	static void PrintFileManifest();
	
    static int OpenFile(const std::string &filename, char **fileBytes, int *fileLen);

    static hd::File * OpenFile(const std::string &filename);
	
};


#endif