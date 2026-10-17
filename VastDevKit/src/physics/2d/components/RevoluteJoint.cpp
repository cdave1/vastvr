#include "Body.h"
#include "physics/2d/BodyInfo.h"
#include "physics/2d/components/World.h"
#include <Box2D/Box2D.h>

using namespace hd::physics2d;

//const char * hd::RevoluteJointUpdatedMessage = "hd::RevoluteJointUpdatedMessage";

RevoluteJoint::RevoluteJoint(unsigned int ID) : Component(ID),
    m_joint(NULL)
{
}


void RevoluteJoint::Reset()
{
    m_joint = NULL;
}


void RevoluteJoint::CreateJoint(hd::physics2d::World *world,
	hd::physics2d::RevoluteJointInfo jointInfo,
	hd::physics2d::Body *first,
	hd::physics2d::Body *second)
{
    b2Body *firstBody =  const_cast<b2Body *>(first->GetPhysicsBody());
    b2Body *secondBody = const_cast<b2Body *>(second->GetPhysicsBody());
    
    if (firstBody == NULL || secondBody == NULL)
    {
        hd::log::i("Couldn't create revolute joint at %f, %f because one of the b2 physics bodies has gone away.",
                 jointInfo.position[0], jointInfo.position[1]);
        return;
    }
    
    b2Vec2 b2RotPoint = b2Vec2(jointInfo.position[0], jointInfo.position[1]);
    b2RevoluteJointDef jointDef;
    
    jointDef.Initialize(firstBody, secondBody, b2RotPoint);
    
    jointDef.enableMotor =  jointInfo.enableMotor;
    jointDef.enableLimit =  jointInfo.enableLimit;
    jointDef.lowerAngle =   jointInfo.lowerAngle;
    jointDef.upperAngle =   jointInfo.upperAngle;
    jointDef.maxMotorTorque = jointInfo.maxMotorTorque;
    jointDef.motorSpeed =   jointInfo.motorSpeed;
    
    b2World *physicsWorld = world->GetPhysicsWorld();
    
    m_joint = (b2RevoluteJoint *)physicsWorld->CreateJoint(&jointDef);
}


const b2Joint * RevoluteJoint::GetJoint() const
{
    return m_joint;
}
