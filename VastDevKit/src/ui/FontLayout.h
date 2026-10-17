#ifndef _VAST_UI_FONT_LAYOUT_H_
#define _VAST_UI_FONT_LAYOUT_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>
#include <difont/difont.h>

namespace difont {
    class Font;
    class SimpleLayout;
}

template<class T> class ComponentPool;

namespace hd {
    namespace ui {

        enum class TextAlignment {
            LEFT = 0,
            CENTERED,
            JUSTIFY,
            RIGHT
        };

        class FontLayout : public Component {
        private:

            hd::ui::TextAlignment m_alignment;

            float m_lineLength;

            float m_lineSpacing;

            difont::SimpleLayout m_layout;


        protected:

            friend class ComponentPool<FontLayout>;

            FontLayout(unsigned int ID);


        public:

            void Reset();

            void Update();

            void SetAlignment(hd::ui::TextAlignment alignment);

            void SetLineLength(float lineLength);

            void SetLineSpacing(float lineSpacing);

            AABB GetSize(difont::Font *font, const std::string &text);

            const difont::SimpleLayout * GetLayout() const;

        };
    }
}

#endif