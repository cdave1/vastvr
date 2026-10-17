/*
 *  hdUIImage.h
 *  hdk
 *
 *  Created by david on 27/05/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#ifndef HD_UI_IMAGE_H_
#define HD_UI_IMAGE_H_

#include "UI/hdUIReceiver.h"
#include "hdCollections.h"


class hdUIImage : public hdUIReceiver
{
public:
	hdUIImage() {} 
	
	hdUIImage(const char* texture,  
			  hdGameWorld* gameWorld); 
	
	
	hdUIImage(const char* texture,  
			 hdGameWorld* gameWorld, 
			 const hdAABB& aabb);
	
	hdUIImage(const char* texture,
			 hdGameWorld* gameWorld, 
			 const float x, const float y, 
			 const float w, const float h);
	
	~hdUIImage();
	
	bool MouseDown(float x, float y);
	
	bool MouseOver(float x, float y);
	
	bool MouseUp(float x, float y);
	
	void Draw() const;

	const hdTexture* GetTexture() const;
	
	void SetTexture(const hdTexture *texture);
	
	void SetTint(float r, float g, float b, float a);
	
	const hdColor4f GetTint();
	
private:
	hdTexture* m_texture;
	
	hdColor4f m_tint;
	
};


inline const hdTexture* hdUIImage::GetTexture() const
{
	return m_texture;
}


inline void hdUIImage::SetTexture(const hdTexture *texture)
{
	m_texture = (hdTexture *)texture;
}


inline void hdUIImage::SetTint(float r, float g, float b, float a)
{
	m_tint.Set(r,g,b,a);
}


inline const hdColor4f hdUIImage::GetTint()
{
	return m_tint;
}



#endif