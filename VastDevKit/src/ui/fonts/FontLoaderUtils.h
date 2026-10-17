#ifndef _VAST_FONT_LOADER_UTILS_H_
#define _VAST_FONT_LOADER_UTILS_H_

#include <string>

namespace difont {
    class Font;
}

namespace hd {
    namespace ui {

        class FontLoader;
        
        class FontFile {
        private:
            
            unsigned char *m_bytes;
            
            unsigned long m_size;
            
        private:
            friend class FontLoader;
            
            FontFile();
            
            ~FontFile();
        };
        
        
        class FontCacheItem {
        private:
            std::string m_filename;
            
            unsigned int m_size;
            
            difont::Font *m_ftFont;
            
            FontFile *m_fontFile;
            
        private:
            friend class FontLoader;
            
            FontCacheItem();
            
            ~FontCacheItem();
            
        };
    }
}

#endif
