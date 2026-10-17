#include "LightRenderer.h"
#include "graphics/components/Light.h"
#include "graphics/components/LightModel.h"
#include "graphics/components/Transform.h"
#include "graphics/GL/GraphicsInterface.h"

using namespace hd::graphics;


void LightRenderer::Begin() {
}


void LightRenderer::RenderLightModel(const LightModel *model) {
}


void LightRenderer::RenderLight(const Light *light) {
}


void LightRenderer::End() {
}


void LightRenderer::SetMaterialUniforms(const Material *material, const Shader *shader) {
    shader->SetUniform4f(hd::graphics::MATERIAL_AMBIENT_COLOR_UNIFORM, material->GetAmbient());
    shader->SetUniform4f(hd::graphics::MATERIAL_DIFFUSE_COLOR_UNIFORM, material->GetDiffuse());
    shader->SetUniform4f(hd::graphics::MATERIAL_SPECULAR_COLOR_UNIFORM, material->GetSpecular());
    shader->SetUniform1f(hd::graphics::MATERIAL_SHININESS_UNIFORM, material->GetShininess());
}


void LightRenderer::SetLightUniforms(const Light *light, const Shader *shader) {
    /*
    Vec3 spotDirection = light->GetSpotDirection();
    float spotExponent = light->GetSpotExponent();
    float spotCutoff = light->GetSpotCutoff();

    float constantAttenuation = light->GetConstantAttenuation();
    float linearAttenuation = light->GetLinearAttenuation();
    float quadraticAttenuation = light->GetQuadraticAttenuation();
*/
    /*
    Vec4 lightPosition(light->GetPosition()[0],
                       light->GetPosition()[1],
                       light->GetPosition()[2],
                       light->IsDirectional() ? 1.0f : 0.0f);*/
//    Color4f ambient = light->GetAmbient();
//    Color4f diffuse = light->GetDiffuse();
//    Color4f specular = light->GetSpecular();

   // shader->SetUniform4f(hd::graphics::LIGHT_POSITION_UNIFORM, lightPosition);
    shader->SetUniform4f(hd::graphics::LIGHT_AMBIENT_COLOR_UNIFORM, light->GetAmbient());
    shader->SetUniform4f(hd::graphics::LIGHT_DIFFUSE_COLOR_UNIFORM, light->GetDiffuse());
    shader->SetUniform4f(hd::graphics::LIGHT_SPECULAR_COLOR_UNIFORM, light->GetSpecular());
}
