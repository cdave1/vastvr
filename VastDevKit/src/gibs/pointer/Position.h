
/**
 * Spatial properties of a pointer event.
 */

#ifndef _VAST_INPUT_POINTER_POSITION_H_
#define _VAST_INPUT_POINTER_POSITION_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    class Entity;
    
    namespace input {
        class EventQueue;
        
        namespace pointer {
            class Position : public Component {
            private:
                
                Vec3 m_position;
                
                Vec3 m_delta;
                
                AABB m_coordinateSpace;
                
                
            private:
                
                friend class hd::input::EventQueue;
                
                void SetPosition(const Vec3 &position);
                
                void SetDelta(const Vec3 &delta);
                
                void SetCoordinateSpace(const AABB &aabb);
                
                
            protected:
                
                friend class ComponentPool<Position>;
                
                Position(unsigned int ID);
                
                
            public:
                
                const Vec3 & GetPosition() const;
                
                const Vec3 & GetDelta() const;
                
                const AABB & GetCoordinateSpace() const;
                
				Vec3 GetPosition(hd::Entity *camera) const;
                
				Vec3 GetDelta(hd::Entity *camera) const;
                
                void PrintDebugInfo() const;
                
                
            };
        }
    }
}

#endif