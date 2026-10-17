#ifndef _VAST_GRAPHICS_MESH_PROTOTYPE_H_
#define _VAST_GRAPHICS_MESH_PROTOTYPE_H_

#include <Vast/VastMath.h>

namespace hd {
    class Texture;
    
    namespace graphics {

        enum class MeshMethod {
            NullMethod,
            Cube,
            SimpleCube,
            Cylinder,
            Font,
            Sphere,
            Quad,
            RepeatingGrid,
            BoxWithCurvedCorners,
            File,
            GapGrid,
            InverseSphere,
            CurvedQuad,
        };

        typedef struct MeshPrototype {
            MeshPrototype() :
                meshMethod(MeshMethod::NullMethod),
                cylinderRadius(0),
                cylinderLength(0),
                cylinderSegments(0),
                quadAABB(),
                cubeAABB(),
                sphereRadius(0),
                sphereSegments(0),
                simpleCubeSize(),
                repeatingGridAABB(),
                repeatingGridCubeSize(),
                repeatingGridGapSize(),
                boxWithCurvedCornersSize(),
                boxWithCurvedCornersCornerRadius(),
                meshFilePath(),
                gapGridSize(),
                gapGridNodeSize(),
                curvedQuadArc(0.0f),
                curvedQuadRadius(0.0f),
                curvedQuadLength(0.0f),
                curvedQuadSegments(0)
            {}

            MeshMethod meshMethod;

            float cylinderRadius;

            float cylinderLength;

            float cylinderSegments;

            AABB quadAABB;

            AABB cubeAABB;

            float sphereRadius;

            float sphereSegments;

			Size3f simpleCubeSize;

            AABB repeatingGridAABB;

			Size3f repeatingGridCubeSize;

			Size3f repeatingGridGapSize;
            
			Size3f boxWithCurvedCornersSize;
            
            float boxWithCurvedCornersCornerRadius;
            
            std::string meshFilePath;

			Size3f gapGridSize;

			Size3f gapGridNodeSize;

            float curvedQuadArc;

            float curvedQuadRadius;

            float curvedQuadLength;

            uint32_t curvedQuadSegments;

        } MeshPrototype;


        inline bool operator == (const MeshPrototype &a, const MeshPrototype &b) {
            return a.meshMethod == b.meshMethod
                && a.meshFilePath == b.meshFilePath
                && a.cylinderRadius == b.cylinderRadius
                && a.cylinderLength == b.cylinderLength
                && a.cylinderSegments == b.cylinderSegments
                && a.quadAABB == b.quadAABB
                && a.cubeAABB == b.cubeAABB
                && a.sphereRadius == b.sphereRadius
                && a.sphereSegments == b.sphereSegments
                && a.simpleCubeSize == b.simpleCubeSize
                && a.repeatingGridAABB == b.repeatingGridAABB
                && a.repeatingGridCubeSize == b.repeatingGridCubeSize
                && a.repeatingGridGapSize == b.repeatingGridGapSize
                && a.boxWithCurvedCornersSize == b.boxWithCurvedCornersSize
                && a.boxWithCurvedCornersCornerRadius == b.boxWithCurvedCornersCornerRadius
                && a.gapGridSize == b.gapGridSize
                && a.gapGridNodeSize == b.gapGridNodeSize
                && a.curvedQuadArc == b.curvedQuadArc
                && a.curvedQuadRadius == b.curvedQuadRadius
                && a.curvedQuadLength == b.curvedQuadLength
                && a.curvedQuadSegments == b.curvedQuadSegments;
        }
    }
}



#endif