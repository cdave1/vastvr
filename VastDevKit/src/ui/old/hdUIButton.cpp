


#include "hdUIButton.h"

hdUIButton::hdUIButton(std::string textureNormal, 
				   std::string textureOver, 
				   std::string textureUp) : hdUIReceiver(NULL, true)
{
	m_textureNormal = hdTextureManager::Instance()->FindTexture(textureNormal.c_str(), TT_16Pic);
	m_textureOver = hdTextureManager::Instance()->FindTexture(textureOver.c_str(), TT_16Pic);
	m_textureUp = hdTextureManager::Instance()->FindTexture(textureUp.c_str(), TT_16Pic);
	
	
	
	this->SetAs2DBox(hdVec3(0,0,0), hdVec3(1.0f, 1.0f, 1.0f));
	
	this->Init();
}


hdUIButton::hdUIButton(const char* textureNormal, 
				   const char* textureOver, 
				   const char* textureUp, 
				   hdGameWorld* gameWorld) : hdUIReceiver(gameWorld, true)
{
	m_textureNormal = hdTextureManager::Instance()->FindTexture(textureNormal, TT_16Pic);
	m_textureOver = hdTextureManager::Instance()->FindTexture(textureOver, TT_16Pic);
	m_textureUp = hdTextureManager::Instance()->FindTexture(textureUp, TT_16Pic);
	
	this->SetAs2DBox(hdVec3(0,0,0), hdVec3(1.0f, 1.0f, 1.0f));
	
	this->Init();
}




hdUIButton::hdUIButton(const char* textureNormal, 
					 const char* textureOver, 
					 const char* textureUp, 
					 hdGameWorld* gameWorld, 
					 const hdAABB& aabb) : hdUIReceiver(gameWorld, true)
{
	m_textureNormal = hdTextureManager::Instance()->FindTexture(textureNormal, TT_16Pic);
	m_textureOver = hdTextureManager::Instance()->FindTexture(textureOver, TT_16Pic);
	m_textureUp = hdTextureManager::Instance()->FindTexture(textureUp, TT_16Pic);
	
	this->SetAs2DBox(aabb.lower, aabb.upper);
	
	this->Init();
}


hdUIButton::hdUIButton(const char* textureNormal, 
		 const char* textureOver, 
		 const char* textureUp, 
		 hdGameWorld* gameWorld, 
		 const float x, const float y, 
		 const float w, const float h) : hdUIReceiver(gameWorld, true)
{
	m_textureNormal = hdTextureManager::Instance()->FindTexture(textureNormal, TT_16Pic);
	m_textureOver = hdTextureManager::Instance()->FindTexture(textureOver, TT_16Pic);
	m_textureUp = hdTextureManager::Instance()->FindTexture(textureUp, TT_16Pic);
	
	this->SetAs2DBox(hdVec3(x,y,0.0), hdVec3(x+w,y+h,0.0));
	
	this->Init();
}


void hdUIButton::Init()
{
	m_buttonState = e_buttonWaiting;
	
	m_mouseDownCallbackObject = NULL;
	m_mouseDownCallback = NULL;

	m_mouseOverCallbackObject = NULL;
	m_mouseOverCallback = NULL;

	m_mouseUpCallbackObject = NULL;
	m_mouseUpCallback = NULL;
}


hdUIButton::~hdUIButton() 
{
}


void hdUIButton::SetMouseDownListener(void *obj, void (*func)(void *, void *))
{
	m_mouseDownCallbackObject = obj;
	m_mouseDownCallback = func;
}


void hdUIButton::SetMouseOverListener(void *obj, void (*func)(void *, void *))
{
	m_mouseOverCallbackObject = obj;
	m_mouseOverCallback = func;
}


void hdUIButton::SetMouseUpListener(void *obj, void (*func)(void *, void *))
{
	m_mouseUpCallbackObject = obj;
	m_mouseUpCallback = func;
}


void hdUIButton::SetMouseDoubleClickListener(void *obj, void (*func)(void *, void *))
{
    m_mouseDoubleClickCallbackObject = obj;
    m_mouseDoubleClickCallback = func;
}


void hdUIButton::DoMouseDownCallback()
{
	if (m_mouseDownCallbackObject != NULL && m_mouseDownCallback != NULL)
	{
		(*m_mouseDownCallback)(m_mouseDownCallbackObject, this); //, (void *)this);
	}
}


void hdUIButton::DoMouseOverCallback()
{
	if (m_mouseOverCallbackObject != NULL && m_mouseOverCallback != NULL)
	{
		(*m_mouseOverCallback)(m_mouseOverCallbackObject, this); //, (void *)this);
	}
}


void hdUIButton::DoMouseUpCallback()
{
	if (m_mouseUpCallbackObject != NULL && m_mouseUpCallback != NULL)
	{
		(*m_mouseUpCallback)(m_mouseUpCallbackObject, this); //, (void *)this);
	}
}


void hdUIButton::DoDoubleClickCallback()
{
	if (m_mouseDoubleClickCallbackObject != NULL && m_mouseDoubleClickCallback != NULL)
	{
		(*m_mouseDoubleClickCallback)(m_mouseDoubleClickCallbackObject, this); //, (void *)this);
	}    
}


void hdUIButton::SetNormalTexture(const hdTexture *tex)
{
	m_textureNormal = (hdTexture *)tex;
}


bool hdUIButton::MouseDown(float x, float y)
{
	if (!this->IsEnabled()) 
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{
		if (m_buttonState == e_buttonWaiting)
		{
#ifdef DEBUG 
			hdPrintf("[hdUIButton::MouseDown]\n");
#endif
			
			m_buttonState = e_buttonMouseDown;
			DoMouseDownCallback();
			return true;
		}
		else
		{
			return false;
		}
		
	} 
	else 
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
}


// If we did not click on the item, then state can not go into mouse over.
bool hdUIButton::MouseOver(float x, float y)
{
	if (!this->IsEnabled()) 
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{
		if (m_buttonState == e_buttonMouseDown)
		{
#ifdef DEBUG 
			hdPrintf("[hdUIButton::MouseOver] mouse down\n");
#endif
			m_buttonState = e_buttonMouseOver;
			DoMouseOverCallback();
			return true;
		}
		else if (m_buttonState == e_buttonMouseOver)
		{
#ifdef DEBUG 
			hdPrintf("[hdUIButton::MouseOver] over\n");
#endif
			DoMouseOverCallback();
			return true;
		}
		else
		{
			return false;
		}
		
	} 
	else 
	{
		if (m_buttonState == e_buttonMouseDown || m_buttonState == e_buttonMouseOver)
		{
			m_buttonState = e_buttonMouseOut;
		} 
		else
		{
			m_buttonState = e_buttonWaiting;
		}
		return false;
	}
}


bool hdUIButton::MouseUp(float x, float y)
{
	if (!this->IsEnabled()) 
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{	
		// if state must be in mouseOver for anything to happen:
		if (m_buttonState == e_buttonMouseOver || m_buttonState == e_buttonMouseDown)
		{
			m_buttonState = e_buttonMouseUp;
			DoMouseUpCallback();
			return true;
		}
		return false;
	} 
	else 
	{ 
		if (m_buttonState == e_buttonMouseOver || m_buttonState == e_buttonMouseDown)
		{
			// The mouse was moved off the button and released, so reset the state.
			m_buttonState = e_buttonWaiting;
		}
		return false;
	}
}


bool hdUIButton::MouseDoubleClick(float x, float y)
{
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{	
		// if state must be in mouseOver for anything to happen:
		if (m_buttonState == e_buttonMouseOver || m_buttonState == e_buttonMouseDown)
		{
			m_buttonState = e_buttonMouseUp;
			DoDoubleClickCallback();
			return true;
		}
		return false;
	}
	else 
	{
		if (m_buttonState == e_buttonMouseOver || m_buttonState == e_buttonMouseDown)
		{
			// The mouse was moved off the button and released, so reset the state.
			m_buttonState = e_buttonWaiting;
		}
		return false;
	}
	return false;
}


bool hdUIButton::MouseMultiClick(float x, float y, unsigned int clicks)
{
	m_buttonState = e_buttonWaiting;
	return false;
}


void hdUIButton::Draw() const
{
	if (this->IsHidden()) return;
	
	if (((hdUIButton*)this)->IsEnabled())
	{
		switch (m_buttonState) {
			case e_buttonMouseDown:
			case e_buttonMouseOver:
				if (m_textureOver)
				{
					DrawVertices(((hdPolygon*)this)->GetVertices(), ((hdPolygon*)this)->GetTextureCoordinates(), 4, m_aabb, m_textureOver);
				}
				break;
			case e_buttonMouseUp:
				if (m_textureUp)
				{
					DrawVertices(((hdPolygon*)this)->GetVertices(), ((hdPolygon*)this)->GetTextureCoordinates(), 4, m_aabb, m_textureUp);
				}
				break;
			case e_buttonWaiting:
			default:
				if (m_textureNormal)
				{
					DrawVertices(((hdPolygon*)this)->GetVertices(), ((hdPolygon*)this)->GetTextureCoordinates(), 4, m_aabb, m_textureNormal);
				}
				break;
		}
	}
	else
	{
		DrawVerticesWithTint(((hdPolygon*)this)->GetVertices(), 
							 ((hdPolygon*)this)->GetTextureCoordinates(), 
							 4, 
							 m_aabb, 
							 m_textureNormal, 
							 hdColor4f(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
	}
	
	// HACK HACK HACK: set state back to normal when state is up.
	if (m_buttonState == e_buttonMouseUp)
	{
		((hdUIButton*)this)->ResetButton();
	}
}