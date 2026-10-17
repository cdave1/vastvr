/*
 *  hdUIContext.h
 *  Smashed
 *
 *  Created by david on 2/11/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#ifndef HD_INTERFACE_CONTEXT_H
#define HD_INTERFACE_CONTEXT_H

#include "hdGame.h"
#include "hdGraphicsLibrary.h"
#include "hdMath.h"
#include "hdCollections.h"
#include "UI/hdUIReceiver.h"
#include "UI/hdUIController.h"
 
/*
 * Retains a list of receivers. Receivers add themselves
 * to this context when they are created.
 *
 * The context dispatches events to each of the receivers.
 *
 * A context exists for the duration of an interface
 * controller, but is not tied to the interface controller.
 *
 * Events are sent via app controller; if there is no context
 * then just do nothing.
 */

class hdUIController;


class hdUIContext
{
public:
	static void Init();
	
	static void TearDown();
	
	static void EnableLogging();
	
	static void DisableLogging();
	
	static bool AddController(const hdUIController *controller);
	
	static bool RemoveController(const hdUIController *controller);
	
	static bool AddReceiver(const hdUIReceiver *receiver);
	
	static bool RemoveReceiver(const hdUIReceiver *receiver);
	
	static void HandleTouchDown(const unsigned int touchId, 
							  float x, float y, int tapCount);
	
	static void HandleTouchMoved(const unsigned int touchId, 
							   const float previousX, const float previousY, 
							   const float currentX, const float currentY);
	
	static void HandleTouchUp(const unsigned int touchId, 
							float x, float y, int tapCount);
	
	static void HandleOrientationVectorUpdate(const float x, const float y, const float z);
};


#endif