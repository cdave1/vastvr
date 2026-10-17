#ifndef _VAST_UI_PLANE_LAYOUT_H_
#define _VAST_UI_PLANE_LAYOUT_H_

#include <Vast/VastMath.h>

///
/// Layout function for positioning items on the surface of a Plane.
///
namespace hd {
    class Entity;
    
    namespace graphics {
        class Transform;
    }
    namespace ui {

        class LayoutDataSource {
        private:
            friend class PlaneLayout;
            friend class SphereLayout;

            // hd::Entity entityForItemAtIndex(Size2)   // cached entity at the given index.
            // uint32_t numberOfSections
            // uint32_t numberOfColumnsInSection(SectionIndex)
            // uint32_t numberOfRowsInSection(SectionIndex)
            // Vector<std::string> sectionIndexTitles
            // std::string titleForHeaderInSection(SectionIndex)
            //
            // float heightForItemAtIndex(Size2)
            // float widthForItemAtIndex(Size2)
            // didSelectItemAtIndex(Size2)
            // float
            //
            // Placement delegate function
            // * setTransformForElementAtPosition(Transform, Size2)

            // Get the entity at the given index.
            std::function<hd::Entity *(Size2 index)> m_entityForItemAtIndex;

            std::function<uint32_t()> m_numberOfSections;

            std::function<uint32_t(Size2 index)> m_numberOfItemsInSection;

            std::function<float(Size2 index)> m_heightForItemAtIndex;

            std::function<float(Size2 index)> m_widthForItemAtIndex;

            hd::Entity * GetEntityForItemAtIndex(Size2 index) {
                if (m_entityForItemAtIndex) {
                    return m_entityForItemAtIndex(index);
                } else {
                    return nullptr;
                }
            }

        public:

            void EntityForItemAtIndex(const std::function<hd::Entity *(Size2 index)> &function);

            void NumberOfItems(const std::function<uint32_t()> &function);

            void HeightForItemAtIndex(const std::function<float(Size2 index)> &function);

            void WidthForItemAtIndex(const std::function<float(Size2 index)> &function);





        };


        class LayoutDelegate {
        private:
            friend class PlaneLayout;

            std::function<void(hd::graphics::Transform *transform, Size2 index)> m_setTransformForEntity;
        };


        class PlaneLayout {
        private:

            uint32_t m_rows;

            uint32_t m_columns;

            Vec2 m_nodeSize;

            Plane m_plane;



        public:




            PlaneLayout(const uint32_t rows, const uint32_t columns, Plane plane);

            ///
            /// Determine the world position of an item on the plane at the given grid location.  The offset is in
            /// grid units.
            ///
            void PositionOnPlane(const uint32_t row, const uint32_t col,
                                  Vec3 *_pos, Quaternion *_rotation, Vec2 offset = Vec2()) const;

            void PositionOnPlane(const uint32_t row, const uint32_t col,
                                  hd::graphics::Transform *transform, Vec2 offset = Vec2()) const;

            void PositionOnPlane(const uint32_t row, const uint32_t col,
                                  const hd::Entity *entity, Vec2 offset = Vec2()) const;


            void SetRows(const uint32_t rows) {
                m_rows = rows;
            }


            void SetColumns(const uint32_t columns) {
                m_columns = columns;
            }


            void SetNodeSize(const Vec2 &nodeSize) {
                m_nodeSize = nodeSize;
            }


            void SetPlane(const Plane &plane) {
                m_plane = plane;
            }


            ///
            /// Calculate the approximate size, in world coordinates, of a single grid node.
            ///
            Vec2 GetGridNodeSize(const uint32_t row = 0, const uint32_t col = 0) const;

        };
    }
}



#endif