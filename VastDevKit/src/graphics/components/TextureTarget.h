#ifndef _VAST_GRAPHICS_TEXTURE_TARGET_H_
#define _VAST_GRAPHICS_TEXTURE_TARGET_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Shader;
        
        class TextureTarget : public Component {
        private:
            
            uint32_t m_frameBufferId;
            
            Color4f m_clearColor;
            
            Size2 m_size;
            
            Texture *m_texture;
            
            
        protected:
            
            friend class ComponentPool<TextureTarget>;
            
            TextureTarget(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void Clear();
            
            void Resize(const Size2 &size);
            
            void SetClearColor(const Color4f &color);
            
            void Start();
            
            void End();
            
            const hd::Texture * GetTexture() const;
            
            Size2 GetTextureSize() const;
            
            
        };
    }
}

#endif
