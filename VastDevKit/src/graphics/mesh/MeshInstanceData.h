#ifndef _VAST_GRAPHICS_MESH_INSTANCE_DATA_H_
#define _VAST_GRAPHICS_MESH_INSTANCE_DATA_H_

#include <Vast/VastMath.h>

///
/// Data specific to each instance of a mesh when renderering with gl*Instanced funtions.
///
namespace hd {
    class Texture;
    
    namespace graphics {

        typedef struct MeshInstanceData {
            MeshInstanceData() :
                matrix(),
                materialAmbientColor() {}

            Matrix matrix;

            Color4ub materialAmbientColor;

            Color4ub materialDiffuseColor;

            Color4ub materialSpecularColor;

            float materialShininess;

        } MeshInstanceData;
    }
}

#endif