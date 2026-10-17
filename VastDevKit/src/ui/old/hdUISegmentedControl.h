/*
 *  hdUISegmentedControl.h
 *  hdk
 *
 *  Created by david on 9/10/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#ifndef HD_UI_SEGMENT_CONTROL
#define HD_UI_SEGMENT_CONTROL

#include "UI/hdUIReceiver.h"
#include "UI/hdUIButton.h"
#include "UI/hdUIImage.h"

typedef struct
{
	char text[64];
	
	int value;
	
} ui_segmentdef_t;


typedef struct
{
	hdTexture *imgNormal;
	
	hdTexture *imgOver;
	
	hdTexture *imgUp;
	
	hdTexture *imgSelected;
	
	hdUIButton *button;
	
	ui_segmentdef_t def;
	
} ui_segmentcomponent_t;


// separator
// left
//	- up/down
// right
//  - up/down
// Canonical texture 
//  - up/down


// Font as name



// List of buttons

class hdUISegmentedControl  : public hdUIReceiver
{
public:
	
	/*
	 * From aabb lower.x, draw:
	 * left edge
	 * -	buttons, with separator in between.
	 *
	 * right edge
	 */
	int AddSegment(const ui_segmentdef_t& segment);
	
	bool RemoveSegment(const int index);
	
	void Draw() const;
	
	const int GetValue(void) const;
	
	void AddValueChangedListener(const void *obj, void (*func)(void *, void *));
	
private:
	hdUIImage *m_imgSeparator;
	
	hdUIImage *m_imgLeftEdgeNormal, *m_imgLeftEdgeOver, *m_imgLeftEdgeUp, *m_imgLeftEdgeSelected;
	
	hdUIImage *m_imgRightEdgeNormal, *m_imgRightEdgeOver, *m_imgRightEdgeUp, *m_imgRightEdgeSelected;
	
	hdUIImage *m_imgButtonNormal, *m_imgButtonOver, *m_imgButtonUp, *m_imgButtonSelected;
	
	// list of buttons
	hdTypedefList<ui_segmentcomponent_t, 64> m_buttons;
	
	// currently selected segement
	int m_value; 
	
	// Funcs
	void (*m_valueChangedCallback)(void *, void *);
	
	void *m_callbackObject;
};

#endif