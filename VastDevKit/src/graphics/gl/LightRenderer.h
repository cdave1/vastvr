#ifndef _VAST_GRAPHICS_LIGHT_RENDERER_H_
#define _VAST_GRAPHICS_LIGHT_RENDERER_H_

#include <Vast/VastMath.h>

namespace hd {
    namespace graphics {
        class Light;
        class LightModel;
        class Material;
        class Shader;

        class LightRenderer {
        public:
            
            static void Begin();
            
            static void RenderLightModel(const LightModel *model);
            
            static void RenderLight(const Light *light);
            
            static void End();

            static void SetMaterialUniforms(const Material *material, const Shader *shader);

            static void SetLightUniforms(const Light *light, const Shader *shader);
            
        };
    }
}

#endif