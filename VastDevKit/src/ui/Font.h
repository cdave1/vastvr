#ifndef _VAST_UI_FONT_H_
#define _VAST_UI_FONT_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

namespace difont {
    class Font;
}

template<class T> class ComponentPool;

namespace hd {
    namespace ui {
        class Font : public Component {
        private:
            
            float m_faceSize;
            
            std::string m_fontName;
            
            difont::Font *m_font;
            
            float m_bearingX;
            
            
        protected:
            
            friend class ComponentPool<Font>;
            
            Font(unsigned int ID);
            
            
        public:
            
            void SetFont(difont::Font *font);
            
            void SetFaceSize(const float faceSize);
            
            AABB GetSize(const std::string &text);
            
            difont::Font * GetFont() const;
            
        };
    }
}

#endif
