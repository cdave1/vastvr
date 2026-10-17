#ifndef _VAST_GRAPHICS_COMPONENT_PERSPECTIVE_DATA_H_
#define _VAST_GRAPHICS_COMPONENT_PERSPECTIVE_DATA_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class PerspectiveData : public Component {
        private:
            
            float m_fovY;
            
            float m_aspectY;
            
            float m_near;
            
            float m_far;
            
            Matrix m_perspectiveMat;
            
            
        protected:
            
            friend class ComponentPool<PerspectiveData>;
            
            PerspectiveData(unsigned int ID);            
            
        public:
            
            void Reset();
            
            void Update();
            
            void SetFovY(float fovY);
            
            float GetFovY() const { return m_fovY; }
            
            void SetAspectY(float aspectY);
            
            float GetAspectY() const { return m_aspectY; }
            
            void SetPerspective(float fovY,
                                float aspectY,
                                float near,
                                float far);
            
            void UpdatePerspective();
            
            void SetMatrix(const Matrix &m);
            
            const Matrix & GetMatrix() const;
            
            const Matrix GetMatrixInverse() const;
            
            
        };
    }
}

#endif