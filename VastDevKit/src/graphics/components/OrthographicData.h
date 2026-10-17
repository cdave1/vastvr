#ifndef _VAST_GRAPHICS_ORTHOGRAPHIC_DATA_H_
#define _VAST_GRAPHICS_ORTHOGRAPHIC_DATA_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class OrthographicData : public Component {
        private:
            
            AABB m_aabb;
            
            Matrix m_orthoMat;
            
        protected:
            
            friend class ComponentPool<OrthographicData>;
            
            OrthographicData(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void Update();
            
            void SetAABB(const AABB &aabb);
            
            const AABB& GetAABB() const;
            
            void SetMatrix(const Matrix &m);
            
            const Matrix & GetMatrix() const;
            
        };
    }
}

#endif
