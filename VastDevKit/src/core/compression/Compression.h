#ifndef _VAST_COMPRESSION_H_
#define _VAST_COMPRESSION_H_

#undef Byte
//#include <zconf.h>
#include <zlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
/*
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
*/
#include "core/Common/Common.h"

namespace hd  {
    class Compression {
    public:
        /*
         * 0 if ok, < 0 if error
         */
        static int Zip_(const char * inBuffer,
                       const long inLen, 
                       char ** outBuffer, 
                       long * outLen);
        
        /*
         * 0 if ok, < 0 if error
         */
        static int Unzip_(const char * inBuffer,
                         const long inLen, 
                         char ** outBuffer, 
                         long * outLen);

        /*
        std::string Zip(const std::string &unzipped);
        
        std::string Unzip(const std::string &zipped);
         */
    };
}

#endif