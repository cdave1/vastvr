#ifndef _VAST_GRAPHICS_MESHLOADER_H_
#define _VAST_GRAPHICS_MESHLOADER_H_

#include <string>
#include "core/collections/Collections.h"
#include "MeshPrototype.h"

namespace hd {
    namespace ui {
        class Font;
        class FontLayout;
    }

    namespace graphics {
        class Mesh;
        class MeshData;


        class MeshLoader {
        private:

            hd::Vector<MeshData *> *m_meshSets;

        public:

            static MeshLoader * Instance();

            ///
            /// Public functions create a mesh prototype.  The prototype is sent to a distribution function
            /// which calls an internal creation function using the MeshPrototype method.
            /// The distribution function uses the MeshPrototype to check for already created meshes with an identical
            /// prototype -- we return the precreated mesh if it is available.
            ///
            /// Once the MeshData has been created, we generate vertex buffers for each of the meshes within the
            /// MeshData structure.
            ///

            hd::graphics::MeshData * GetFontMesh(const hd::ui::Font *font,
                                                 const std::string text);

            hd::graphics::MeshData * GetFontMesh(const hd::ui::Font *font,
                                                 const hd::ui::FontLayout *layout,
                                                 const std::string text);

            hd::graphics::MeshData * GetMeshFromFile(const std::string &path);

            hd::graphics::MeshData * GetCylinder(const float radius,
                                                 const float length,
                                                 const int segments);

            hd::graphics::MeshData * GetQuad(const Vec3 &quad);

            hd::graphics::MeshData * GetQuad(const AABB &aabb);

            hd::graphics::MeshData * GetCube(const Size3f &cube);

            hd::graphics::MeshData * GetCube(const AABB &aabb);

            hd::graphics::MeshData * GetSphere(const float radius, const float segments);

            hd::graphics::MeshData * GetInverseSphere(const float radius, const float segments);

            hd::graphics::MeshData * GetSimpleCube(const Size3f &cube);

            hd::graphics::MeshData * GetGapGrid(const Size3f &size, const Size3f &gap);

            hd::graphics::MeshData * GetRepeatingGrid(const AABB &aabb, const Size3f &cube, const Size3f &gap);

            hd::graphics::MeshData * GetBoxWithCurvedCorners(const Size3f &size,
                                                             const float cornerRadius);

            hd::graphics::MeshData * GetCurvedQuad(const float curvedQuadArc,
                                                   const float curvedQuadRadius,
                                                   const float curvedQuadLength,
                                                   const uint32_t curvedQuadSegments);

            hd::graphics::MeshData * GetMesh(const MeshPrototype &prototype);

            bool DestroyMeshData(hd::graphics::MeshData *meshData);

        private:

            MeshLoader();

            ~MeshLoader();

            hd::graphics::MeshData * FromMeshPrototype(const MeshPrototype &prototype);

            hd::graphics::MeshData * FontMesh(const hd::ui::Font *font,
                                              const std::string text);

            hd::graphics::MeshData * FontMesh(const hd::ui::Font *font,
                                              const hd::ui::FontLayout *layout,
                                              const std::string text);

            hd::graphics::MeshData * MeshFile(const std::string &path);

            hd::graphics::MeshData * Cylinder(const float radius,
                                              const float length,
                                              const int segments);

            hd::graphics::MeshData * Quad(const AABB &aabb);

            hd::graphics::MeshData * Cube(const AABB &aabb);

            hd::graphics::MeshData * Sphere(const float radius, const float segments);

            hd::graphics::MeshData * IcoSphere(const float radius);

            hd::graphics::MeshData * SimpleCube(const Size3f &cube);

            hd::graphics::MeshData * GapGrid(const Size3f &size, const Size3f &gap);

            void AddGapGridNode(hd::graphics::Mesh *grid, const Size3f &gap, const Vec3 &center, uint32_t *idx);

            hd::graphics::MeshData * RepeatingGrid(const AABB &aabb, const Size3f &cube, const Size3f &gap);

            hd::graphics::MeshData * BoxWithCurvedCorners(const Size3f &size,
                                                          const float cornerRadius);

            hd::graphics::MeshData * CurvedQuad(const float curvedQuadArc,
                                                const float curvedQuadRadius,
                                                const float curvedQuadLength,
                                                const uint32_t curvedQuadSegments);
            
            hd::graphics::MeshData * LoadFromObj(const std::string &path);
            
            hd::graphics::MeshData * LoadFromJson(const std::string &path);
            
        };
    }
}

#endif
