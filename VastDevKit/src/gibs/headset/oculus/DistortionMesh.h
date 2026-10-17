#ifndef _HD_INPUT_DISTORTION_VERTEX_BUFFER_H_
#define _HD_INPUT_DISTORTION_VERTEX_BUFFER_H_

#include <Vast/VastCore.h>
#include "gibs/headset/oculus/DistortionVertex.h"

namespace hd {
    namespace graphics {
        class Shader;
    }

    namespace input {
        class DistortionMesh {
        private:

            uint32_t m_vertexArrayObject;

            uint32_t m_vertexBufferObject;

            uint32_t m_vertexCount;

            uint32_t m_indexBufferObject;

            uint32_t m_indexCount;


        protected:

            friend class OculusHMD;

            DistortionMesh();


        public:

            void Reset();

            void SetVertices(const hd::Vector<hd::input::DistortionVertex> *vertices);

            void SetIndices(unsigned short *data, unsigned int indexCount);

            void SetDefaultAttributeLocations(const hd::graphics::Shader *shader);

            void Render();

            void RenderLines();
            
        };
    }
}

#endif
