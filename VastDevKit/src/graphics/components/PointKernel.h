//
// Generates and stores point cloud information to be
// used by shaders.a
//
#ifndef _VAST_POINT_KERNEL_H_
#define _VAST_POINT_KERNEL_H_

#include <Vast/VastMath.h>
#include "math/Vec3.h"
#include "core/Collections/Collections.h"
#include "core/component/Component.h"

template<class T> class ComponentPool;

#define DEFAULT_KERNEL_SIZE 8

namespace hd {
    namespace graphics {
        class PointKernel : public Component {
        public:
            
            Vector<Vec3> *m_kernel;
            
            
        protected:
            
            friend class ComponentPool<PointKernel>;
            
            PointKernel(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void GenerateSSAO(unsigned size = DEFAULT_KERNEL_SIZE);
            
            void GenerateKernel(unsigned size = DEFAULT_KERNEL_SIZE);
            
            unsigned int GetKernelSize() const;
            
            Vec3 * GetKernelLocation() const;
            
        };
    }
}

#endif