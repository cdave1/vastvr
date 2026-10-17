/*
 *  hdIBFontPolygon.cpp
 *  hdkOSX
 *
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "hdIBFontPolygon.h"
#include "hdUIFontPolygon.h"

hdIBFontPolygon::hdIBFontPolygon() : hdIBProxy() 
{
	m_interfaceElement = NULL;
}


hdIBFontPolygon::hdIBFontPolygon(const char *fontFileName, 
							const hdVec3 &lower,
							const hdVec3 &upper) : hdIBProxy()
{
	this->SetFontFileName(fontFileName);
	this->SetAs2DBox(lower, upper);
	m_interfaceElement = NULL;
	m_tint.Set(0.0f, 0.0f, 0.0f, 1.0f);
	m_fontAlignment = hdIBFontPolygonAlignmentLeft;
	snprintf(m_text, 2048, "%s", "This is some placeholder text");
}


hdIBFontPolygon::~hdIBFontPolygon()
{
	delete m_interfaceElement;
}


hdUIReceiver * hdIBFontPolygon::CreateInterfaceElement()
{
	this->ResetAABB();
	if (!m_interfaceElement)
	{
		m_interfaceElement = new hdUIFontPolygon(m_fontFileName, m_text, NULL, this->GetAABB());
		((hdUIFontPolygon *)m_interfaceElement)->SetAs2DBox(this->GetAABB().lower, this->GetAABB().upper);
		((hdUIFontPolygon *)m_interfaceElement)->SetTint(m_tint.r, m_tint.g, m_tint.b, m_tint.a);
		SetFontAlignment(m_fontAlignment);
	}
	return m_interfaceElement;
}


void hdIBFontPolygon::SetText(const char *text)
{
	snprintf(m_text, 2048, "%s", text);
	if (m_interfaceElement)
	{
		((hdUIFontPolygon *)m_interfaceElement)->SetText(m_text);
	}
}


void hdIBFontPolygon::SetTint(const hdColor4f& tint)
{
	m_tint.Set(tint);
	if (m_interfaceElement)
	{
		((hdUIFontPolygon *)m_interfaceElement)->SetTint(m_tint.r, m_tint.g, m_tint.b, m_tint.a);
	}
}


void hdIBFontPolygon::SetFontAlignment(const hdIBFontPolygonAlignment_t alignment)
{
	m_fontAlignment = alignment;
	if (m_interfaceElement)
	{
		if (m_fontAlignment == hdIBFontPolygonAlignmentLeft)
		{
			((hdUIFontPolygon *)m_interfaceElement)->AlignLeft();
		}
		else if (m_fontAlignment == hdIBFontPolygonAlignmentCenter)
		{
			((hdUIFontPolygon *)m_interfaceElement)->AlignCenter();
		}
		else if (m_fontAlignment == hdIBFontPolygonAlignmentRight)
		{
			((hdUIFontPolygon *)m_interfaceElement)->AlignRight();
		}
	}
}


void hdIBFontPolygon::Draw() const
{
	if (m_interfaceElement)
		m_interfaceElement->Draw();
}