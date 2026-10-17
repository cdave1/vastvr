#include "Polygon.h"

using namespace hd::graphics;

Polygon::Polygon(unsigned int ID) : Component(ID),
  m_vertices(NULL),
  m_texRepeatX(1.0f),
  m_texRepeatY(1.0f) {
	m_vertices = new hd::Vector<Vertex>(32);
}


void Polygon::Reset() {
    m_vertices->Clear();
}


void Polygon::AddVertex(Vertex vertex) {
    m_vertices->Add(vertex);
}


void Polygon::AddVertex(Vec3 vertex) {
    Vertex v;
    v.origin = vertex;
    m_vertices->Add(v);
}


void Polygon::AddVertex(const float x, const float y) {
    Vertex v;
    v.origin.Set(x, y, 0);
    m_vertices->Add(v);
}


void Polygon::AddVertex(const float x, const float y, const float z) {
    Vertex v;
    v.origin.Set(x, y, z);
    m_vertices->Add(v);
}


void Polygon::AddVertex(const float x, const float y, const float z, const Vec3 &offset) {
    Vertex v;
    v.origin.Set(x + offset[0], y + offset[1], z + offset[2]);
    m_vertices->Add(v);
}


void Polygon::AddLine(const Vec3 &from, const Vec3 &to) {
    Vertex f;
    f.origin.Set(from);
    f.color.Set(1, 1, 1, 1);
    AddVertex(f);
    
    Vertex t;
    t.origin.Set(to);
    t.color.Set(1, 0, 1, 0.5);
    AddVertex(t);
}


void Polygon::RemoveVertex(const int index) {
    m_vertices->RemoveAtIndex(index);
}


void Polygon::ClearVertices() {
    m_vertices->Clear();
}


void Polygon::Rotate(const Vec3 &rotationVector, const Vec3 &rotationAxis) {
    if (rotationVector[0] == 0 && rotationVector[1] == 0 && rotationVector[2] == 0) return;
    
    for (unsigned i = 0; i < m_vertices->Size(); ++i) {
        Vertex *v = m_vertices->Item(i);
        v->origin -= rotationAxis;
	}
    
    {
        Matrix rot;
        Matrix::RotationXYZOrigin(rot, rotationVector);
        
        for (unsigned i = 0; i < m_vertices->Size(); ++i) {
            Vertex *v = m_vertices->Item(i);
            Matrix::Vec3Multiply(v->origin, v->origin, rot);
        }
    }
    
    for (unsigned i = 0; i < m_vertices->Size(); ++i) {
        Vertex *v = m_vertices->Item(i);
        v->origin += rotationAxis;
	}
}


void Polygon::Scale(const Vec3 &scaleVector, const Vec3 &scaleAxis) {
    hd::TranslateVertices(m_vertices->GetItems(),
						m_vertices->Size(),
						-scaleAxis);
	
    hd::ScaleVertices(m_vertices->GetItems(),
                    m_vertices->Size(),
                    scaleVector);
	
    hd::TranslateVertices(m_vertices->GetItems(),
						m_vertices->Size(),
						scaleAxis);
	this->ResetAABB();
}


void Polygon::MoveTo(const Vec3 &center) {
    
}


void Polygon::MoveBy(const Vec3 &delta) {
    for (unsigned i = 0; i < m_vertices->Size(); ++i) {
        Vertex *v = m_vertices->Item(i);
        v->origin += delta;
	}
    
	this->ResetAABB();
}


void Polygon::Create2DBox(const AABB &aabb) {
    ClearVertices();
	
	this->AddVertex(aabb.lower[0], aabb.lower[1]);
	this->AddVertex(aabb.lower[0], aabb.upper[1]);
	this->AddVertex(aabb.upper[0], aabb.upper[1]);
	this->AddVertex(aabb.upper[0], aabb.lower[1]);
    
	this->ResetTextureCoords();
    this->ResetNormals();
}


void Polygon::Create2DCylinder(const Vec3 &center,
                               const float radius,
                               const int segments) {
    ClearVertices();
	
	const float k_increment = 2.0f * hd_pi / segments;
	float theta = 0.0f;
	
	for (int i = 0; i < segments; ++i) {
		Vec3 v = center + radius * Vec3(cosf(theta), sinf(theta), 0);
		this->AddVertex(v);
		theta -= k_increment;
	}
	this->ResetTextureCoords();
    this->ResetNormals();
}


/*
 * 5 point star with tips sitting on circle with radius
 */
void Polygon::Create2DStar(const AABB &aabb,
                           const float radius,
                           const float innerRadius,
                           const unsigned int circleSegments) {
    ClearVertices();
	
	Vec3 center = aabb.Center();
	
	const float k_increment = (2.0f * hd_pi) / circleSegments;
	float theta = 0.0f;
	
	this->AddVertex(center);
	for (unsigned int i = 0; i < circleSegments; ++i) {
		Vec3 v = center + radius * Vec3(sinf(theta), cosf(theta), 0.0f);
		
		Vec3 inner = center + innerRadius *
            Vec3(sinf(theta+(0.5f * k_increment)),
                 cosf(theta+(0.5f * k_increment)), 0.0f);
		
		this->AddVertex(v);
		this->AddVertex(inner);
		theta += k_increment;
	}
	Vec3 endVertex = center + radius * Vec3(sinf(0.0), cosf(0.0), 0.0f);
	this->AddVertex(endVertex);

	this->ResetTextureCoords();
    this->ResetNormals();
}


void Polygon::CreateCube(const float size, const Vec3 &center) {
    CreateCube(size, size, size, center);
}


void Polygon::CreateCube(const float width, const float height, const float depth, const Vec3 &center) {
    ClearVertices();

    float w = width * 0.5f;
    float h = height * 0.5f;
    float d = depth * 0.5f;

    // front
    this->AddVertex( w, -h,  d, center);
    this->AddVertex( w,  h,  d, center);
    this->AddVertex(-w,  h,  d, center);
    this->AddVertex(-w, -h,  d, center);
    
    // back
    this->AddVertex(-w, -h, -d, center);
    this->AddVertex(-w,  h, -d, center);
    this->AddVertex( w,  h, -d, center);
    this->AddVertex( w, -h, -d, center);
    
    // left
    this->AddVertex(-w, -h,  d, center);
    this->AddVertex(-w,  h,  d, center);
    this->AddVertex(-w,  h, -d, center);
    this->AddVertex(-w, -h, -d, center);
    
    // right
    this->AddVertex( w, -h, -d, center);
    this->AddVertex( w,  h, -d, center);
    this->AddVertex( w,  h,  d, center);
    this->AddVertex( w, -h,  d, center);
    
    // top
    this->AddVertex(-w,  h, -d, center);
    this->AddVertex(-w,  h,  d, center);
    this->AddVertex( w,  h,  d, center);
    this->AddVertex( w,  h, -d, center);
    
    // bottom
    this->AddVertex( w, -h, -d, center);
    this->AddVertex( w, -h,  d, center);
    this->AddVertex(-w, -h,  d, center);
    this->AddVertex(-w, -h, -d, center);
	
    this->ResetTextureCoords();
    this->ResetNormals();
}


void Polygon::CreateSphere(const float radius, const float segments) {
    ClearVertices();
    
    // Create a cylinder, then rotate entire cylinder around origin at increments.
    
    const float TEXTURE_REPEAT = 1.0f;
    
	const uint32_t CIRCLE_SEGMENTS = 16;
	const uint32_t SPHERE_SEGMENTS = 32;
    
    const float circle_increment = hd_pi / CIRCLE_SEGMENTS;
    const float sphere_increment = hd_2_pi / SPHERE_SEGMENTS;
    
    const float circle_wrap_increment = TEXTURE_REPEAT / CIRCLE_SEGMENTS;
    const float sphere_wrap_increment = TEXTURE_REPEAT / SPHERE_SEGMENTS;
	
	float sphere_theta = 0.0f;
    float circle_theta = -hd_half_pi;

    float sphere_texture_wrap = 0.0f;
    
    Matrix rot1, rot2;
    
	for (int i = 0; i < SPHERE_SEGMENTS; ++i) {
        
        // rotate around y axis
        Matrix::RotationXYZOrigin(rot1, Vec3(0.0f, sphere_theta, 0.0f));
        Matrix::RotationXYZOrigin(rot2, Vec3(0.0f, sphere_theta + sphere_increment, 0.0f));
        
        circle_theta = -hd_half_pi;
        float circle_texture_wrap = 0.0f;
        
		for (uint32_t j = 0; j < CIRCLE_SEGMENTS; ++j) {
            Vertex v1, v2, v3, v4, v;
            v.origin = radius * Vec3(cosf(circle_theta), sinf(circle_theta), 0);
            Matrix::Vec3Multiply(v1.origin, v.origin, rot1);
            Matrix::Vec3Multiply(v2.origin, v.origin, rot2);

            v.origin = radius * Vec3(cosf(circle_theta + circle_increment), sinf(circle_theta + circle_increment), 0);
            Matrix::Vec3Multiply(v4.origin, v.origin, rot1);
            Matrix::Vec3Multiply(v3.origin, v.origin, rot2);
            
            v1.texCoord.Set(1.0f - sphere_texture_wrap, TEXTURE_REPEAT - circle_texture_wrap);
            v2.texCoord.Set(1.0f - (sphere_texture_wrap + sphere_wrap_increment), TEXTURE_REPEAT - circle_texture_wrap);
            v3.texCoord.Set(1.0f - (sphere_texture_wrap + sphere_wrap_increment), TEXTURE_REPEAT - (circle_texture_wrap + circle_wrap_increment));
            v4.texCoord.Set(1.0f - sphere_texture_wrap, TEXTURE_REPEAT - (circle_texture_wrap + circle_wrap_increment));
            
            this->AddVertex(v1);
            this->AddVertex(v2);
            this->AddVertex(v3);
            this->AddVertex(v4);
            
            circle_texture_wrap += circle_wrap_increment;
            circle_theta += circle_increment;
        }
        
        sphere_texture_wrap += sphere_wrap_increment;
        sphere_theta += sphere_increment;
	}
    this->ResetNormals();
    assert(this->GetVertices()->GetSize() == 4 * CIRCLE_SEGMENTS * SPHERE_SEGMENTS);
}


float Polygon::GetTextureRepeatX() const {
    return m_texRepeatX;
}


float Polygon::GetTextureRepeatY() const {
    return m_texRepeatY;
}


void Polygon::SetTextureRepeatX(const float repeatX) {
    m_texRepeatX = repeatX;
}


void Polygon::SetTextureRepeatY(const float repeatY) {
    m_texRepeatY = repeatY;
}


void Polygon::ResetNormals() {
    ResetAABB();
    
    if (m_vertices->Size() % 4 == 0) {
        /*
         The cross product of two sides of the triangle equals the surface normal. So,
         if V = P2 - P1 and W = P3 - P1, and N is the surface normal, then:
         
         Nx=(Vy∗Wz)−(Vz∗Wy)
         Ny=(Vz∗Wx)−(Vx∗Wz)
         Nz=(Vx∗Wy)−(Vy∗Wx)
         */
		for (uint32_t i = 0; i < m_vertices->Size(); i += 4) {
            Vertex *v1 = m_vertices->Item(i);
            Vertex *v2 = m_vertices->Item((i+1) % m_vertices->Size());
            Vertex *v3 = m_vertices->Item((i+2) % m_vertices->Size());
            Vertex *v4 = m_vertices->Item((i+3) % m_vertices->Size());
            
            Vec3 v = v2->origin - v1->origin;
            Vec3 w = v3->origin - v1->origin;
            
            Vec3 normal = hd::cross(v, w);
            
            v1->normal = hd::normalize(normal);
            v2->normal = hd::normalize(normal);
            v3->normal = hd::normalize(normal);
            v4->normal = hd::normalize(normal);
        }
    } else {
        Vec3 center = GetAABB().Center();
		for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
            Vertex *vertex = m_vertices->Item(i);
            vertex->normal = hd::normalize(vertex->origin - center);
        }
    }
}


/**
 * Scale the texture to the longest side of the aabb, but clip it to the
 * aabb (unstretched).
 */
void Polygon::ResetTextureCoords() {
    ResetAABB();

	Vec3 box = m_aabb.upper - m_aabb.lower;
	if (box[0] == 0.0 || box[1] == 0.0) return;
	
	float longest = hd::max(box[0], box[1]);
        
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        
		float x = m_texRepeatX * ((vertex->origin[0] - m_aabb.lower[0])/longest);
		float y = m_texRepeatY * (1.0f - ((vertex->origin[1] - m_aabb.lower[1])/longest));
		
        vertex->texCoord.Set(x, y);
	}
}


void Polygon::ResetTextureCoords(const Size2 &size) {
    ResetAABB();

	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        
		float x = m_texRepeatX * ((vertex->origin[0] - m_aabb.lower[0])/size[0]);
		float y = m_texRepeatY * ((vertex->origin[1] - m_aabb.lower[1])/size[1]);
		
        vertex->texCoord.Set(x, y);
	}
}


void Polygon::InvertNormals() {
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        vertex->normal = - vertex->normal;
    }
}


AABB Polygon::GetAABB() const {
    return m_aabb;
}


const hd::Vector<Vertex> * Polygon::GetVertices() const {
    return m_vertices;
}


void Polygon::ResetAABB() {
    if (m_vertices->Size() == 0) return;
    
    Vertex v0 = m_vertices->At(0);
    m_aabb.lower = v0.origin;
    m_aabb.upper = v0.origin;
    
    for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex vertex = m_vertices->At(i);
        m_aabb.lower = hd::min(m_aabb.lower, vertex.origin);
        m_aabb.upper = hd::max(m_aabb.upper, vertex.origin);
    }
}
