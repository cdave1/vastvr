/*
 *  hdUIContainer.cpp
 *  hdk
 *
 *  Created by david on 23/05/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#include "hdUIContainer.h"

hdUIContainer::hdUIContainer(hdGameWorld* gameWorld) : hdUIReceiver(gameWorld)
{
	this->Init();
}


hdUIContainer::hdUIContainer(hdGameWorld* gameWorld, const hdVec3& lower, const hdVec3& upper) : hdUIReceiver(gameWorld)
{
	m_aabb.lower = lower;
	m_aabb.upper = upper;
	
	this->SetAs2DBox(hdVec3toVec2(lower), hdVec3toVec2(upper));
	
	this->ResetAABB();
	
	this->Init();
}


void hdUIContainer::Init()
{
	//m_polygons = new hdPointerList<hdPolygon, 32>();
	m_receivers = new hdTypedefList<hdUIReceiver*, 32>();
	m_isEnabled = true;
}


hdUIContainer::~hdUIContainer()
{
	//delete m_polygons;
	delete m_receivers;
}


int hdUIContainer::Add(hdUIReceiver *receiver)
{
	return m_receivers->Add(receiver);
}


bool hdUIContainer::Remove(hdUIReceiver *receiver)
{
	m_receivers->Remove(receiver);
	return true;
}


void hdUIContainer::Hide()
{
	for (unsigned i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->Hide();
	}
	((hdGameObject *)this)->Hide();
}
	

void hdUIContainer::Show()
{
	for (unsigned i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->Show();
	}
	((hdGameObject *)this)->Show();
}


void hdUIContainer::Enable()
{
	for (unsigned i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->Enable();
	}
	//((hdUIReceiver *)this)->Enable();
}


void hdUIContainer::Disable()
{
	for (unsigned i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->Disable();
	}
	//((hdUIReceiver *)this)->Disable();
}


void hdUIContainer::Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis)
{
	for (unsigned i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->Scale(scaleVector, scaleAxis);
	}
#if 0
	hdVec3 center = this->GetWorldCenter();
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -(this->GetWorldCenter()));
	hdScaleVertices(this->GetVertices(), this->GetVertexCount(), scaleVector);
	//hdScaleVertices(&center, 1, scaleVector);
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), center);
#else 
	hdVec3 center = this->GetWorldCenter();
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -center);
	hdScaleVertices(this->GetVertices(), this->GetVertexCount(), scaleVector);
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), center);
#endif
	this->ResetAABB();
}


void hdUIContainer::Translate(const hdVec3& translationVector, const hdVec3& translationAxis)
{
	//this->RotateAndTranslate(hdVec3(0,0,0), translationVector+translationAxis);
	for (unsigned i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->Translate(translationVector, translationAxis);
	}
	
	//this->Translate(translationVector, translationAxis);
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), translationVector);
	this->GetTransform().translation = translationAxis + translationVector;
	this->ResetAABB();
}


void hdUIContainer::MoveTo(const hdVec3& center)
{
	hdVec3 translationVector = center - this->GetWorldCenter();
	this->Translate(translationVector, this->GetWorldCenter());
}


void hdUIContainer::RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector)
{
	for (unsigned i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->RotateAndTranslate(rotVector, transVector);
	}
	
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -(this->GetWorldCenter()));
	hdRotateVertices(this->GetVertices(), this->GetVertexCount(), rotVector - m_obb.transform.rotation);
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), (this->GetWorldCenter()) + (transVector - (this->GetWorldCenter())));
	
	m_obb.transform.translation = transVector;
	m_obb.transform.rotation = rotVector;
	this->ResetAABB();
}


// Check whether the touch event was in the bounds of any buttons.
bool hdUIContainer::MouseDown(float x, float y)
{
	if (this->IsHidden()) return false;
	if (this->IsDisabled()) return false;
	
	//for (int i = 0; i < m_receivers->GetItemCount(); ++i)
	//{
	//	if (m_receivers->GetItems()[i]->MouseDown(x, y))
	//	{
	//		return true;
	//	}
	//}
	return false;
}


bool hdUIContainer::MouseOver(float x, float y)
{
	if (this->IsHidden()) return false;
	if (this->IsDisabled()) return false;
	
	//for (int i = 0; i < m_receivers->GetItemCount(); ++i)
	//{
	//	m_receivers->GetItems()[i]->MouseOver(x, y);
	//}
	return false;
}


bool hdUIContainer::MouseUp(float x, float y)
{
	if (this->IsHidden()) return false;
	if (this->IsDisabled()) return false;
	
	//for (int i = 0; i < m_receivers->GetItemCount(); ++i)
	//{
	//	if (m_receivers->GetItems()[i]->MouseUp(rOut, x, y))
	//	{
	//		break;
	//	}
	//}
	return false; //true;
}


void hdUIContainer::Draw() const
{
	if (this->IsHidden()) return;
	
	for (unsigned i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->Draw();
	}
}