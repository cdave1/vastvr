/*
 *  hdUIThumbPad.cpp
 *  fpsTest
 *
 *  Created by david on 16/11/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#include "hdUIThumbPad.h"



hdUIThumbPad::hdUIThumbPad(const char *bgTexture,
					   const char *padTexture,
					   const float x, const float y, 
					   const float w, const float h) : hdUIReceiver(NULL, true)
{
	m_bgTexture = hdTextureManager::Instance()->FindTexture(bgTexture, TT_16Pic);
	
	SetAs2DBox(hdVec2(x,y), hdVec2(x+w, y+h));
	
	hdVec3 tmp = GetWorldCenter();
	
	m_radius = w;
	
	m_pad = new hdUIImage(padTexture, NULL);
	m_pad->SetAs2DBox(tmp.x - m_radius/3.0f, tmp.y - m_radius/3.0f, 
					  (m_radius/1.5f), (m_radius/1.5f));
	
	m_pad->SetCanRotate(false);
}


hdUIThumbPad::~hdUIThumbPad()
{
	hdDeleteSafely(m_pad);
}


bool hdUIThumbPad::IsMoving()
{
	return (m_state == e_padMoving);
}


bool hdUIThumbPad::MouseDown(float x, float y)
{
	if (this->IsDisabled()) 
	{
		m_state = e_padWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_state = e_padWaiting;
		return false;
	}
	
	if (hdPolygonContainsPoint(GetVertices(), GetVertexCount(), hdVec3(x,y,0)))
	{
		if (m_state == e_padWaiting)
		{
			MoveThumbPadTo(x, y);
			
			m_state = e_padMoving;
			return true;
		}
		else
		{
			return false;
		}
		
	} 
	else 
	{
		m_state = e_padWaiting;
		return false;
	}
}




bool hdUIThumbPad::MouseOver(float x, float y)
{
	if (this->IsDisabled()) 
	{
		m_state = e_padWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_state = e_padWaiting;
		return false;
	}
	
	// If we're moving it means
	// players started on the pad.
	if (m_state == e_padMoving)
	{
		MoveThumbPadTo(x, y);
		
		
		return true;
	}
	else
	{
		m_state = e_padWaiting;
		return false;
	}
}


bool hdUIThumbPad::MouseUp(float x, float y)
{
	
	
	if (this->IsDisabled()) 
	{
		m_state = e_padWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_state = e_padWaiting;
		return false;
	}
	
	if (m_state == e_padMoving)
	{
		MoveThumbPadTo(this->GetWorldCenter().x,
					   this->GetWorldCenter().y);
		
		m_state = e_padWaiting;
		return true;
	}
	else
	{
		m_state = e_padWaiting;
		return false;
	}
}


const hdVec2 hdUIThumbPad::GetPadDirection()
{
	return hdVec2((m_pad->GetWorldCenter().x - this->GetWorldCenter().x)/m_radius,
				  (m_pad->GetWorldCenter().y - this->GetWorldCenter().y)/m_radius);
}


void hdUIThumbPad::MoveThumbPadTo(float x, float y)
{
	hdVec3 diff, tmp, norm;
	hdAABB aabb;
	
	diff.Set(x - this->GetWorldCenter().x,
			 y - this->GetWorldCenter().y,
			 0);
	
	aabb = ((hdUIThumbPad *)this)->GetAABB();
	
	if (hdFastHypotenuse(diff.x, diff.y) > (m_radius * 0.5f))
	{
		norm = hdNormalize3(diff);
		
		tmp.Set(this->GetWorldCenter().x + (norm.x * m_radius * 0.5f), 
				this->GetWorldCenter().y + (norm.y * m_radius * 0.5f), 
				0.0f);
	}
	else
	{
		tmp.Set(x, y, 0);
	}
	
	m_pad->SetAs2DBox(tmp.x - m_radius/3.0f, tmp.y - m_radius/3.0f, 
					  (m_radius/1.5f), (m_radius/1.5f));
}


void hdUIThumbPad::Init()
{
}


void hdUIThumbPad::Draw() const
{
	((hdUIThumbPad *)this)->DrawImpl();
}


void hdUIThumbPad::DrawImpl()
{
	if (IsHidden()) return;
	
	if (IsEnabled())
	{
		if (m_bgTexture)
		{
			DrawVertices(GetVertices(), GetTextureCoordinates(), GetVertexCount(), m_aabb, m_bgTexture);
		}
		m_pad->Draw();
	}
	else
	{
		DrawVerticesWithTint(GetVertices(), 
							 GetTextureCoordinates(), 
							 GetVertexCount(), 
							 m_aabb, 
							 m_bgTexture, 
							 hdColor4f(1.0f, 1.0f, 1.0f, 1.0f), 0.4f);
	}
}