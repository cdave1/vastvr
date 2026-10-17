//
//  hdIBCheckbox.m
//  hdk.1.5.OSX
//
//  Copyright 2010 n/a. All rights reserved.
//

#include "hdIBCheckbox.h"
#include "hdUICheckbox.h"

hdIBCheckbox::hdIBCheckbox() : hdIBProxy()
{
	m_interfaceElement = NULL;
}


hdIBCheckbox::hdIBCheckbox(const char *textureOnNormal, 
			 const char *textureOnOver, 
			 const char *textureOffNormal,
			 const char *textureOffOver,
			 const hdVec3 &lower, 
			 const hdVec3 &upper) : hdIBProxy()
{
	this->SetTexture(textureOnNormal,	hdIBCheckboxTextureType_OnNormal);
	this->SetTexture(textureOnOver,		hdIBCheckboxTextureType_OnOver);
	this->SetTexture(textureOffNormal,	hdIBCheckboxTextureType_OffNormal);
	this->SetTexture(textureOffOver,	hdIBCheckboxTextureType_OffOver);

	this->SetAs2DBox(lower, upper);
	m_interfaceElement = NULL;
}


hdIBCheckbox::~hdIBCheckbox()
{
}


hdUIReceiver * hdIBCheckbox::CreateInterfaceElement()
{
	this->ResetAABB();
	this->ResetOBB();
	if (!m_interfaceElement)
	{
		m_interfaceElement = new hdUICheckbox(m_textureOnNormal, 
											  m_textureOnOver,
											  m_textureOffNormal,
											  m_textureOffOver,
											  NULL);
		((hdUICheckbox *)m_interfaceElement)->SetAs2DBox(this->GetAABB().lower,
														 this->GetAABB().upper);
	}
	return m_interfaceElement;
}


void hdIBCheckbox::Draw() const
{
	if (m_interfaceElement)
		m_interfaceElement->Draw();
}