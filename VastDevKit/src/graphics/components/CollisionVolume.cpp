#include "CollisionVolume.h"

hd::graphics::CollisionVolume::CollisionVolume(unsigned int ID) : Component(ID), m_aabb(), m_nodeSize(), m_nodeCount()  {
}

void hd::graphics::CollisionVolume::SetAABB(const AABB &aabb) {
    m_aabb = aabb;
}


void hd::graphics::CollisionVolume::SetNodeSize(const Vec3 &size) {
    m_nodeSize = size;
}


const Vec3& hd::graphics::CollisionVolume::GetNodeSize() const {
    return m_nodeSize;
}


void hd::graphics::CollisionVolume::SetNodeCount(const Size2 &count) {
    m_nodeCount = count;
}


Size2 hd::graphics::CollisionVolume::GetNodeCount() const {
    return Size2(m_aabb.Width() / m_nodeSize[0], m_aabb.Depth() / m_nodeSize[2]);
}


Vec3 hd::graphics::CollisionVolume::CenterOf(const Size2 &index) {
    Vec3 _center((index[0] * m_nodeSize[0]) + (0.5f * m_nodeSize[0]),
                 0.0f,
                 (index[1] * m_nodeSize[2]) + (0.5f * m_nodeSize[2]));
    _center += m_aabb.lower;
    return _center;
}


uint32_t hd::graphics::CollisionVolume::IndexesOf(const Collision3 &start,
                                                  const Collision3 &end,
                                                  Size2 *indexes,
                                                  Vec3 *centers,
                                                  const uint32_t count) {
    Size2 startIndex, endIndex;
    Vec3 startCenter, endCenter;

    if (this->IndexOf(start, &startIndex, &startCenter) &&
        this->IndexOf(end, &endIndex, &endCenter)) {

        Size2 from = hd::min(startIndex, endIndex);
        Size2 to = hd::max(startIndex, endIndex);

        uint32_t c = 0;
        for (uint32_t i = from[0]; i <= to[0]; ++i) {
			for (uint32_t j = from[1]; j <= to[1]; ++j) {
                if (c < count) {
                    indexes[c] = Size2(i, j);
                    centers[c] = this->CenterOf(indexes[c]);
                    c++;
                } else {
                    return c;
                }
            }
        }
        return c;
    } else {
        return 0;
    }
}


Vec3 hd::graphics::CollisionVolume::GetCollisionCenter(const Collision3 &collision) {
    Vec3 origin = collision.origin + (0.5f * m_nodeSize);
    Vec3 normalized(floor(origin[0] / m_nodeSize[0]),
                    floor(origin[1] / m_nodeSize[1]),
                    floor(origin[2] / m_nodeSize[2]));

    return Vec3(normalized[0] * m_nodeSize[0],
                normalized[1] * m_nodeSize[1],
                normalized[2] * m_nodeSize[2]);
}


Size3 hd::graphics::CollisionVolume::GetCollisionIndex(const Collision3 &collision) {
    Vec3 origin = collision.origin - m_aabb.lower;
    Vec3 normalized(origin[0] / m_aabb.Width(),
                    origin[1] / m_aabb.Height(),
                    origin[2] / m_aabb.Depth());

    Size3 nodeCount(m_aabb.Width() / m_nodeSize[0],
                    m_aabb.Height() / m_nodeSize[1],
                    m_aabb.Depth() / m_nodeSize[2]);
    return Size3(normalized[0] * nodeCount[0],
                 normalized[1] * nodeCount[1],
                 normalized[2] * nodeCount[2]);
}


bool hd::graphics::CollisionVolume::IndexOf(const Collision3 &collision, Size2 *index, Vec3 *center) {
    Vec3 origin = collision.origin - m_aabb.lower;

    if (origin[0] < 0.0f || origin[2] < 0.0f ||
        origin[0] > m_aabb.Width() || origin[2] > m_aabb.Depth()) {
        return false;
    }

    Vec2 normalized(origin[0] / m_aabb.Width(),
                    origin[2] / m_aabb.Depth());

    Size2 nodeCount(m_aabb.Width() / m_nodeSize[0], m_aabb.Depth() / m_nodeSize[2]);
    Size2 _index = Size2(normalized[0] * nodeCount[0], normalized[1] * nodeCount[1]);

    if (index) {
        *index = _index;
    }

    // HACK HACK HACK
    //
    // Y axis is always zero.
    Vec3 _center((_index[0] * m_nodeSize[0]) + (0.5f * m_nodeSize[0]),
                 0.0f,
                 (_index[1] * m_nodeSize[2]) + (0.5f * m_nodeSize[2]));
    _center += m_aabb.lower;

    if (center) {
        *center = _center;
    }

    return true;
}
