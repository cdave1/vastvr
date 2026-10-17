#ifndef _VAST_WINDOWS_FILESYSTEM_H_
#define _VAST_WINDOWS_FILESYSTEM_H_

#include "core/filesystem/FileSystem.h"
#include "core/filesystem/PakFileSystem.h"

namespace hd {
	class WindowsFileSystem {
	public:
		/*
		* Creates an editable file in docs folder.
		*
		* Returns 0 on success, negative number on fail. Will return
		* -1 if an file with the given name already exists.
		*/
		static int CreateFile(const std::string &fileName);


		/*
		* File existance in document folder. Returns false
		* if the file does not exist; prints out NSError contents
		* if NSError.
		*/
		static bool FileExists(const std::string &fileName);


		/*
		* Writes data to a file in the docs folder.
		*
		* If the file does not exist, an error will be returned.
		*
		* Returns 0 on success.
		*/
		static int WriteToFile(const std::string &fileName, std::string &data);


		/*
		* Read all data from file into NSData object.
		*/
		static std::string ReadFromFile(const std::string &filename);
	};
}


#endif