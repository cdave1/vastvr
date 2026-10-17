#ifndef _VAST_MATH_TRIANGLE_H_
#define _VAST_MATH_TRIANGLE_H_

#include "math/Vertex.h"

struct Triangle {
	Triangle () {
        vertices[0].Reset();
        vertices[1].Reset();
        vertices[2].Reset();
    }
	
    Vertex vertices[3];
};



#endif