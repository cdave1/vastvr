/*
 * hdUIReceiver means a potential receiver of user input
 *
 *
 */


#ifndef HD_RECEIVER_H_
#define HD_RECEIVER_H_

#include "hdGame.h"
#include "hdGraphicsLibrary.h"
#include "hdMath.h"


class hdUIReceiver : public hdPolygon, public hdDrawable
{
public:
	hdUIReceiver();
	
	hdUIReceiver(hdGameWorld* gameWorld);
	
	hdUIReceiver(hdGameWorld* gameWorld, bool addToContext);
	
	~hdUIReceiver();
	
	virtual bool MouseDown(float x, float y) = 0;

	virtual bool MouseOver(float x, float y) = 0;

	virtual bool MouseUp(float x, float y) = 0;
	
	virtual bool MouseDoubleClick(float x, float y);
	
	/**
	 * For anything above two taps.
	 */
	virtual bool MouseMultiClick(float x, float y, unsigned int clicks);
	
	virtual void RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector);
	
	virtual void RotateAndTranslate2(const hdVec3& rotVector, const hdVec3& rotationAxis,
									 const hdVec3& transVector, const hdVec3& transVectorAxis);
	
	virtual void Draw() const = 0;
	
	const bool IsDisabled();
	
	const bool IsEnabled();
	
	virtual void Disable();
	
	virtual void Enable();
	
	void SetCanRotate(const bool canRotate);
	
	const bool GetCanRotate() const;
	
	int TestFunc();
	
protected:
	bool m_isEnabled;
	
	bool m_canRotate;

private:
	void Init(bool addToContext);
};


inline const bool hdUIReceiver::IsDisabled()
{
	return !m_isEnabled;
}


inline const bool hdUIReceiver::IsEnabled()
{
	return m_isEnabled;
}


inline void hdUIReceiver::Disable()
{
	m_isEnabled = false;
}


inline void hdUIReceiver::Enable()
{
	m_isEnabled = true;
}


inline void hdUIReceiver::SetCanRotate(const bool canRotate)
{
	m_canRotate = canRotate;
}


inline const bool hdUIReceiver::GetCanRotate() const
{
	return m_canRotate;
}

inline int hdUIReceiver::TestFunc()
{
	return 1;
}


#endif