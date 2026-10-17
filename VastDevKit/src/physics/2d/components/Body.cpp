#include "Body.h"
#include "physics/2d/BodyInfo.h"
#include "physics/2d/components/World.h"
#include <Box2D/Box2D.h>

using namespace hd;

const char * hd::physics2d::BodyUpdatedMessage = "hd::physics2d::BodyUpdatedMessage";

physics2d::Body::Body(unsigned int ID) : Component(ID),
    m_physicsBody(NULL)
{
}


void physics2d::Body::Reset()
{
    m_physicsBody = NULL;
}

void physics2d::Body::CreatePhysicsBody(physics2d::World *world, physics2d::BodyInfo bodyInfo)
{
    if (m_physicsBody)
    {
        // TODO: destroy.
    }
    
    m_physicsBody = world->CreateBody(bodyInfo);
    m_isStatic = bodyInfo.isStatic;
    hdAssert(m_physicsBody);
}


const b2Body * physics2d::Body::GetPhysicsBody() const
{
    return m_physicsBody;
}


Vec3 physics2d::Body::GetPosition() const
{
    if (m_physicsBody)
    {
        b2Transform transform = m_physicsBody->GetTransform();
        return Vec3(transform.p.x * PHYSICS_SIZE_FACTOR,
                    transform.p.y * PHYSICS_SIZE_FACTOR, 0.0f);
    }
    return Vec3();
}


float physics2d::Body::GetAngle() const
{
    if (m_physicsBody)
    {
        return m_physicsBody->GetAngle();
    }
    return 0.0f;
}
