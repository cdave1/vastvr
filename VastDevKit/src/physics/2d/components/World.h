#ifndef _VAST_PHYSICS_2D_ENTITIES_WORLD_H_
#define _VAST_PHYSICS_2D_ENTITIES_WORLD_H_

#include <vast/Vast.h>
#include "physics/2d/WorldInfo.h"
#include "physics/2d/BodyInfo.h"
#include <Box2D/Box2D.h>

#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd
{
    namespace physics2d
    {
        class BodyInfo;
        class WorldInfo;
        
        class World : public Component
        {
        private:
            
            b2World *m_physicsWorld;
            
            WorldInfo m_worldInfo;
            
            bool m_isEnabled;
            
            
        public:
            
            void CreateWorld(WorldInfo worldInfo);
            
            void Update();
            
            void SetGravityVector(float x, float y);
            
            b2Body * CreateBody(BodyInfo bodyInfo);
            
            b2World * GetPhysicsWorld() const;
            
            
        protected:
            
            friend class ComponentPool<World>;
            
            World(unsigned int ID);
            
            
        private:
            
            b2Body * Rectangle(BodyInfo bodyInfo);
            
            b2Body * Circle(BodyInfo bodyInfo);
            
            
            /**
             * Creates physics body made up of multiple shapes - triangles in a triangle fan configuration - from the vertices given.
             *
             * The given vertices do not need to form a convex hull - can contain caves.
             *
             * Automagically ensures that the triangles are in CCW order.
             */
            //b2Body * MultiShape(BodyInfo bodyInfo);
            
            
            /**
             * Creates physics body made up of triangles in a triangle strip configuration.
             *
             * The given vertices do not need to form a convex hull - can contain caves.
             *
             * Automagically ensures that the triangles are in CCW order.
             */
            //b2Body * TriangleStrip(BodyInfo bodyInfo);
            
            
            /**
             * Physics body made from multiple polygons in a mesh. The polygons are
             * not necessarily contiguous shapes, but nonetheless must be attached to the
             * same physics body.
             *
             * Polygons are assumed to be convex.
             */
            //b2Body * Mesh(BodyInfo bodyInfo);
            
            /*
             * Single shape from vertices means that vertices must be in CCW order and form a convex hull. Returns nulls if
             * not CCW or contains a cave. Also returns a null body if the
             *
             * Out variables are the body and shape resulting from the box2D shape creation.
             */
            //b2Body * SingleShape(BodyInfo bodyInfo);
            
            bool IsCentroidValid(b2PolygonShape *polygon);
            
            bool IsCentroidValid(b2Vec2& c, b2PolygonShape *polygon);
            
            bool IsCentroidValid(b2Vec2& c, b2Vec2 *vertices, unsigned int vertexCount);
            
            bool IsTriangleValid(b2PolygonShape *polygon);
            
            bool IsTriangleValid(b2Vec2 *vertices, unsigned int vertexCount);
            
        };
    }
}

#endif