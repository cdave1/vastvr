#ifndef _VAST_MATH_VERTEX_H_
#define _VAST_MATH_VERTEX_H_

#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Color4f.h"

struct Vertex {
	Vertex () {
        origin.Set(0, 0, 0);
        color.Set(1.0f, 1.0f, 1.0f, 1.0f);
        normal.Set(0, 0, 0);
        tangent.Set(0, 0, 0);
        texCoord.Set(0, 0);
    }

    Vertex(float x, float y) {
        Reset();
        origin.Set(x, y, 0.0f);
    }

    Vertex(float x, float y, float z) {
        Reset();
        origin.Set(x, y, z);
    }

    Vertex(Vec3 vec) {
        Reset();
        origin.Set(vec);
    }
    
    void Reset() {
        origin.Set(0, 0, 0);
        color.Set(1.0f, 1.0f, 1.0f, 1.0f);
        normal.Set(0, 0, 0);
        tangent.Set(0, 0, 0);
        texCoord.Set(0, 0);
    }
	
    Vec3 origin;
    Color4f color;
    Vec3 normal;
    Vec3 tangent;
    Vec2 texCoord;
};

#endif