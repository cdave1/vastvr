/*
 *  hdUIController.cpp
 *  hdk
 *
 *  Created by david on 23/04/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 */

#include "hdUIController.h"

#define kDefaultScreenDepth 50.0f


hdUIController::hdUIController()
{
	m_parentController = NULL;
	float width = hdConfig::GetFloatValue("d_screenWidth");
    float height = hdConfig::GetFloatValue("d_screenHeight");
	this->Init(e_hdInterfaceOrientationPortrait, 
			   width / 2.0f, 
			   height / 2.0f, 
			   kDefaultScreenDepth);
}


hdUIController::hdUIController(const hdUIController* parentController)
{
	m_parentController = (hdUIController *)parentController;
	float width = hdConfig::GetFloatValue("d_screenWidth");
    float height = hdConfig::GetFloatValue("d_screenHeight");
	this->Init(e_hdInterfaceOrientationPortrait, 
			   width / 2.0f, 
			   height / 2.0f, 
			   kDefaultScreenDepth);
}


hdUIController::hdUIController(const e_hdInterfaceOrientation orientation, const hdUIController* parentController)
{
	m_parentController = (hdUIController *)parentController;
	float width = hdConfig::GetFloatValue("d_screenWidth");
    float height = hdConfig::GetFloatValue("d_screenHeight");
	this->Init(orientation, 
			   width / 2.0f, 
			   height / 2.0f, 
			   kDefaultScreenDepth);
}


hdUIController::hdUIController(const float screenWidth, const float screenHeight, const float screenDepth)
{
	m_parentController = NULL;
	this->Init(e_hdInterfaceOrientationPortrait, screenWidth, screenHeight, screenDepth);
}


void hdUIController::Init(const e_hdInterfaceOrientation orientation, const float screenWidth, const float screenHeight, const float screenDepth)
{
	hdAABB worldAABB;
	hdAABB projectionAABB;
	
	m_projection = NULL;
	m_isEnabled = true;
	m_momentumCancelCountdown = -1;
	
	worldAABB.lower = hdVec3(0,0,0) - hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
	worldAABB.upper = hdVec3(m_screenWidth, m_screenHeight, -m_screenDepth);
	m_gameWorld = new hdGameWorld(worldAABB);
	
	SetOrientation(orientation);

	m_followObjectAnimation = NULL;
	
	m_followingObject = NULL;
	m_centerZoomAnimation = NULL;
	m_centerZoomAction = NULL;
	m_duration = 0.0f;
	m_fracn = 0.0f;
	
	m_moveVector.Set(0,0,0);
	
	m_panMomentumEnabled = false;
	m_lastPanDirectionVector.Set(0.0f, 0.0f);
	
	hdUIContext::AddController(this);
}


hdUIController::~hdUIController()
{
	hdUIContext::RemoveController(this);
	
	hdAnimationController::Instance()->StopAnimations(this);
	hdAnimationController::Instance()->PurgeAnimations(this);
	
	hdDeleteSafely(m_projection);
	hdDeleteSafely(m_gameWorld);
}


void hdUIController::ResetGameWorld()
{
	delete m_gameWorld;
	hdAABB worldAABB;
	worldAABB.lower = hdVec3(0,0,0) - hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
	worldAABB.upper = hdVec3(m_screenWidth, m_screenHeight, -m_screenDepth);
	m_gameWorld = new hdGameWorld(worldAABB);
}


/*
 * Call this to kick off an animation that centres the projection on the given point,
 * at the given zoom level, over a specific duration.
 *
 * If duration 0, effect is instant.
 *
 * If damping, the animation effect will "slow" as it gets within a certain threshold distance
 * of the destination (without curves to define this stuff, it may look faked).
 */
void hdUIController::CentreAndZoomProjection(const hdVec3& dest, const hdVec3& zoomDim, const float duration)
{
	hdVec3 lo = m_projection->GetAABB().lower;
	hdVec3 hi = m_projection->GetAABB().upper;
	m_startingAABB = m_projection->GetAABB();
	
	m_moveVector = dest - m_projection->GetWorldCenter();
	
	if (duration <= 0.0f)
	{
		lo -= m_moveVector;
		hi -= m_moveVector;
	
		m_projection->SetAABB(lo, hi);
	} 
	else
	{
		if (m_centerZoomAnimation == NULL)
		{
			m_centerZoomAnimation = hdAnimationController::CreateAnimation(this, false);
		}
		
		if (m_centerZoomAction == NULL)
		{
			m_centerZoomAction = new hdFunctionAction<hdUIController>();
		}
		m_fracn = 0.0f;
		
		// Function action calls the step function below until times out.
		m_centerZoomAction->SetDuration(m_duration);
		m_centerZoomAction->SetFunctionObject(this);
		m_centerZoomAction->SetFunction(&hdUIController::CenterAndZoomProjectionStep);
				
		m_centerZoomAnimation->AddGameObject(m_projection);
		m_centerZoomAnimation->AddAction(m_centerZoomAction);
		m_centerZoomAnimation->StartAnimation();
	}
}


void hdUIController::CenterAndZoomProjectionStep(hdTimeInterval interval)
{
	// Formula: 
	//
	// n_0 = 0
	// n = n_-1 + ((1 - elapsedPercent) * factor)
	
	hdVec3 lo = m_startingAABB.lower;
	hdVec3 hi = m_startingAABB.upper;
	
	m_fracn += (1.0f - (m_centerZoomAction->GetProgress() / m_centerZoomAction->GetDuration())) * (0.2f); // * interval);
	
	hdVec3 diffCenter = m_fracn * m_moveVector;
		
	lo += diffCenter;
	hi += diffCenter;

	m_projection->SetAABB(lo, hi);
}





void hdUIController::FollowGameObject(const hdGameObject *object, const float duration)
{
	if (object == NULL) return;
	
	m_followingObject = (hdGameObject *)object;
	
	if (m_followObjectAnimation == NULL)
	{
		m_followObjectAnimation = hdAnimationController::CreateAnimation(this, false);
	} 
	else
	{
		m_followObjectAnimation->StopAnimation(false, false);
	}
	
	m_followObjectAnimation->RemoveAllGameObjects();
	m_followObjectAnimation->RemoveAllActions();
	
	hdFunctionAction<hdUIController> *followObjectAction = new hdFunctionAction<hdUIController>();
	
	// Function action calls the step function below until times out.
	followObjectAction->SetDuration(duration);
	followObjectAction->SetFunctionObject(this);
	followObjectAction->SetFunction(&hdUIController::FollowGameObjectStep);
	
	m_followObjectAnimation->AddGameObject(m_followingObject);
	m_followObjectAnimation->AddAction(followObjectAction);
	m_followObjectAnimation->StartAnimation();
}


void hdUIController::FollowGameObjectStep(hdTimeInterval interval)
{
	if (m_followingObject == NULL) return;

	ZoomProjection( 0.0f, 0.0f,
				   0.5f, 0.75f,
				    0.0f, 0.0f,
				   -0.75f, -0.75f);
}


const float hdUIController::GetScreenWidthForCurrentOrientation() const
{
	return m_PixelScreenWidth;
}


const float hdUIController::GetScreenHeightForCurrentOrientation() const
{
	return m_PixelScreenHeight;
}


void hdUIController::TranslateProjection(const hdVec3& trans)
{
	hdVec3 lo, hi, box;
	
	lo = m_projection->GetAABB().lower;
	hi = m_projection->GetAABB().upper;
	box = hi - lo;
	
	lo += trans;
	hi += trans;
	
	lo.x = hdClamp(lo.x, GetWorldAABB().lower.x, (GetWorldAABB().upper.x - box.x));
	lo.y = hdClamp(lo.y, GetWorldAABB().lower.y, (GetWorldAABB().upper.y - box.y));
	
	hi.x = hdClamp(hi.x, GetWorldAABB().lower.x + box.x, GetWorldAABB().upper.x);
	hi.y = hdClamp(hi.y, GetWorldAABB().lower.y + box.y, GetWorldAABB().upper.y);
	
	m_projection->SetAABB(lo, hi);
	m_projection->ResetAABB();
}


void hdUIController::PanProjection(const hdVec2& pan)
{
	PanProjection(0.0f, 0.0f, pan.x, pan.y);
}


void hdUIController::PanProjection(const float aPreviousX, const float aPreviousY,
									const float aCurrentX, const float aCurrentY)
{
	hdVec2 previous, current;
	hdVec3 lo, hi, box, diff;
	hdVec2 screenDiff;

	lo = m_projection->GetAABB().lower;
	hi = m_projection->GetAABB().upper;
	box = hi - lo;
	
	ConvertRawToScreen(previous, aPreviousX, aPreviousY);
	ConvertRawToScreen(current, aCurrentX, aCurrentY);
	
	// Screen distance travelled.
	screenDiff = previous - current;
	
	// no movement - we only want to stop if the user
	// has stopped moving, but not removed, their finger.
	if (screenDiff.x == 0 && screenDiff.y == 0) 
	{	
		
		if (m_momentumCancelCountdown == -1)
		{
			m_momentumCancelCountdown = 10;
		}
		else
		{
			--m_momentumCancelCountdown;
			
			if (m_momentumCancelCountdown == 0)
			{
				m_lastPanDirectionVector.Set(0, 0);
				m_momentumCancelCountdown = -1;
			}
		}
		
		//if (hdFastHypotenuse(m_lastPanDirectionVector.x, m_lastPanDirectionVector.y) < 0.001f)
		//{
		//	m_lastPanDirectionVector.Set(0, 0);
		//}
		return;
	}	
	m_momentumCancelCountdown = -1;
	
	// Game units per pixel to find the actual quantity of game units used.
	float unitsPerPixelX = box.x / m_PixelScreenWidth;
	float unitsPerPixelY = box.y / m_PixelScreenHeight;
	
	diff.Set(screenDiff.x * unitsPerPixelX, screenDiff.y * unitsPerPixelY, 0.0f);
	
	lo += diff;
	hi += diff;
	
	// Clamp lo and hi to the boundaries of the world.
	lo.x = hdClamp(lo.x, GetWorldAABB().lower.x, (GetWorldAABB().upper.x - box.x));
	lo.y = hdClamp(lo.y, GetWorldAABB().lower.y, (GetWorldAABB().upper.y - box.y));
	
	hi.x = hdClamp(hi.x, GetWorldAABB().lower.x + box.x, GetWorldAABB().upper.x);
	hi.y = hdClamp(hi.y, GetWorldAABB().lower.y + box.y, GetWorldAABB().upper.y);
	
	m_projection->SetAABB(lo, hi);
	
	m_lastPanDirectionVector.Set(aPreviousX - aCurrentX, aPreviousY - aCurrentY);
}



// HACK: why is there so much code here? surely this should be simpler...
void hdUIController::ZoomProjection(const float aPreviousX, const float aPreviousY,
										 const float aCurrentX, const float aCurrentY,
										 const float bPreviousX, const float bPreviousY,	
										 const float bCurrentX, const float bCurrentY)
{
	float prevDist, currDist, unitsPerPixelX;
	hdVec3 lo, hi, box;
	
	lo = m_projection->GetAABB().lower;
	hi = m_projection->GetAABB().upper;
	box = hi - lo;
	
	unitsPerPixelX = box.x / m_PixelScreenWidth;
	
	// Just compare the hypotenuse of previous with current.
	prevDist = hdFastHypotenuse(aPreviousX - bPreviousX, aPreviousY - bPreviousY);
	currDist = hdFastHypotenuse(aCurrentX - bCurrentX, aCurrentY - bCurrentY);
	
	if (prevDist > currDist)
	{
		// Can't get any bigger, so bail out.
		if (box.x >= m_GameMaxScreenWidth) return;
		
		// f is the absolute zoom distance in game area units
		float f = (prevDist - currDist)* unitsPerPixelX;
		
		// Keep the projection centered on the middle of the screen
		lo -= hdVec3(f * m_lowerZoomRatioX, f * m_lowerZoomRatioY, 0.0f);
		hi += hdVec3(f * m_upperZoomRatioX, f * m_upperZoomRatioY, 0.0f);
	} 
	else if (prevDist < currDist)
	{
		// Can't get any smaller, so bail out.
		if (box.x <= m_GameMinScreenWidth) return;
		
		float f = (currDist - prevDist) * unitsPerPixelX;
		
		lo += hdVec3(f * m_lowerZoomRatioX, f * m_lowerZoomRatioY, 0.0f);
		hi -= hdVec3(f * m_upperZoomRatioX, f * m_upperZoomRatioY, 0.0f);
	} 
	
	// Clamp lo and hi to the boundaries of the world.
	lo.x = hdClamp(lo.x, GetWorldAABB().lower.x, (GetWorldAABB().upper.x - box.x));
	lo.y = hdClamp(lo.y, GetWorldAABB().lower.y, (GetWorldAABB().upper.y - box.y));
	
	hi.x = hdClamp(hi.x, GetWorldAABB().lower.x + box.x, GetWorldAABB().upper.x);
	hi.y = hdClamp(hi.y, GetWorldAABB().lower.y + box.y, GetWorldAABB().upper.y);
	
	// ensure the zoom aabb does not get smaller or greater than
	// the min/max projection size.
	if ((hi.x - lo.x) <= m_GameMinScreenWidth)
	{
		hi.x = lo.x + m_GameMinScreenWidth;
		hi.y = lo.y + m_GameMinScreenHeight;
	}
	
	if ((hi.x - lo.x) >= m_GameMaxScreenWidth)
	{
		hi.x = lo.x + m_GameMaxScreenWidth;
		hi.y = lo.y + m_GameMaxScreenHeight;
	}
	
	// ensure the aabb is the same ratio as the screen
	if ((hi.y - lo.y) / (hi.x - lo.x) != m_ScreenAspectRatio)
	{
		// Just set the bb upper value
		hi.y = lo.y + ((hi.x - lo.x) / m_ScreenAspectRatio);
	}
	
	m_projection->SetAABB(lo, hi);
}


/*
 * NOTE: Will not work properly with Raw input coordinates - call
 * ConvertRawToScreen on such coordinates before calling this function.
 *
 * TODO: look for a way to enforce this rule.
 */
void hdUIController::ConvertScreenToInterface(hdVec2& vOut, float x, float y)
{
	hdVec3 projectionBoundingBox = m_projection->GetAABB().upper - m_projection->GetAABB().lower;
	hdVec3 projectionPosition = m_projection->GetAABB().lower;
	
	float unitsPerPixelX = projectionBoundingBox.x / m_PixelScreenWidth;
	float unitsPerPixelY = projectionBoundingBox.y / m_PixelScreenHeight;
	
	vOut.Set((x * unitsPerPixelX) + projectionPosition.x,
			 (y * unitsPerPixelY) + projectionPosition.y);
}


void hdUIController::ConvertInterfaceToScreen(hdVec2& vOut, float x, float y)
{
	hdVec3 projectionBoundingBox = m_projection->GetAABB().upper - m_projection->GetAABB().lower;
	hdVec3 projectionPosition = m_projection->GetAABB().lower;
	
	float pixelsPerUnitX = m_PixelScreenWidth / projectionBoundingBox.x;
	float pixelsPerUnitY = m_PixelScreenHeight / projectionBoundingBox.y;
	
	vOut.Set(pixelsPerUnitX * (x - projectionPosition.x), 
				 pixelsPerUnitY * (y - projectionPosition.y));
}


void hdUIController::ConvertRawToScreen(hdVec2& vOut, float x, float y)
{
#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
	vOut.Set(x, m_PixelScreenHeight - y);
#else
	vOut.Set(x, y);
#endif
}



void hdUIController::HandleResize(const float newWidth, const float newHeight)
{
	hdAABB boxAABB, projectionAABB;
	
	m_PixelScreenWidth = newWidth;
	m_PixelScreenHeight = newHeight;
	m_ScreenAspectRatio = newHeight / newWidth;
	
	m_GameMinScreenWidth = newWidth / m_pixelsPerGameUnit; 
	m_GameMinScreenHeight = newHeight / m_pixelsPerGameUnit;
	m_GameMaxScreenWidth = 4 * m_GameMinScreenWidth; 
	m_GameMaxScreenHeight = 4 * m_GameMaxScreenHeight; 
	
	m_screenWidth = newWidth / 100.0f;
	m_screenHeight = newHeight / 100.0f;
	m_screenDepth = kDefaultScreenDepth;
	
	hdVec3 center = m_projection->GetWorldCenter();
	
	boxAABB.lower = hdVec3(0,0,0);
	boxAABB.upper = hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
	
	projectionAABB.lower = center - hdVec3(boxAABB.Width(), boxAABB.Height(), -m_screenDepth);
	projectionAABB.upper = center + hdVec3(boxAABB.Width(), boxAABB.Height(), m_screenDepth);
	
	m_projection->SetAABB(projectionAABB.lower, 
						  projectionAABB.upper);
	
	HandleOrientationChanged();	
}


void hdUIController::ResetOrientation()
{
	this->SetOrientation(m_orientation);
}


void hdUIController::SetOrientation(const e_hdInterfaceOrientation orientation)
{
	hdAABB projectionAABB;
    
    m_PixelScreenWidth = hdConfig::GetFloatValue("d_screenWidth");
    m_PixelScreenHeight = hdConfig::GetFloatValue("d_screenHeight");
	m_ScreenAspectRatio = hdConfig::GetFloatValue("d_screenAspectRatio");
    
    m_GameMinScreenWidth = hdConfig::GetFloatValue("c_minWidth");
    m_GameMinScreenHeight = hdConfig::GetFloatValue("c_minHeight");
    m_GameMaxScreenWidth = hdConfig::GetFloatValue("c_maxWidth");
    m_GameMaxScreenHeight = hdConfig::GetFloatValue("c_maxHeight");
    
    m_lowerZoomRatioX = hdConfig::GetFloatValue("c_lowerZoomRatioX");
    m_lowerZoomRatioY = hdConfig::GetFloatValue("c_lowerZoomRatioY");
    m_upperZoomRatioX = hdConfig::GetFloatValue("c_upperZoomRatioX");
    m_upperZoomRatioY = hdConfig::GetFloatValue("c_upperZoomRatioY");
    
    m_pixelsPerGameUnit = hdConfig::GetFloatValue("c_pixelsPerGameUnit");
	
	if (m_projection == NULL)
	{
        m_screenWidth = hdConfig::GetFloatValue("c_defaultWidth");
        m_screenHeight = hdConfig::GetFloatValue("c_defaultHeight");
		m_screenDepth = hdConfig::GetFloatValue("c_defaultDepth");

		projectionAABB.lower = hdVec3(0,0,0) - hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
		projectionAABB.upper = hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
		m_projection = new hdOrthographicProjection(m_gameWorld, projectionAABB);
	}

	m_landscapeRotationZValue = 0.0f;
}



void hdUIController::AnimateShow() {}

void hdUIController::HandleTouchUp(const unsigned int touchId, float x, float y, int tapCount) {}

void hdUIController::HandleTouchDown(const unsigned int touchId, float x, float y, int tapCount) {}

void hdUIController::HandleTouchMoved(const unsigned int touchId, 
									const float previousX, const float previousY, 
									const float currentX, const float currentY) {}

