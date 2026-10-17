/*
 *  hdIBController.cpp
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "hdUIViewController.h"


hdUIViewController::hdUIViewController() : hdUIContainer(NULL)
{
    m_hdIBViewController = NULL;
    m_width = 0.0f;
	m_height = 0.0f;
}


hdUIViewController::hdUIViewController(const float width, const float height) : hdUIContainer(NULL)
{
    m_hdIBViewController = NULL;
	m_width = width;
	m_height = height;
	
	this->SetAs2DBox(hdVec3(0,0,0), hdVec3(m_width, m_height, 0));
	this->ResetAABB();
}


hdUIViewController::hdUIViewController(const char *hdIBViewControllerFilePath) : hdUIContainer(NULL)
{
	m_hdIBViewController = NULL;
    m_width = 0.0f;
	m_height = 0.0f;
	InitWithViewControllerFilePath(hdIBViewControllerFilePath);
}


hdUIViewController::~hdUIViewController()
{
}


void hdUIViewController::InitWithViewControllerFilePath(const char *hdIBViewControllerFilePath)
{
    if ((m_hdIBViewController = hdIBLoader::Instance()->LoadViewControllerFromFile(hdIBViewControllerFilePath)))
	{
		InitInterface();
		InitSounds();
		m_width = m_hdIBViewController->GetWidth();
		m_height = m_hdIBViewController->GetHeight();
		
		this->SetAs2DBox(hdVec3(0,0,0), hdVec3(m_width, m_height, 0));
		this->ResetAABB();
	}
	else 
	{
		hdError(0, "Could not load hdUIViewController at %s\n",hdIBViewControllerFilePath);
		hdAssert(false);
	}
}


void hdUIViewController::SetCloseListener(void *obj, void (*func)(void *, void *))
{
	m_closePrefsCallbackObject = obj;
	m_closePrefsCallback = func;
}


void hdUIViewController::DoClosePrefsCallback()
{
	if (m_closePrefsCallbackObject != NULL && m_closePrefsCallback != NULL)
	{
		(*m_closePrefsCallback)(m_closePrefsCallbackObject, this); //, (void *)this);
	}
}





void hdUIViewController::InitInterface()
{
	if (!m_hdIBViewController) return;
	
	((hdIBViewController *)m_hdIBViewController)->CreateInterfaceObjects();
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		this->Add((hdUIReceiver *)(proxy->GetInterfaceElement()));
	}
}


void hdUIViewController::SetMouseDownCallbackForAllItems(void *obj, void (*func)(void *, void *))
{
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeButton)
			((hdUIButton *)(proxy->GetInterfaceElement()))->SetMouseDownListener(obj, func);
	}
}


void hdUIViewController::SetMouseUpCallbackForAllItems(void *obj, void (*func)(void *, void *))
{
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeButton)
			((hdUIButton *)(proxy->GetInterfaceElement()))->SetMouseUpListener(obj, func);
	}
}


void hdUIViewController::SetMouseDownCallbackForGroupedItems(const char *group, void *obj, void (*func)(void *, void *))
{
	if (!group || strlen(group) == 0) return;
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (strcmp(group, proxy->GetGroup()) == 0)
		{
			if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeButton)
				((hdUIButton *)(proxy->GetInterfaceElement()))->SetMouseDownListener(obj, func);
		}
	}
}


void hdUIViewController::SetMouseUpCallbackForGroupedItems(const char *group, void *obj, void (*func)(void *, void *))
{
	if (!group || strlen(group) == 0) return;
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (strcmp(group, proxy->GetGroup()) == 0)
		{
			if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeButton)
				((hdUIButton *)(proxy->GetInterfaceElement()))->SetMouseUpListener(obj, func);
		}
	}
}


void hdUIViewController::SetMouseDownCallbackForTaggedItems(const char *tag, void *obj, void (*func)(void *, void *))
{
	if (!tag || strlen(tag) == 0) return;
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (strcmp(tag, proxy->GetTag()) == 0)
		{
			if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeButton)
				((hdUIButton *)(proxy->GetInterfaceElement()))->SetMouseDownListener(obj, func);
		}
	}
}


void hdUIViewController::SetMouseUpCallbackForTaggedItems(const char *tag, void *obj, void (*func)(void *, void *))
{
	if (!tag || strlen(tag) == 0) return;
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (strcmp(tag, proxy->GetTag()) == 0)
		{
			if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeButton)
				((hdUIButton *)(proxy->GetInterfaceElement()))->SetMouseUpListener(obj, func);
		}
	}
}


void hdUIViewController::SetValueChangedCallbackForGroupedItems(const char *group, void *obj, void (*func)(void *, void *))
{
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (strcmp(group, proxy->GetGroup()) == 0)
		{
			if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeCheckbox)
				((hdUICheckbox *)(proxy->GetInterfaceElement()))->AddValueChangedListener(obj, func);
		}
	}
}


const hdUIReceiver * hdUIViewController::GetInterfaceItemWithTag(const char *tag)
{
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (tag && strcmp(tag, proxy->GetTag()) == 0)
		{
			return proxy->GetInterfaceElement();
		}
	}
	return NULL;
}


const hdUIReceiver * hdUIViewController::GetInterfaceItemWithGroup(const char *group)
{
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (group && strcmp(group, proxy->GetGroup()) == 0)
		{
			return proxy->GetInterfaceElement();
		}
	}
	return NULL;
}


const uint32 hdUIViewController::GetInterfaceItemsForGroup(const char *group, hdUIReceiver ** items, uint32 maxItems) const
{
	uint32 count = 0;
	
	for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
	{
		if (count >= maxItems) break;
		
		hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
		if (strcmp(group, proxy->GetGroup()) == 0)
		{
			items[count++] = (hdUIReceiver *)(proxy->GetInterfaceElement());
		}
	}
	return count;
}


void hdUIViewController::InitSounds() {}


void hdUIViewController::RefreshLayout(const e_hdInterfaceOrientation &orientation) {}


void hdUIViewController::ScaleToScreenDimensions(const float screenWidth, const float screenHeight)
{
	hdVec3 scale(screenWidth/m_width, screenHeight/m_height, 1.0f);
	if (scale.x != 1.0f || scale.y != 1.0f)
	{
		this->Scale(scale, GetWorldCenter());
		m_width = screenWidth;
		m_height = screenHeight;
		this->ResetAABB();
	}
	this->MoveTo(hdVec3(m_width * 0.5f, m_height * 0.5f, 0.0f));
}


void hdUIViewController::Draw()
{
	this->WillDraw();
	glEnable(GL_TEXTURE_2D);
	((hdUIContainer *)this)->Draw();
	glDisable(GL_TEXTURE_2D);
	this->DidDraw();
}


void hdUIViewController::WillDraw() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPushMatrix();
	hdglOrthof(0.0f, m_width, 
			   0.0f, m_height, 
			   -10000.0f, 10000.0f);
	
	glDepthFunc( GL_LEQUAL );
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void hdUIViewController::DidDraw() const
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
