#ifndef _VAST_UI_FONT_LOADER_H_
#define _VAST_UI_FONT_LOADER_H_

#include <map>
#include <string>

namespace difont {
    class Font;
}

namespace hd {
    namespace ui {

        enum class FontEncoding {
            ADOBE_LATIN_1,
            HD_UNICODE
        };

        class FontFile;
        class FontCacheItem;
        
        class FontLoader {
        private:
            
            std::map <std::string, hd::ui::FontFile *> m_fontFiles;
            
            std::map <std::string, hd::ui::FontCacheItem *> m_fonts;
            
        public:
            
            static FontLoader * Instance();
            
            difont::Font * FindTextureFont(const char *fontName, const unsigned int faceSize, hd::ui::FontEncoding encoding = hd::ui::FontEncoding::HD_UNICODE);
            
            void DestroyFont(const char *fontName, const unsigned int faceSize);
            
            void PrintStats();
            

        private:
            
            FontLoader();
            
            ~FontLoader();
            
            hd::ui::FontFile * FindFontFile(const std::string &fontPath);
            
            difont::Font * LoadDefaultFont(std::string &sfkey, const unsigned int faceSize);
            
        };
    }
}


#endif