#ifndef _VAST_GRAPHICS_COMPONENT_PROJECTION_H_
#define _VAST_GRAPHICS_COMPONENT_PROJECTION_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Projection : public Component {
        private:

            Matrix m_projectionMatrix;

            Matrix m_inverseProjectionMatrix;


        protected:

            friend class ComponentPool<Projection>;

            Projection(unsigned int ID);

        public:

            void Reset();

            void SetMatrix(const Matrix &m);

            void SetInverseMatrix(const Matrix &m);

            const Matrix & GetMatrix() const;

            const Matrix & GetInverseMatrix() const;

        };
    }
}

#endif