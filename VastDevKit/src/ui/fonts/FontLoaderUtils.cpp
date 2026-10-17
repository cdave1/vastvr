#include "FontLoaderUtils.h"
#include <difont/difont.h>

hd::ui::FontFile::FontFile() :
    m_bytes(NULL),
    m_size(0) {}


hd::ui::FontFile::~FontFile() {
    FreeSafely(m_bytes);
}


hd::ui::FontCacheItem::FontCacheItem() :
    m_filename(),
    m_size(0),
    m_ftFont(NULL),
    m_fontFile(NULL) {}


hd::ui::FontCacheItem::~FontCacheItem() {
    DeleteSafely(m_ftFont);
}