/*
 *  hdUICheckbox.cpp
 *  hdk
 *
 *  Created by david on 20/10/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#include "hdUICheckbox.h"

hdUICheckbox::hdUICheckbox(const char *textureOnNormal, 
					   const char *textureOnOver, 
					   const char *textureOffNormal,
					   const char *textureOffOver,
					   hdGameWorld* gameWorld)  : hdUIReceiver(gameWorld, true)
{
	// create two buttons
	
	m_onButton = new hdUIButton(textureOnNormal, 
							  textureOnOver, 
							  textureOnOver);
	
	m_offButton = new hdUIButton(textureOffNormal, 
							   textureOffOver, 
							   textureOffOver);
	
	this->SetOff();
	
	m_callbackObject = NULL;
	m_valueChangedCallback = NULL;
}


hdUICheckbox::~hdUICheckbox()
{
	hdDeleteSafely(m_onButton);
	hdDeleteSafely(m_offButton);
}


void hdUICheckbox::SetAs2DBox(const float& x, const float& y, const float& w, const float& h)
{
	m_onButton->SetAs2DBox(x, y, w, h);
	m_offButton->SetAs2DBox(x, y, w, h);
}


void hdUICheckbox::SetAs2DBox(const hdVec3& aa, const hdVec3& bb)
{
	m_onButton->SetAs2DBox(aa, bb);
	m_offButton->SetAs2DBox(aa, bb);
}


void hdUICheckbox::Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis)
{
	//((hdUIReceiver *)this)->Rotate(rotationVector, rotationAxis);
	m_onButton->Rotate(rotationVector, rotationAxis);
	m_offButton->Rotate(rotationVector, rotationAxis);
}


void hdUICheckbox::Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis)
{
	//((hdUIReceiver *)this)->Scale(scaleVector, scaleAxis);
	m_onButton->Scale(scaleVector, scaleAxis);
	m_offButton->Scale(scaleVector, scaleAxis);
}


void hdUICheckbox::MoveTo(const hdVec3& center)
{
	//((hdUIReceiver *)this)->MoveTo(center);
	//hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), center - this->GetWorldCenter());
	//this->GetTransform().translation = center;
	//this->ResetAABB();
	
	m_onButton->MoveTo(m_onButton->GetWorldCenter() + center);
	m_offButton->MoveTo(m_offButton->GetWorldCenter() + center);
}


void hdUICheckbox::Translate(const hdVec3& translationVector, const hdVec3& translationAxis)
{
	m_onButton->Translate(translationVector, translationAxis);
	m_offButton->Translate(translationVector, translationAxis);
}


void hdUICheckbox::MoveBy(const hdVec3& delta)
{
	m_onButton->MoveBy(delta);
	m_offButton->MoveBy(delta);
}


void hdUICheckbox::AddValueChangedListener(void *obj, void (*func)(void *, void *))
{
	m_callbackObject = obj;
	m_valueChangedCallback = func;
}



bool hdUICheckbox::MouseDown(float x, float y)
{
	if (this->IsDisabled()) return false;
	if (this->IsHidden()) return false;
	
	return m_activeButton->MouseDown(x, y);
}


bool hdUICheckbox::MouseOver(float x, float y)
{
	if (this->IsDisabled()) return false;
	if (this->IsHidden()) return false;
	
	return m_activeButton->MouseOver(x, y);
}


bool hdUICheckbox::MouseUp(float x, float y)
{
	if (this->IsDisabled()) return false;
	if (this->IsHidden()) return false;
	
	bool res = m_activeButton->MouseUp(x, y);
	if (res)
	{
		Toggle();
		if (m_callbackObject != NULL && m_valueChangedCallback != NULL)
		{
			(*m_valueChangedCallback)(m_callbackObject, this); //, (void *)this);
		}
	}
	
	return res;
}

bool hdUICheckbox::MouseDoubleClick(float x, float y)
{
	if (this->IsDisabled()) return false;
	if (this->IsHidden()) return false;
	
	return m_activeButton->MouseDoubleClick(x, y);
}


void hdUICheckbox::Toggle()
{
	if (m_activeButton == m_onButton)
	{
		SetOff();
	} 
	else
	{
		SetOn();
	}
}


void hdUICheckbox::SetOn()
{
	hdAssert(m_onButton && m_offButton);
	m_activeButton = m_onButton;
	m_onButton->Enable();
	m_onButton->ResetButton();
	m_offButton->Disable();
}


void hdUICheckbox::SetOff()
{
	hdAssert(m_onButton && m_offButton);
	m_activeButton = m_offButton;
	m_offButton->Enable();
	m_offButton->ResetButton();
	m_onButton->Disable();
}


void hdUICheckbox::Draw() const
{
	m_activeButton->Draw();
}


void hdUICheckbox::Disable()
{
	m_offButton->Disable();
	m_onButton->Disable();
	m_isEnabled = false;
}


void hdUICheckbox::Enable()
{
	m_offButton->Enable();
	m_onButton->Enable();
	m_isEnabled = true;
}


bool hdUICheckbox::IsOn() const
{
	return (m_activeButton == m_onButton);
}