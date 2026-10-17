#ifndef HD_BUTTON_H_
#define HD_BUTTON_H_

#include "UI/hdUIReceiver.h"

typedef enum
{
	e_buttonWaiting = 0,
	e_buttonMouseDown,
	e_buttonMouseOver,
	e_buttonMouseOut,
	e_buttonMouseUp,
	e_buttonStateDisabled,
	e_buttonStateCount
} e_buttonState;


class hdUIButton : public hdUIReceiver
{
public:
	hdUIButton() {} 
	
	
	hdUIButton(std::string textureNormal, 
			 std::string textureOver, 
			 std::string textureUp);
	
	
	hdUIButton(const char* textureNormal, 
			 const char* textureOver, 
			 const char* textureUp);
	
	
	hdUIButton(const char* textureNormal, 
			 const char* textureOver, 
			 const char* textureUp, 
			 hdGameWorld* gameWorld, 
			 const hdAABB& aabb);
	
	hdUIButton(const char* textureNormal, 
			 const char* textureOver, 
			 const char* textureUp, 
			 hdGameWorld* gameWorld, 
			 const float x, const float y, 
			 const float w, const float h);
	
	~hdUIButton();
	
	bool MouseDown(float x, float y);
	
	bool MouseOver(float x, float y);
	
	bool MouseUp(float x, float y);
	
	bool MouseDoubleClick(float x, float y);
	
	bool MouseMultiClick(float x, float y, unsigned int clicks);
	
	hdUIReceiver* MouseUpFirstReceiver(float x, float y);
	
	void Draw() const;
	
	void ResetButton()
	{
		m_buttonState = e_buttonWaiting;
	}
	
	void SetMouseUpListener(void *obj, void (*func)(void *, void *));
	
	void SetMouseOverListener(void *obj, void (*func)(void *, void *));
	
	void SetMouseDownListener(void *obj, void (*func)(void *, void *));
    
    void SetMouseDoubleClickListener(void *obj, void (*func)(void *, void *));
	
	void SetNormalTexture(const hdTexture *tex);

private:
	
	void Init();
	
	void DoMouseDownCallback();
	
	void DoMouseOverCallback();
	
	void DoMouseUpCallback();
    
    void DoDoubleClickCallback();
	
	e_buttonState m_buttonState;
	
	hdTexture* m_textureNormal;
	
	hdTexture* m_textureOver;
	
	hdTexture* m_textureUp;
	
	
	
	void (*m_mouseDownCallback)(void *, void *);
	
	void *m_mouseDownCallbackObject;
	
	void (*m_mouseOverCallback)(void *, void *);
	
	void *m_mouseOverCallbackObject;
	
	void (*m_mouseUpCallback)(void *, void *);
	
	void *m_mouseUpCallbackObject;
    
    void (*m_mouseDoubleClickCallback)(void *, void *);
	
	void *m_mouseDoubleClickCallbackObject;
};




#endif