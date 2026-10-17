/*
 *  HDIBImage.cpp
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "hdIBImage.h"
#include "hdUIImage.h"

hdIBImage::hdIBImage() : hdIBProxy() 
{
	m_interfaceElement = NULL;
}


hdIBImage::hdIBImage(const char *texture, 
					 const float x, const float y, 
					 const float w, const float h) : hdIBProxy()
{
	this->SetTexture(texture);
	this->SetAs2DBox(hdVec3(x,y,0.0f), hdVec3(x+w,y+h,0.0f));
	m_tint.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_interfaceElement = NULL;
}


hdIBImage::hdIBImage(const char *texture, 
					 const hdVec3 &lower, const hdVec3 &upper) : hdIBProxy()
{
	this->SetTexture(texture);
	this->SetAs2DBox(lower, upper);
	m_tint.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_interfaceElement = NULL;
}


hdIBImage::~hdIBImage()
{
}


hdUIReceiver * hdIBImage::CreateInterfaceElement()
{
	this->ResetAABB();
	if (!m_interfaceElement)
	{
		m_interfaceElement = new hdUIImage(m_texture, NULL);  //, this->GetAABB());
		m_interfaceElement->RemoveAllPoints();
	
		for (uint32 i = 0; i < this->GetVertexCount(); ++i)
		{
			m_interfaceElement->AddPoint(this->GetVertices()[i].x, 
										 this->GetVertices()[i].y,
										 m_zOffset);
			m_interfaceElement->AddTextureCoord(this->GetTextureCoordinates()[i].x, 
												this->GetTextureCoordinates()[i].y);
		}
		
		((hdGameObject *)m_interfaceElement)->ResetAABB();
		//((hdUIImage *)m_interfaceElement)->ResetTextureCoords();
	}
	return m_interfaceElement;
}


void hdIBImage::SetTint(const hdColor4f& tint)
{
	m_tint.Set(tint);
	if (m_interfaceElement)
	{
		((hdUIImage *)m_interfaceElement)->SetTint(m_tint.r, m_tint.g, m_tint.b, m_tint.a);
	}
}


void hdIBImage::Draw() const
{
	if (m_interfaceElement)
		m_interfaceElement->Draw();
}