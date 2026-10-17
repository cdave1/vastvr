/*
 *  hdUICheckbox.h
 *  hdk
 *
 *  Created by david on 20/10/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#ifndef HD_UI_CHECKBOX_H
#define HD_UI_CHECKBOX_H

#include "hdUIButton.h"

class hdUICheckbox : public hdUIReceiver
	{
	public:
		hdUICheckbox(const char *textureOnNormal, 
				 const char *textureOnOver, 
				   const char *textureOffNormal,
				   const char *textureOffOver,
				 hdGameWorld* gameWorld);
		
		~hdUICheckbox();
		
		void SetAs2DBox(const float& x, const float& y, const float& w, const float& h);
		
		void SetAs2DBox(const hdVec3& aa, const hdVec3& bb);
		
		void Translate(const hdVec3& translationVector, const hdVec3& translationAxis);
		
		void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis);
		
		void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis);
		
		void MoveTo(const hdVec3& center);
		
		void MoveBy(const hdVec3& delta);
		
		bool MouseDown(float x, float y);
		
		bool MouseOver(float x, float y);
		
		bool MouseUp(float x, float y);
		
		bool MouseDoubleClick(float x, float y);
		
		void Draw() const;
		
		void Disable();
		
		void Enable();
		
		void Toggle();
		
		bool IsOn() const;
		
		void SetOn();
		
		void SetOff();
		
		void AddValueChangedListener(void *obj, void (*func)(void *, void *));
	
	protected:
		//static void CheckMouseUpCallback(void *handler, void *sender);
		
	
	private:
		hdUIButton *m_onButton;
		
		hdUIButton *m_offButton;
		
		hdUIButton *m_activeButton;
		
		void (*m_valueChangedCallback)(void *, void *);
		
		void *m_callbackObject;
	};


#endif