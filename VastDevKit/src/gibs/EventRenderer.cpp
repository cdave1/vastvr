#include "EventRenderer.h"
#include "graphics/GL/GraphicsInterface.h"
#include "gibs/pointer/Position.h"
#include "core/Device/Device.h"

using namespace hd::input;

EventRenderer::EventRenderer(unsigned int ID) : Component(ID) {}


void EventRenderer::RenderCamera() {
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    hd::gl::Orthof(0, hd::Device::GetDisplayScreenWidth(),
                   0, hd::Device::GetDisplayScreenHeight(),
                   -10000, 10000);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}


void EventRenderer::RenderPointerSelect(const pointer::Position *pointer) {
    Vec3 pos = pointer->GetPosition();
    
    //glPointSize(16.0f);
    glDisable(GL_TEXTURE_2D);
    hd::gl::Begin(GL_POINTS);
    hd::gl::Color4f(1.0f, 0.0f, 0.0f, 1.0f);
    hd::gl::Vertex3f(pos);
    hd::gl::End();
}


void EventRenderer::RenderPointerDrag(const pointer::Position *pointer) {
    Vec3 to    = pointer->GetPosition();
    Vec3 from  = pointer->GetPosition() - pointer->GetDelta();
    
    glDisable(GL_TEXTURE_2D);
    hd::gl::Begin(GL_LINES);
    hd::gl::Color4f(1.0f, 0.0f, 1.0f, 1.0f);
    hd::gl::Vertex3f(from);
    hd::gl::Vertex3f(to);
    hd::gl::End();
}



void EventRenderer::RenderPointerRelease(const pointer::Position *pointer) {
    Vec3 pos = pointer->GetPosition();
    
    //glPointSize(16.0f);
    glDisable(GL_TEXTURE_2D);
    hd::gl::Begin(GL_POINTS);
    hd::gl::Color4f(1.0f, 0.0f, 1.0f, 1.0f);
    hd::gl::Vertex3f(pos);
    hd::gl::End();
}
