/*
 *  hdUIContainer.h
 *  hdk
 *
 *  Created by david on 23/05/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#ifndef HD_INTERFACE_CONTAINER_H_
#define HD_INTERFACE_CONTAINER_H_

#include "UI/hdUIReceiver.h"
#include "hdCollections.h"


class hdUIContainer : public hdUIReceiver
{
public:
	hdUIContainer(hdGameWorld* gameWorld);
	
	hdUIContainer(hdGameWorld* gameWorld,  const hdVec3& lower, const hdVec3& upper);
	
	~hdUIContainer();
	
	int Add(hdUIReceiver *receiver);
	
	bool Remove(hdUIReceiver *receiver);
	
	void MoveTo(const hdVec3& center);
	
	void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis);
	
	void Translate(const hdVec3& translationVector, const hdVec3& translationAxis);
	
	void RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector);
	
	// Check whether the touch event was in the bounds of any buttons.
	bool MouseDown(float x, float y);
	
	bool MouseOver(float x, float y);
	
	bool MouseUp(float x, float y);
	
	void Draw() const;
	
	void Hide();
	
	void Show();
	
	void Enable();
	
	void Disable();

	
	
private:
	void Init();
	//hdPointerList<hdUIImage, 64>* m_images;
	
	//hdPointerList<hdPolygon, 32>* m_polygons;
	
	hdTypedefList<hdUIReceiver*, 32>* m_receivers;
	
	bool m_isEnabled;
	
	void *m_tag;
};



#endif