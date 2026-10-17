/*
 *  hdUIImage.cpp
 *  hdk
 *
 *  Created by david on 27/05/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#include "hdUIImage.h"

hdUIImage::hdUIImage(const char* texture, 
					 hdGameWorld* gameWorld) : hdUIReceiver(gameWorld)
{
	m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
	this->SetAs2DBox(hdVec3(0, 0, 0), hdVec3(1.0f, 1.0f, 1.0f));
	m_tint.Set(1.0f, 1.0f, 1.0f, 1.0f);
}


hdUIImage::hdUIImage(const char* texture, 
				   hdGameWorld* gameWorld, 
				   const hdAABB& aabb) : hdUIReceiver(gameWorld)
{
	m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
	this->SetAs2DBox(aabb.lower, aabb.upper);
	
	m_tint.Set(1.0f, 1.0f, 1.0f, 1.0f);
}


hdUIImage::hdUIImage(const char* texture,
				   hdGameWorld* gameWorld, 
				   const float x, const float y, 
				   const float w, const float h) : hdUIReceiver(gameWorld)
{
	m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
	this->SetAs2DBox(hdVec3(x,y,0.0f), hdVec3(x+w,y+h,0.0f));
	
	m_tint.Set(1.0f, 1.0f, 1.0f, 1.0f);
}


hdUIImage::~hdUIImage() 
{
}


bool hdUIImage::MouseDown(float x, float y)
{	
	return false;
	//if (this->IsHidden()) return false;
	///return (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)));
}


// If we did not click on the item, then state can not go into mouse over.
bool hdUIImage::MouseOver(float x, float y)
{
	return false;
	//if (this->IsHidden()) return false;
	//return (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)));
}


bool hdUIImage::MouseUp(float x, float y)
{
	return false;
	/*
	if (this->IsHidden()) return false;
	
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{	
		return true;
	}
	else
	{
		return false;
	}*/
}


void hdUIImage::Draw() const
{
	if (this->IsHidden()) return;
	DrawVerticesWithTint(((hdUIReceiver*)this)->GetVertices(), 
						 ((hdUIReceiver*)this)->GetTextureCoordinates(), 
						 ((hdUIReceiver*)this)->GetVertexCount(), m_aabb, m_texture, m_tint, m_alpha);
}

