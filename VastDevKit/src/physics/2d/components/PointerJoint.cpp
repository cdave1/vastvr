#include "PointerJoint.h"
#include "physics/2d/PointerJointInfo.h"
#include "physics/2d/components/World.h"
#include <Box2D/Box2D.h>

using namespace hd::physics2d;

PointerJoint::PointerJoint(unsigned int ID) : Component(ID),
    m_joint(NULL)
{
}


void PointerJoint::Reset()
{
    m_joint = NULL;
}


void PointerJoint::CreateJoint(hd::physics2d::World *world,
	hd::physics2d::PointerJointInfo jointInfo,
	hd::physics2d::Body *first,
	hd::physics2d::Body *second)
{
    if (m_joint) return;
    
    b2Body *firstBody =  const_cast<b2Body *>(first->GetPhysicsBody());
    b2Body *secondBody = const_cast<b2Body *>(second->GetPhysicsBody());
    
    if (firstBody == NULL || secondBody == NULL)
    {
        hd::log::i("Couldn't create pointer joint\n");
        return;
    }
    
    b2MouseJointDef md;
    md.bodyA = firstBody;
    md.bodyB = secondBody;
    md.collideConnected = jointInfo.collideConnected;
    md.target           = b2Vec2((jointInfo.target[0] + jointInfo.targetOffset[0]) / PHYSICS_SIZE_FACTOR,
                                 (jointInfo.target[1] + jointInfo.targetOffset[1]) / PHYSICS_SIZE_FACTOR);
    md.frequencyHz      = jointInfo.frequencyHz;
    md.dampingRatio     = jointInfo.dampingRatio;
    md.maxForce         = jointInfo.forceMultiplier * secondBody->GetMass();

    secondBody->SetActive(true);
    
    b2World *physicsWorld = world->GetPhysicsWorld();
    b2Joint *joint = physicsWorld->CreateJoint(&md);
    m_joint = static_cast<b2MouseJoint *>(joint);    
}


void PointerJoint::UpdateJoint(const Vec3 &target)
{
    if (m_joint)
    {
        b2Vec2 newTarget(target[0] / PHYSICS_SIZE_FACTOR,
                         target[1] / PHYSICS_SIZE_FACTOR);
        m_joint->SetTarget(newTarget);
    }
}


void PointerJoint::DestroyJoint(hd::physics2d::World *world)
{
    if (m_joint)
    {
        b2World *physicsWorld = world->GetPhysicsWorld();
        physicsWorld->DestroyJoint(m_joint);
        m_joint = NULL;
    }
}


const b2MouseJoint * PointerJoint::GetJoint() const
{
    return m_joint;
}
