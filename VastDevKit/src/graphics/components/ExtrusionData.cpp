#include "ExtrusionData.h"
#include "graphics/components/Polygon.h"

using namespace hd::graphics;

ExtrusionData::ExtrusionData(unsigned int ID) : Component(ID),
    m_depth(1.0f),
    m_reflect(false),
    m_textureCoords(NULL) {
	m_textureCoords = new hd::Vector<Vec2>(256);
}


void ExtrusionData::Reset() {
    m_textureCoords->Clear();
}


float ExtrusionData::GetDepth() const {
    return m_depth;
}


void ExtrusionData::SetDepth(float depth) {
    m_depth = depth;
}


void ExtrusionData::GenerateTextureCoords(const Polygon *polygon) {
	float multiplier;
    const Vector<Vertex> *vertices = polygon->GetVertices();
    Vec2 t1, t2, t3, t4;
        
	if (vertices->Size() > 4) {
        AABB aabb = polygon->GetAABB();
		Vec3 diff = aabb.upper - aabb.lower;
		float perim = 0.33f * ((diff[0]) + (diff[0]));
		multiplier = fabs(perim/(m_depth*2));
	} else
	{
		multiplier = vertices->Size() * polygon->GetTextureRepeatX();
	}
	
	for (int i = 0; i < vertices->Size(); ++i)
	{
		int i1 = i;
		int i2 = ((i+1) >= vertices->Size()) ? 0 : (i+1);
        
        Vertex v1 = vertices->At(i1);
        Vertex v2 = vertices->At(i2);
        
		if (v1.texCoord[1] == v2.texCoord[1])
		{
			t1.Set(v1.texCoord[0], 0.0f);
			t2.Set(v1.texCoord[0], 1.0f);
			t3.Set(v2.texCoord[0], 1.0f);
			t4.Set(v2.texCoord[0], 0.0f);
		}
		else if (v1.texCoord[0] == v2.texCoord[0])
		{
			t1.Set(0.0f, v1.texCoord[1]);
			t2.Set(1.0f, v1.texCoord[1]);
			t3.Set(1.0f, v2.texCoord[1]);
			t4.Set(0.0f, v2.texCoord[1]);
		}
		else
		{
			float texCoord1 = multiplier * float(i1)   / float(vertices->Size());
			float texCoord2 = multiplier * float(i1+1) / float(vertices->Size());
			
			t1.Set(0.0f, texCoord1);
			t2.Set(1.0f, texCoord1);
			t3.Set(1.0f, texCoord2);
			t4.Set(0.0f, texCoord2);
		}
        
        m_textureCoords->Add(t1);
        m_textureCoords->Add(t2);
        m_textureCoords->Add(t3);
        m_textureCoords->Add(t4);
	}
}


const hd::Vector<Vec2> * ExtrusionData::GetTextureCoords() const
{
    return m_textureCoords;
}
