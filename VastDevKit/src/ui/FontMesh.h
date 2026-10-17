#ifndef _VAST_UI_FONT_MESH_H_
#define _VAST_UI_FONT_MESH_H_

#include "core/component/Component.h"
#include <Vast/VastGraphics.h>

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class MeshData;
    }

    namespace ui {
        class Font;
        class FontLayout;

        class FontMesh : public Component {
        private:

            hd::graphics::MeshData *m_currentMesh;

            std::string m_currentText;


        protected:

            friend class ComponentPool<FontMesh>;

            FontMesh(unsigned int ID);


        public:

            hd::graphics::MeshData *GetMeshData() const;

            std::string GetText() const;
            
            void RenderMesh(const hd::ui::Font *font, const std::string text);

            void RenderMesh(const hd::ui::Font *font, const hd::ui::FontLayout *layout, const std::string text);
            
        };
    }
}

#endif
