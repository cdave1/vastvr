#include "PolygonRenderer.h"
#include "graphics/components/Polygon.h"
#include "graphics/components/Material.h"
#include "graphics/GL/GraphicsInterface.h"

using namespace hd::graphics;

PolygonRenderer::PolygonRenderer(unsigned int ID) : Component(ID),
    m_currentFrame(0)
{
}


void PolygonRenderer::Reset()
{
    m_currentFrame = 0;
}


void PolygonRenderer::Update()
{
    ++m_currentFrame;
}


void PolygonRenderer::RenderSphere(const hd::graphics::Polygon *polygon) const {
    const Vector<Vertex> *vertices = polygon->GetVertices();
    
	hd::gl::Begin(GL_TRIANGLE_STRIP);
	for (uint32_t i = 0; i < vertices->Size(); ++i) {
        Vertex vertex = vertices->At(i);
        
        hd::gl::Color4f(vertex.color);
        hd::gl::Vertex3f(vertex.origin);
	}
	hd::gl::End();
}


void PolygonRenderer::RenderLines(const hd::graphics::Polygon *polygon) const {
    const Vector<Vertex> *vertices = polygon->GetVertices();
    
	for (uint32_t i = 0; i < vertices->Size(); i += 2) {
        Vertex from = vertices->At(i);
        Vertex to   = vertices->At(i+1);
        
        hd::gl::Begin(GL_LINES);
        hd::gl::Color4f(from.color);
        hd::gl::Vertex3f(from.origin);
        hd::gl::Color4f(to.color);
        hd::gl::Vertex3f(to.origin);
        hd::gl::End();
	}
}


void PolygonRenderer::RenderLineStrip(const hd::graphics::Polygon *polygon) const {
    const Vector<Vertex> *vertices = polygon->GetVertices();
    
	hd::gl::Begin(GL_LINE_STRIP);
	for (uint32_t i = 0; i < vertices->Size(); ++i) {
        Vertex vertex = vertices->At(i);
        
        hd::gl::Color4f(vertex.color);
        hd::gl::Vertex3f(vertex.origin);
	}
	hd::gl::End();
}


void PolygonRenderer::RenderSolid(const hd::graphics::Polygon *polygon, const Color4f &color) const {
    const Vector<Vertex> *vertices = polygon->GetVertices();
    GLenum prim = 0;
	
	if (vertices->Size() == 3) {
		prim = GL_TRIANGLES;
	} else if (vertices->Size() == 4) {
		prim = GL_QUADS;
	} else {
//#if TARGET_GL_OPENGL == 1
//		prim = GL_POLYGON;
//#else
		prim = GL_TRIANGLE_FAN;
//#endif
	}
	
	hd::gl::Begin(prim);
	hd::gl::Color4f(color);
	
	for (uint32_t i = 0; i < vertices->Size(); ++i) {
        Vertex vertex = vertices->At(i);
        
        hd::gl::Vertex3f(vertex.origin);
	}
	
	hd::gl::End();
}


void PolygonRenderer::RenderNormals(const hd::graphics::Polygon *polygon) const {
    glLineWidth(1.0f);
    const Vector<Vertex> *vertices = polygon->GetVertices();
    
	for (uint32_t i = 0; i < vertices->Size(); ++i) {
        hd::gl::Begin(GL_LINES);
        hd::gl::Color4f(Color4f(1, 1, 1, 1));
        hd::gl::Vertex3f(vertices->At(i).origin);
        hd::gl::Color4f(Color4f(1, 1, 1, 1));
        hd::gl::Vertex3f(vertices->At(i).origin + (10.0f * vertices->At(i).normal));
        hd::gl::End();
	}
}


void PolygonRenderer::RenderPolygon(const hd::graphics::Polygon *polygon) const {
    const Vector<Vertex> *vertices = polygon->GetVertices();
    GLenum prim = 0;
	
	if (vertices->Size() == 3) {
		prim = GL_TRIANGLES;
	} else if (vertices->Size() == 4) {
		prim = GL_QUADS;
	} else {
//#if TARGET_GL_OPENGL == 1
//		prim = GL_POLYGON;
//#else
		prim = GL_TRIANGLE_FAN;
//#endif
	}
    
	hd::gl::Begin(prim);
	for (uint32_t i = 0; i < vertices->Size(); ++i) {
        Vertex vertex = vertices->At(i);
        
        hd::gl::TexCoord2f(vertex.texCoord);
        hd::gl::Color4f(vertex.color);
        hd::gl::Vertex3f(vertex.origin);
        
       // hd::log::i("%4.0f, %4.0f, %4.0f\n", vertex.origin[0], vertex.origin[1], vertex.origin[2]);
	}
//    hd::log::i("\n");
	hd::gl::End();
}
