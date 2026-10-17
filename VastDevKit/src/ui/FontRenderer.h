#ifndef _VAST_UI_FONT_RENDERER_H_
#define _VAST_UI_FONT_RENDERER_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Shader;
    }

    namespace ui {
        class Font;
        class FontLayout;

        class FontRenderer : public Component {
        private:

            int32_t m_originLocation;

            int32_t m_colorLocation;

            int32_t m_normalLocation;

            int32_t m_tangentLocation;

            int32_t m_texCoordLocation;


        protected:

            friend class ComponentPool<FontRenderer>;

            FontRenderer(unsigned int ID);


        public:

            void SetDefaultAttributeLocations(const hd::graphics::Shader *shader);

            void SetOriginAttributeLocation(const int32_t location);

            void SetColorAttributeLocation(const int32_t location);

            void SetNormalAttributeLocation(const int32_t location);

            void SetTangentAttributeLocation(const int32_t location);

            void SetTexCoordAttributeLocation(const int32_t location);
            
            void Render(hd::ui::Font *font, std::string text) const;

            void Render(hd::ui::Font *font, hd::ui::FontLayout *layout, std::string text) const;
            
        };
    }
}

#endif
