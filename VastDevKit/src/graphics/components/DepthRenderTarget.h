#ifndef _VAST_GRAPHICS_DEPTH_RENDER_TARGET_H_
#define _VAST_GRAPHICS_DEPTH_RENDER_TARGET_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

///
/// For rendering to a depth buffer texture.  Very similar to hd::graphics::DepthRenderTarget
///
namespace hd {
    namespace graphics {
        class Shader;
        
        class DepthRenderTarget : public Component {
        private:
            
            uint32_t m_frameBufferId;
            
            uint32_t m_depthBufferId;
            
            Color4f m_clearColor;
            
            Size2 m_size;
            
            Texture *m_depthTexture;
            
            std::function<void(const hd::Entity *)> m_renderGeometryCallback;
            
            
        protected:
            
            friend class ComponentPool<DepthRenderTarget>;
            
            DepthRenderTarget(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void Clear();
            
            void Resize(const Size2 &size);
            
            void SetClearColor(const Color4f &color);
            
            void Start();
            
            void End();
            
            const hd::Texture * GetDepthTexture() const;
            
            void BindTexturesToShader(Shader *shader,
                                      const std::string &depthSamplerName = "iDepthTexture");
            
            Size2 GetTextureSize() const;
            
            template<class T>
            void OnRenderGeometry(T *src, void (T::*renderGeometryCallback)(const hd::Entity *));
            
            void RenderGeometry(const hd::Entity *entity);
            
            
        };
    }
}

template<class T>
void hd::graphics::DepthRenderTarget::OnRenderGeometry(T *src, void (T::*renderGeometryCallback)(const hd::Entity *)) {
    m_renderGeometryCallback = std::bind(renderGeometryCallback, src, std::placeholders::_1);
}

#endif
