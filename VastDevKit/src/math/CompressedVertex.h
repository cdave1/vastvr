#ifndef _VAST_MATH_COMPRESSED_VERTEX_H_
#define _VAST_MATH_COMPRESSED_VERTEX_H_

#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vertex.h"
#include "math/Color4f.h"
#include "math/Color4ub.h"

//
// TODO: MOAR COMPRESHUN.
//
struct CompressedVertex {
	CompressedVertex () {
        origin.Set(0, 0, 0);
        color.Setf(1.0f, 1.0f, 1.0f, 1.0f);
        normal.Set(0, 0, 0);
        texCoord.Set(0, 0);
    }

    CompressedVertex(const Vertex &vertex) {
        origin.Set(vertex.origin);
        color.Setf(vertex.color[0], vertex.color[1], vertex.color[2], vertex.color[3]);
        normal.Set(vertex.normal);
        texCoord.Set(vertex.texCoord);
    }
    
    void Reset() {
        origin.Set(0, 0, 0);
        color.Setf(1.0f, 1.0f, 1.0f, 1.0f);
        normal.Set(0, 0, 0);
        texCoord.Set(0, 0);
    }
	
    Vec3 origin;
    Color4ub color;
    Vec3 normal;
    Vec2 texCoord;
};

#endif