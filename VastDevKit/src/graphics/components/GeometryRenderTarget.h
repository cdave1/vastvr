#ifndef _VAST_GEOMETRY_RENDER_TARGER_H_
#define _VAST_GEOMETRY_RENDER_TARGER_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    class Entity;

    namespace graphics {
        
        class GeometryRenderTarget : public Component {
        private:
            
            uint32_t m_frameBufferId;
            
            uint32_t m_depthBufferId;
            
            Color4f m_clearColor;
            
            Size2 m_size;
            
            Texture *m_colorTexture;

            Texture *m_lightingTexture;
            
            Texture *m_normalsTexture;
            
            Texture *m_depthTexture;
            
            std::function<void(const hd::Entity *)> m_renderCallback;
            
            
        protected:
            
            friend class ComponentPool<GeometryRenderTarget>;
            
            GeometryRenderTarget(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void Clear();

            void ClearColor();

            void ClearDepth();

            void SetDepthMask();

            void SetColorMask();
            
            void Resize(const Size2 &size);
            
            void SetClearColor(const Color4f &color);
            
            void Start();
            
            void End();

            void StartDepthOnly();
            
            const hd::Texture * GetColorTexture() const;

            const hd::Texture * GetLightingTexture() const;
            
            const hd::Texture * GetNormalsTexture() const;
            
            const hd::Texture * GetDepthTexture() const;
            
            Size2 GetTextureSize() const;
            
            template<class T>
            void OnRender(T *src, void (T::*renderGeometryCallback)(const hd::Entity *));
            
            void Render(const hd::Entity *entity);
            
        };
    }
}

template<class T>
void hd::graphics::GeometryRenderTarget::OnRender(T *src, void (T::*renderCallback)(const hd::Entity *)) {
    m_renderCallback = std::bind(renderCallback, src, std::placeholders::_1);
}

#endif