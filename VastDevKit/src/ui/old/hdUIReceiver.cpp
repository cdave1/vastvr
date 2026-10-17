/*
 *  hdUIReceiver.cpp
 *  hdk
 *
 *  Created by david on 23/05/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */


#include "hdUIReceiver.h"
#include "hdUIContext.h"

hdUIReceiver::hdUIReceiver()
{
	m_isEnabled = true;
	m_canRotate = true;
	
	Init(false);
}

hdUIReceiver::hdUIReceiver(hdGameWorld* gameWorld) : hdPolygon(gameWorld)
{
	m_isEnabled = true;
	m_canRotate = true;
	
	Init(false);
}


hdUIReceiver::hdUIReceiver(hdGameWorld *gameWorld, bool addToContext) : hdPolygon(gameWorld)
{
	m_isEnabled = true;
	m_canRotate = true;
	Init(addToContext);
}


void hdUIReceiver::Init(bool addToContext)
{
	if (addToContext)
	{
		hdUIContext::AddReceiver(this);
	}
}


hdUIReceiver::~hdUIReceiver()
{
    hdUIContext::RemoveReceiver(this);
}


void hdUIReceiver::RotateAndTranslate2(const hdVec3& rotVector, const hdVec3& rotationAxis, 
									 const hdVec3& transVector, const hdVec3& transVectorAxis)
{
	if (m_canRotate)
	{
		// 
		//hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), transVector - transVectorAxis);
		
		// rotate around rotationAxis
		
		hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -rotationAxis);
		hdRotateVertices(this->GetVertices(), this->GetVertexCount(), rotVector - m_obb.transform.rotation);
		hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), rotationAxis);
		
		
		
		m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);
		m_obb.transform.rotation.Set(rotVector.x, rotVector.y, rotVector.z);
	}
	else
	{
		hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), transVector - transVectorAxis);
		
		m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);		
	}
	this->ResetAABB();
}


// This function is broken - rotation needs to happen around some axis; if
// we assume this is the transVector, then it works okay, but if it isn't then
// the object will end up AT transvector.
void hdUIReceiver::RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector)
{
	
	//hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -(this->GetWorldCenter()));
	//hdRotateVertices(this->GetVertices(), this->GetVertexCount(), rotVector - m_obb.transform.rotation);
	//hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), (this->GetWorldCenter()) + (transVector - (this->GetWorldCenter())));
	
	if (m_canRotate)
	{
		// rotate around transvector
		
		hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -m_obb.transform.translation);
		hdRotateVertices(this->GetVertices(), this->GetVertexCount(), rotVector - m_obb.transform.rotation);
		hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), m_obb.transform.translation + (transVector - m_obb.transform.translation));
		
		m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);
		m_obb.transform.rotation.Set(rotVector.x, rotVector.y, rotVector.z);
	}
	else
	{
		hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -m_obb.transform.translation);
		hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), m_obb.transform.translation + (transVector - m_obb.transform.translation));
		
		m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);		
	}
	this->ResetAABB();
}


bool hdUIReceiver::MouseDoubleClick(float x, float y)
{
	return false;
}


bool hdUIReceiver::MouseMultiClick(float x, float y, unsigned int clicks)
{
	return false;
}
