/*
 *  hdUIEnums.h
 *  hdk
 *
 *  Created by david on 21/09/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#ifndef HD_INTERFACE_ENUMS_H
#define HD_INTERFACE_ENUMS_H

typedef enum
{
	e_hdInterfaceClickStateDown = 0,
	e_hdInterfaceClickStateOver,
	e_hdInterfaceClickStateUp,
} e_hdInterfaceClickState;


typedef enum
{
	e_hdInterfaceOrientationPortrait = 0,
	e_hdInterfaceOrientationPortraitUpsideDown = 1,
	e_hdInterfaceOrientationLandscapeLeft = 2,
	e_hdInterfaceOrientationLandscapeRight = 3,
} e_hdInterfaceOrientation;


typedef enum
{
	e_screenEdgeLeft = 0,
	e_screenEdgeRight,
	e_screenEdgeBottom,
	e_screenEdgeTop,
} e_screenEdge;


#endif