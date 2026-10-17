#ifndef _VAST_BUILDER_OCTREE_H_
#define _VAST_BUILDER_OCTREE_H_

#include "core/component/Component.h"
#include "core/component/ComponentPool.h"

namespace hd {
    template<class T> class ComponentPool;
}

///
/// An Octree is a 3 dimensional sparse tree.  The root of the tree is a volume, in world space,
/// aligned to the axis, with fixed width and height.  Children of each node are defined as
/// sub-divisions of the parent space with the same orientation and width:height ratio.  In three
/// dimensions, that absolute minimum number of children per node that has the property of the same
/// width:height ratio, but smaller, is 8.  This is where the OcTree gets the name.
///
/// An Octree is used for organising space, because the space can be defined mathematically.  We
/// can map a position to a specific octree subdivision by index, or geometry.
///
/// An Octree translates a ray collision into a specific grid location.
/// The grid location will be defined in world space, so we can get the box dimensions of the
/// specific location without having to store ALL grid locations.
///
///
///
class OcTree : public hd::Component {
private:

    AABB m_fullSize;

    Quaternion m_orientation;

    uint32_t m_depth;


protected:

    friend class hd::ComponentPool<OcTree>;

    OcTree(unsigned int ID);


public:

    

};

#endif