#include "CurveRenderer.h"
#include "graphics/components/Curve.h"

using namespace hd::graphics;

CurveRenderer::CurveRenderer(unsigned int ID) : Component(ID) {}


void CurveRenderer::RenderCurve(const Curve *curve) {
    Vec3 p1 = curve->GetPoint(0);
    Vec3 p2 = curve->GetPoint(1);
    Vec3 c1 = curve->GetControlPoint(0);
    Vec3 c2 = curve->GetControlPoint(1);
    RenderCurve(p1, p2, c1, c2);
}


void CurveRenderer::RenderCurve(Vec3 p1, Vec3 p2, Vec3 c1, Vec3 c2) {
    glLineWidth(1.0f);
    hd::gl::Begin(GL_LINE_STRIP);
    hd::gl::Color4f(0.0f, 0.0f, 0.0f, 1.0f);
    const int SEGMENTS = 64;
    for (int i = 0; i <= SEGMENTS; ++i) {
        float t = float(i) / float(SEGMENTS);
        Vec3 vec = hd::BezierPoint(t, p1, p2, c1, c2);
        hd::gl::Vertex3f(vec);
    }
    hd::gl::End();
    
    //glPointSize(16.0f);
    glDisable(GL_TEXTURE_2D);
    hd::gl::Begin(GL_POINTS);
    hd::gl::Color4f(1.0f, 0.0f, 1.0f, 1.0f);
    hd::gl::Vertex3f(p1);
    hd::gl::Vertex3f(p2);
    hd::gl::Vertex3f(c1);
    hd::gl::Vertex3f(c2);
    hd::gl::End();
}
