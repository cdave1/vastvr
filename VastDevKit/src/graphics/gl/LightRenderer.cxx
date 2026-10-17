#include "LightRenderer.h"
#include "Light.h"
#include "LightModel.h"
#include "Transform.h"
#include "graphics/GL/GraphicsInterface.h"

using namespace hd::graphics;

void LightRenderer::Begin() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}


void LightRenderer::RenderLightModel(const LightModel *model) {
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model->GetAmbient().f);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
}


void LightRenderer::RenderLight(const Light *light) const {
    Vec3 spotDirection = light->GetSpotDirection();
    float spotExponent = light->GetSpotExponent();
    float spotCutoff = light->GetSpotCutoff();
    
    float constantAttenuation = light->GetConstantAttenuation();
    float linearAttenuation = light->GetLinearAttenuation();
    float quadraticAttenuation = light->GetQuadraticAttenuation();
    
    Vec4 position(light->GetPosition()[0],
                  light->GetPosition()[1],
                  light->GetPosition()[2],
                  light->IsDirectional() ? 1.0f : 0.0f);
    Color4f ambient = light->GetAmbient();
    Color4f diffuse = light->GetDiffuse();
    Color4f specular = light->GetSpecular();

    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection.v);
    glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
    glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff);
    
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constantAttenuation);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    
    glLightfv(GL_LIGHT0, GL_POSITION, position.v);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient.f);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse.f);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular.f);
}


void LightRenderer::End() {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);
}
