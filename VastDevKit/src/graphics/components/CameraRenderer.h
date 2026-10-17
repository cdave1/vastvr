#ifndef _VAST_GRAPHICS_COMPONENTS_CAMERA_RENDERER_H_
#define _VAST_GRAPHICS_COMPONENTS_CAMERA_RENDERER_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Viewport;
        class OrthographicData;
        class Projection;
        class PerspectiveData;
        class LookAtData;
        class Transform;
        
        class CameraRenderer : public Component {
        public:
            
            void Reset();
            
            void RenderViewport(const Viewport *viewport);
            
            void RenderPerspective(const PerspectiveData *perspectiveData, const AABB &aabb);
            
            void RenderLookAt(const LookAtData *lookAtData);
            
            void RenderOrthographic(const OrthographicData *orthographicData);
            
            void RenderOrthographicLookAt(const OrthographicData *orthographicData,
                                          const LookAtData *lookAtData);

            void RenderProjectionLookAt(const Projection *projection,
                                         const LookAtData *lookAtData);

            void RenderPerspectiveLookAt(const PerspectiveData *perspectiveData,
                                         const LookAtData *lookAtData);

            void RenderProjectionMatrix(const Matrix &projection);

            void RenderModelViewMatrix(const Matrix &modelViewMatrix);
            
            
        protected:
            
            friend class ComponentPool<CameraRenderer>;
            
            CameraRenderer(unsigned int ID);
            
            
        };
    }
}

#endif