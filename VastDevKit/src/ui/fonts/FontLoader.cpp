#include "FontLoader.h"
#include <difont/difont.h>
#include "core/filesystem/FileSystem.h"
#include "FontLoaderUtils.h"
#include "DefaultFont.h"

hd::ui::FontLoader *s_fontLoaderInstance = NULL;

hd::ui::FontLoader * hd::ui::FontLoader::Instance() {
    if (!s_fontLoaderInstance) {
        s_fontLoaderInstance = new hd::ui::FontLoader();
    }
    return s_fontLoaderInstance;
}


hd::ui::FontLoader::FontLoader() :
    m_fontFiles(),
    m_fonts() {
}


hd::ui::FontLoader::~FontLoader() {
    std::map<std::string, hd::ui::FontCacheItem *>::iterator iter = m_fonts.begin();
    for (iter = m_fonts.begin(); iter != m_fonts.end(); ++iter) {
        DeleteSafely((* iter).second);
    }
}


difont::Font * hd::ui::FontLoader::FindTextureFont(const char *fontName, const unsigned int faceSize, hd::ui::FontEncoding encoding) {
    char fkey[256];
    snprintf(fkey, 256, "%s:%u", fontName, faceSize);
    std::string sfkey(fkey);
    
    std::map<std::string, hd::ui::FontCacheItem *>::iterator iter = m_fonts.find(sfkey);
    if (iter == m_fonts.end()) {
        if (strcmp(fontName, "DEFAULT") == 0) {
            return LoadDefaultFont(sfkey, faceSize);
        }

        hd::ui::FontCacheItem *item = NULL;

        hd::ui::FontFile *fontFile = this->FindFontFile(fontName);
        if (!fontFile) {
            hd::log::e("FontContext: Error loading font: %s\n", fontName);
            return LoadDefaultFont(sfkey, faceSize);
        }
        
        difont::Font *font = new difont::PolygonFont(fontFile->m_bytes, fontFile->m_size);
        if (font && font->Error()) {
            hd::log::e("FontContext: Error created FTPolygonFont: %s\n", fontName);
            DeleteSafely(font);
            return LoadDefaultFont(sfkey, faceSize);
        }
        
        if (!font->FaceSize(faceSize)) {
            hd::log::e("FontContext: Error setting face size font: %s\n", fontName);
        }
        if (encoding == hd::ui::FontEncoding::ADOBE_LATIN_1) {
            font->CharMap(FT_ENCODING_ADOBE_LATIN_1);
		} else if (encoding == hd::ui::FontEncoding::HD_UNICODE) {
            font->CharMap(FT_ENCODING_UNICODE);
        }
        
        item = new FontCacheItem();
        item->m_filename = fontName;
        item->m_size = faceSize;
        item->m_ftFont = font;
        item->m_fontFile = fontFile;
        m_fonts[sfkey] = item;
        return font;
    }
    
    return (* iter).second->m_ftFont;
}


void hd::ui::FontLoader::DestroyFont(const char *fontName, const unsigned int faceSize) {
    char fkey[256];
    snprintf(fkey, 256, "%s:%u", fontName, faceSize);
    
    std::string sfkey(fkey);
    
    std::map<std::string, hd::ui::FontCacheItem *>::iterator iter = m_fonts.find(sfkey);
    if (iter != m_fonts.end()) {
        hd::ui::FontCacheItem * fontItem = (* iter).second;
        DeleteSafely(fontItem);
        m_fonts.erase(iter);
    }
}


void hd::ui::FontLoader::PrintStats() {
    hd::log::i("[FONT CONTEXT] %d fonts used.\n\n", int(m_fonts.size()));
    std::map<std::string, hd::ui::FontCacheItem *>::iterator iter = m_fonts.begin();
    
    for (iter = m_fonts.begin(); iter != m_fonts.end(); ++iter) {
        hd::log::i("\t%s\n", (* iter).first.c_str());
    }
}


hd::ui::FontFile * hd::ui::FontLoader::FindFontFile(const std::string &path) {
    std::map<std::string, hd::ui::FontFile *>::iterator iter = m_fontFiles.find(path);
    if (iter != m_fontFiles.end()) {
        return (* iter).second;
    }
    
    hd::File *file = NULL;
	if (NULL == (file = hd::FileSystem::OpenFile(path.c_str()))) {
		hd::log::i("[FontLoader]: Could not open (%s) for reading. Couldn't find file\n", path.c_str());
		return NULL;
    }
	
	unsigned char *buffer = NULL;
	int len = hd::FileSystem::GetFileSize(file);
	if (NULL == (buffer = (unsigned char *)calloc(1, sizeof(unsigned char) * len))) {
        hd::log::e("CALLOC!\n");
		return NULL;
	}
	
	hd::FileSystem::ReadFile(buffer, len, 1, file);
    hd::FileSystem::CloseFile(file);
    
    hd::ui::FontFile *fontFile = new hd::ui::FontFile();
    fontFile->m_bytes = buffer;
    fontFile->m_size = len;
    m_fontFiles[path] = fontFile;
    return fontFile;
}


difont::Font * hd::ui::FontLoader::LoadDefaultFont(std::string &sfkey, const unsigned int faceSize) {
    difont::Font *font = new difont::PolygonFont ((unsigned char *)DefaultFontBytes, DefaultFontLen);
    if (!font || font->Error()) {
        hd::log::e("Font error.");
        return NULL;
    }

    if (!font->FaceSize(faceSize)) {
        hd::log::e("FontLoader: Error setting face size for default font\n");
    }
    font->CharMap(FT_ENCODING_UNICODE);
    
    hd::ui::FontCacheItem *item = new hd::ui::FontCacheItem();
    item->m_size = faceSize;
    item->m_ftFont = font;
    item->m_fontFile = NULL;
    m_fonts[sfkey] = item;
    return font;
}
