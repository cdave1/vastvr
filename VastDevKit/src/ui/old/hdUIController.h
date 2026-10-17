/*
 *  hdUIController.h
 *  hdk
 *
 *  Created by david on 23/04/10.
 *  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
 *
 *
 * "hdUIController" is a misnomer.  Better to be called
 * "hdUICameraController".
 */

#ifndef HD_INTERFACE_CONTROLLER_H_
#define HD_INTERFACE_CONTROLLER_H_

#include "hdCore.h"
#include "hdGraphicsLibrary.h"
#include "hdAnimationEngine.h"

#include "UI/hdUIEnums.h"
#include "UI/hdUIContext.h"

class hdUIController 
{
public:	
	hdUIController();
	
	hdUIController(const hdUIController* parentController);

	hdUIController(const e_hdInterfaceOrientation orientation, 
						  const hdUIController* parentController);
	
	hdUIController(const float screenWidth, 
						  const float screenHeight, 
						  const float screenDepth);
		
	~hdUIController();
	
	virtual void Step(double interval) = 0;
		
	virtual void Draw() = 0;
	
	virtual void AnimateShow();
	
	
	virtual void HandleTouchDown(const unsigned int touchId, float x, float y, int tapCount);
	
	virtual void HandleTouchMoved(const unsigned int touchId, 
						const float previousX, const float previousY, 
						const float currentX, const float currentY);
	
	virtual void HandleTouchUp(const unsigned int touchId, float x, float y, int tapCount);
	
	virtual bool HandleOrientationVectorUpdate(const float x, const float y, const float z) { return false; }
	
	virtual void HandleResize(const float newWidth, const float newHeight);
	
	virtual void HandleOrientationChanged() {}
	
	const e_hdInterfaceOrientation GetOrientation() const;
	
	const bool IsLandscapeOrientation() const;
	
	void SetOrientation(const e_hdInterfaceOrientation orientation);
	
	void ResetOrientation();
	
	//virtual void AnimateHide() = 0;
	
	const hdAABB GetWorldAABB() const;
	
	void SetWorldAABB(const hdAABB& aabb);
	
	void SetParentController(const hdUIController* parentController);
	
	void ConvertScreenToInterface(hdVec2& vOut, float x, float y);
	
	void ConvertInterfaceToScreen(hdVec2& vOut, float x, float y);
	
	void ConvertRawToScreen(hdVec2& vOut, float x, float y);
	
	const float GetRotationZValue() const;
	
	bool IsEnabled() const;
	
	bool IsDisabled() const;
	
	void Enable();
	
	void Disable();
	
	/**
	 * HACK HACK HACK.  These functions prove how fucked this hdUIController class is,
	 * because when the screen is oriented landscape we're returning the 
	 * height.  Total WTF!
	 *
	 * Gets the width of the screen for the current devices orientation, such
	 * that (0,0) is at the bottom left of the screen while held parallel to the 
	 * ground.
	 */
	const float GetScreenWidthForCurrentOrientation() const;
	
	const float GetScreenHeightForCurrentOrientation() const;
	
	const hdVec3 GetCameraLocation() const;
	
	void TranslateProjection(const hdVec3& trans);
	
protected:
	friend class hdUIContext;

	void ResetGameWorld();
	
	//virtual void FinishedController(hdUIController * controller);
	
	
	
	void PanProjection(const hdVec2& pan);
	
	void PanProjection(const float aPreviousX, const float aPreviousY,
					   const float aCurrentX, const float aCurrentY);
	
	void ZoomProjection(const float aPreviousX, const float aPreviousY,
						const float aCurrentX, const float aCurrentY,
						const float bPreviousX, const float bPreviousY,	
						const float bCurrentX, const float bCurrentY);
	
	
	void CentreAndZoomProjection(const hdVec3& center, const hdVec3& zoomDim, const float duration);
	
	void CenterAndZoomProjectionStep(hdTimeInterval interval);
	
	void FollowGameObject(const hdGameObject *object, const float duration);
	
	void FollowGameObjectStep(hdTimeInterval interval);
	
	
	
	// gameworld stuff
	hdUIController *m_parentController;
	
	hdGameWorld *m_gameWorld;
	
	float m_screenHeight;
	
	float m_screenWidth;
	
	float m_screenDepth;
	
	bool m_isEnabled;
	
	float m_PixelScreenWidth;
	float m_PixelScreenHeight;
	float m_ScreenAspectRatio;
	
	float m_GameMinScreenWidth;	
	float m_GameMinScreenHeight;
	float m_GameMaxScreenWidth;	
	float m_GameMaxScreenHeight;
	
	float m_pixelsPerGameUnit;
	float m_lowerZoomRatioX;
	float m_lowerZoomRatioY;
	float m_upperZoomRatioX;
	float m_upperZoomRatioY;
	
	float m_landscapeRotationZValue;
	
	e_hdInterfaceOrientation m_orientation;

	hdOrthographicProjection *m_projection;
	
	bool m_panMomentumEnabled;
	
	hdVec2 m_lastPanDirectionVector;

private:
	void Init(const e_hdInterfaceOrientation orientation, const float screenWidth, const float screenHeight, const float screenDepth);
	
	// Camera centre zooms to the game object position, and then follows it around.
	hdAnimation *m_followObjectAnimation;
	//static hdFunctionAction<hdUIController> *m_followObjectAction = NULL;
	hdGameObject* m_followingObject;
	
	hdAnimation *m_centerZoomAnimation;
	hdFunctionAction<hdUIController> *m_centerZoomAction;
	float m_duration;
	float m_fracn;
	
	hdAABB m_startingAABB;
	//hdAABB m_worldAABB;
	
	
	hdVec3 m_moveVector;
	
	int16 m_momentumCancelCountdown;
	
	
};


inline const hdVec3 hdUIController::GetCameraLocation() const
{
	if (!m_projection) return hdVec3(0,0,0);
	return m_projection->GetWorldCenter();
}


inline void hdUIController::SetParentController(const hdUIController* parentController)
{
	m_parentController = (hdUIController*)parentController;
}


inline const hdAABB hdUIController::GetWorldAABB() const
{
	return m_gameWorld->GetAABB();
}


inline void hdUIController::SetWorldAABB(const hdAABB& aabb)
{
	assert(m_gameWorld != NULL);
	m_gameWorld->SetAABB(aabb);
	//m_worldAABB = aabb;
}


inline const e_hdInterfaceOrientation hdUIController::GetOrientation() const
{
	return m_orientation;
}


inline const bool hdUIController::IsLandscapeOrientation() const
{
	return (m_orientation == e_hdInterfaceOrientationLandscapeLeft ||
			m_orientation == e_hdInterfaceOrientationLandscapeRight);
}


inline const float hdUIController::GetRotationZValue() const
{
	return m_landscapeRotationZValue;
}


inline bool hdUIController::IsEnabled() const
{
	return m_isEnabled;
}


inline bool hdUIController::IsDisabled() const
{
	return !m_isEnabled;
}


inline void hdUIController::Enable()
{
	m_isEnabled = true;
}


inline void hdUIController::Disable()
{
	m_isEnabled = false;
}


#endif