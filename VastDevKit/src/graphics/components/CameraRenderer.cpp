#include "CameraRenderer.h"

using namespace hd::graphics;

CameraRenderer::CameraRenderer(unsigned int ID) : Component(ID) {
}


void CameraRenderer::Reset() {
    /* TODO: Reinstate
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
     */
}


void CameraRenderer::RenderViewport(const Viewport *viewport) {

}


void CameraRenderer::RenderPerspective(const PerspectiveData *perspectiveData,
                                       const AABB &aabb) {
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(perspectiveData->GetMatrix().f);
    glTranslatef(-aabb.Center()[0],
                 -aabb.Center()[1],
				 -(aabb.Height()/2.0f));
     */
}


void CameraRenderer::RenderLookAt(const LookAtData *lookAtData) {
    //glMatrixMode(GL_MODELVIEW);
    //glLoadMatrixf(lookAtData->GetMatrix().f);
}


void CameraRenderer::RenderOrthographic(const OrthographicData *orthographicData) {
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(orthographicData->GetMatrix().f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
     */
}


void CameraRenderer::RenderOrthographicLookAt(const OrthographicData *orthographicData,
                                              const LookAtData *lookAtData) {
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(orthographicData->GetMatrix().f);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(lookAtData->GetMatrix().f);
     */
}


void CameraRenderer::RenderProjectionLookAt(const Projection *projection,
                                             const LookAtData *lookAtData) {
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection->GetMatrix().f);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(lookAtData->GetMatrix().f);
     */
}


void CameraRenderer::RenderProjectionMatrix(const Matrix &projection) {
    //glMatrixMode(GL_PROJECTION);
   // glLoadMatrixf(projection.f);
}


void CameraRenderer::RenderModelViewMatrix(const Matrix &modelViewMatrix) {
   // glMatrixMode(GL_MODELVIEW);
   // glLoadMatrixf(modelViewMatrix.f);
}


void CameraRenderer::RenderPerspectiveLookAt(const PerspectiveData *perspectiveData,
                                             const LookAtData *lookAtData) {
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(perspectiveData->GetMatrix().f);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(lookAtData->GetMatrix().f);
     */
}
