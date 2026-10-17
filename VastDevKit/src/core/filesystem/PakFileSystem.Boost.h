#ifndef _VAST_PAK_MAKER_H_
#define _VAST_PAK_MAKER_H_

#include <fstream>
#include <regex>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>

#include "core/filesystem/FileSystem.h"
#include "core/compression/Compression.h"


class PakFileEntry {
public:
	long m_uncompressedSize;
	
	long m_compressedSize;
	
	long m_start;
		
	char m_fileName[256];
	
    std::string m_pathToFile;
	
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & m_uncompressedSize;
		ar & m_compressedSize;
		ar & m_start;
		ar & m_fileName;
	}
};


class PakFile {
public:
    std::string m_pakFileName;
	
    std::string m_fullDirectoryPath;
	
    std::string m_directoryName;
	
	bool m_isCompressed;
	
	std::map<std::string, PakFileEntry *> m_files;
	
	
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & m_pakFileName;
		ar & m_directoryName;
		ar & m_isCompressed;
		ar & m_files;
	}
};


class PakFileManifest {
public:
	std::map<std::string, std::string> m_fileLookupTable;
	
	std::map<std::string, PakFile*> m_pakFiles;
	
	int m_pakFileCount;
	
	
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & m_fileLookupTable;
		ar & m_pakFiles;
		ar & m_pakFileCount;
	}
};


class PakFileSystem {
public:
	static void MakePak(const char *pakManifestFilePath, const char *destDir);
	
	static void InitPakFileSystem(const char *baseDir);
	
	static void PrintFileManifest();
	
	static int OpenFile(const char *fileName, char **fileBytes, int *fileLen);

	static hd::File * OpenFile(const char *fileName);
	
};


#endif