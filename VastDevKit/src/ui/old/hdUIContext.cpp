/*
 *  hdUIContext.cpp
 *  Smashed
 *
 *  Created by david on 2/11/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#include "hdUIContext.h"

// HACK HACK HACK
// THESE SHOULD NOT FUCKING BE GLOBAL!!!!
static hdTypedefList<hdUIReceiver *, 256> *m_contextReceivers = NULL;

static hdTypedefList<hdUIController *, 256> *m_contextControllers = NULL;

static bool loggingEnabled = false;


/**
 * HACK HACK HACK
 *
 * The Init function can be called anytime, and thus can be used to clear
 * the receivers list.  This is a desirable effect, but it assumes that
 * there are no existing receivers for which we want to receive events for.
 *
 * A fix for this is to allow the context to support multiple controllers,
 * using weakly held pointers.  
 * 
 * This also means the receivers list does not need to be maintained -- 
 * events are simply passed to all the active controllers which handle shit 
 * internally.
 *
 * TODO: completely rethink this concept for hdTech2.  The idea is that
 * we do not want to handle interface interaction directly in view
 * controller implementations -- we should get it for free by sending all
 * user input events to the context and letting the context do what it takes
 * to route the events to the controls.
 */
void hdUIContext::Init()
{
#ifdef DEBUG 
	loggingEnabled = false;
#endif
	
	hdDeleteSafely(m_contextControllers);
	m_contextControllers = new hdTypedefList<hdUIController *, 256>();
	
	hdDeleteSafely(m_contextReceivers);
	m_contextReceivers = new hdTypedefList<hdUIReceiver *, 256>();
}


void hdUIContext::TearDown()
{	
	hdDeleteSafely(m_contextControllers);
	hdDeleteSafely(m_contextReceivers);
}


void hdUIContext::EnableLogging()
{
	loggingEnabled = true;
}


void hdUIContext::DisableLogging()
{
	loggingEnabled = false;
}


bool hdUIContext::AddReceiver(const hdUIReceiver *receiver)
{
	int res;
	if (!m_contextReceivers) return false;
	
	res = m_contextReceivers->Add((hdUIReceiver *)receiver);
	
	if (HD_COLLECTIONS_ERROR_FULL == res ||
		HD_COLLECTIONS_ERROR_INVALID_INDEX == res)
	{
		hdError(-1, "Collection was full. Could not add receiver.");
		return false;
	}
    
    hdPrintf("Added receiver! %p\n", receiver);
	
	return true;
}


bool hdUIContext::RemoveReceiver(const hdUIReceiver *receiver)
{
    hdPrintf("Removed receiver! %p\n", receiver);
	return m_contextReceivers->Remove((hdUIReceiver *)receiver);
}


bool hdUIContext::AddController(const hdUIController *controller)
{
	int res;
	if (!m_contextControllers) return false;
	
	res = m_contextControllers->Add((hdUIController *)controller);
	
	if (HD_COLLECTIONS_ERROR_FULL == res ||
		HD_COLLECTIONS_ERROR_INVALID_INDEX == res)
	{
		hdError(-1, "Collection was full. Could not add receiver.");
		return false;
	}
	
	return true;
}


bool hdUIContext::RemoveController(const hdUIController *controller)
{
	return m_contextControllers->Remove((hdUIController *)controller);
}


void hdUIContext::HandleTouchDown(const unsigned int touchId, 
								float x, float y, int tapCount)
{
	hdAssert(m_contextControllers);
	hdAssert(m_contextControllers->GetItemCount() > 0);
	hdAssert(m_contextReceivers);
	hdVec2 screen;
	
	m_contextControllers->GetItems()[0]->ConvertRawToScreen(screen, x, y);
	
	if (loggingEnabled)
		hdPrintf("\t\tCONTEXT HandleTouchDown: %3.3f, %3.3f, %3.3f, %3.3f\n", x, y, screen.x, screen.y);
	
	for (uint32 i = 0; i < m_contextReceivers->GetItemCount(); ++i)
	{
		if (m_contextReceivers->GetItems()[i]->MouseDown(screen.x, screen.y))
		{
			hdPrintf("\t\tMouseDown success: %d\n", i);
			return;
		}
	}
	
	for (uint32 i = 0; i < m_contextControllers->GetItemCount(); ++i)
	{
		m_contextControllers->GetItems()[i]->HandleTouchDown(touchId, x, y, tapCount);
	}
}


void hdUIContext::HandleTouchMoved(const unsigned int touchId, 
								 const float previousX, const float previousY, 
								 const float currentX, const float currentY)
{
	hdAssert(m_contextControllers);
	hdAssert(m_contextControllers->GetItemCount() > 0);
	hdAssert(m_contextReceivers);
	hdVec2 screen;
	
	m_contextControllers->GetItems()[0]->ConvertRawToScreen(screen, currentX, currentY);
	
	for (uint32 i = 0; i < m_contextReceivers->GetItemCount(); ++i)
	{
		if (m_contextReceivers->GetItems()[i]->MouseOver(screen.x, screen.y))
			return;
	}
	
	for (uint32 i = 0; i < m_contextControllers->GetItemCount(); ++i)
	{
		m_contextControllers->GetItems()[i]->HandleTouchMoved(touchId, previousX, previousY, currentX, currentY);
	}
}


void hdUIContext::HandleTouchUp(const unsigned int touchId, 
							  float x, float y, int tapCount)
{
	hdAssert(m_contextControllers);
	hdAssert(m_contextControllers->GetItemCount() > 0);
	hdAssert(m_contextReceivers);
	hdVec2 screen;
	
	m_contextControllers->GetItems()[0]->ConvertRawToScreen(screen, x, y);
	
	if (loggingEnabled)
		hdPrintf("\t\tCONTEXT HandleTouchUp: %3.3f, %3.3f, %3.3f, %3.3f (tapcount: %d)\n", x, y, screen.x, screen.y, tapCount);
	
	for (uint32 i = 0; i < m_contextReceivers->GetItemCount(); ++i)
	{
		if (tapCount == 0 || tapCount == 1)
		{
			if (m_contextReceivers->GetItems()[i]->MouseUp(screen.x, screen.y))
				return;
		}
		else if (tapCount == 2)
		{
			if (m_contextReceivers->GetItems()[i]->MouseDoubleClick(screen.x, screen.y))
				return;
		}
		else if (tapCount > 2)
		{
			if (m_contextReceivers->GetItems()[i]->MouseMultiClick(screen.x, screen.y, tapCount))
				return;			
		}
	}
	
	for (uint32 i = 0; i < m_contextControllers->GetItemCount(); ++i)
	{
		m_contextControllers->GetItems()[i]->HandleTouchUp(touchId, x, y, tapCount);
	}
}



/*
void hdUIContext::HandleTouchMovedDouble(const float aPreviousX, const float aPreviousY,
									   const float aCurrentX, const float aCurrentY,
									   const float bPreviousX, const float bPreviousY,
									   const float bCurrentX, const float bCurrentY)
{
	
	for (uint32 i = 0; i < m_contextControllers->GetItemCount(); ++i)
	{
		m_contextControllers->GetItems()[i]->HandleTouchMovedDouble(aPreviousX, aPreviousY,
														   aCurrentX, aCurrentY,
														   bPreviousX, bPreviousY,
														   bCurrentX, bCurrentY);
	}
}*/


void hdUIContext::HandleOrientationVectorUpdate(const float x, const float y, const float z)
{
	if (m_contextReceivers->GetItemCount() == 0) return;
	
	//m_controller->HandleOrientationChanged(x,y,z);	 
}




